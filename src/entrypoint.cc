#include "entrypoint.hh"
#include "module/motor.hpp"
#include "utility/pid.hpp"

#include "can.h"
#include "main.h"
#include "tim.h"
#include "usart.h"
#include <cstring>
float move_speed_target = 0.0;
float rotate_speed_target = 0.0;

float move_current_speed = 0.0;
float rotate_current_speed = 0.0;

bool move_direction = true;
bool last_move_direction = true;

GPIO_PinState remote_high_state;
GPIO_PinState nearby_high_state;
GPIO_PinState remote_read_state;
GPIO_PinState nearby_read_state;

bool current_button_state = false;

bool system_enabled
    = false;

pid_controller controller_3508(2, 0.002, 0.02);
pid_controller controller_6020(40, 0.4);

uint16_t distance;
float speed_output_6020;
float speed_output_3508;

uint8_t can_data_buffer[8];
uint8_t receive_commend_buffer[10];
uint8_t receive_sensor_buffer[10];
CAN_RxHeaderTypeDef can_rx_header_buffer;

/// @brief 电机控制
void motor_control_loop() {

    // analyze the messages of global
    // calculate target current
    // send can message

    speed_output_6020 = controller_6020.calculate_speed(rotate_speed_target, rotate_current_speed);
    speed_output_3508 = controller_3508.calculate_speed(move_speed_target, move_current_speed);
    set_motor_speed(speed_output_3508, speed_output_6020);
}

/// @brief 电机信息处理
void handle_motor_data() {

    // deal with the data from can
    // update the global values
    // 回报文头判断
    HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &can_rx_header_buffer, can_data_buffer);
    if (can_rx_header_buffer.StdId == id_3508) {
        get_motor_status(can_data_buffer, m3508);
        move_current_speed = m3508.speed / 2000.;

    } else if (can_rx_header_buffer.StdId == id_6020) {
        get_motor_status(can_data_buffer, m6020);
        rotate_current_speed = m6020.speed / 120.;
    }
}

struct __packed Data {
    float move_speed;
    float rotate_speed;
} data;
/// @brief 用户命令处理
void handle_command_data() {
    // handle watchdog
    // handle data
    // // update the target values and boolean values

    std::memcpy(&data, receive_commend_buffer, sizeof(data));

    if (move_direction == true) {
        move_speed_target = data.move_speed;
    } else if (move_direction == false) {
        move_speed_target = -data.move_speed;
    }
    rotate_speed_target = data.rotate_speed;
}

/// @brief 传感器处理
void handle_sensor_data() {
    // handle the data from serial
    // parse the data
    // update the values

    // static bool button_pressed = true;
    current_button_state = (HAL_GPIO_ReadPin(remote_read_GPIO_Port, remote_read_Pin) == GPIO_PIN_SET) || (HAL_GPIO_ReadPin(nearby_read_GPIO_Port, nearby_read_Pin) == GPIO_PIN_SET);
    if (current_button_state == true && last_move_direction != move_direction) {
        move_direction = !move_direction;
        last_move_direction = move_direction;
    } else if (current_button_state == false) {
        last_move_direction = !move_direction;
    }
}

void entrypoint() {

    init_motor();

    HAL_UARTEx_ReceiveToIdle_IT(&huart1, receive_sensor_buffer, sizeof(receive_sensor_buffer));
    HAL_TIM_Base_Start_IT(&htim14);

    HAL_GPIO_WritePin(remote_high_GPIO_Port, remote_high_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(nearby_high_GPIO_Port, nearby_high_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(remote_low_GPIO_Port, remote_low_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(nearby_low_GPIO_Port, nearby_low_Pin, GPIO_PIN_RESET);
    while (true) {
        // motor_control_loop();
        // keep the led running
        HAL_GPIO_TogglePin(LEDA_GPIO_Port, LEDA_Pin);
    }
}

extern "C" {
/*100ms per*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
    if (htim->Instance == TIM14) {
        motor_control_loop();
        handle_sensor_data();
    }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan) {
    if (hcan == &hcan1)
        handle_motor_data();
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t size) {
    if (huart == &huart1) {
        if (size == 8)
            handle_command_data();
        HAL_UARTEx_ReceiveToIdle_IT(&huart1, receive_commend_buffer, size);
    }
}
}
