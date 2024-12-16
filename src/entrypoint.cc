#include "entrypoint.hh"
#include "base/uart.hpp"
#include "module/motor.hpp"
#include "module/sensor.hpp"
#include "utility/pid.hpp"

#include "can.h"
#include "main.h"
#include "usart.h"

struct Control {
    int16_t current[4];

    int16_t& operator[](size_t index) {
        return current[index];
    }

    void friend operator<<(uint8_t be_data[8], Control& control) {
        auto le_data = reinterpret_cast<uint8_t*>(&control);

        be_data[0] = le_data[1];
        be_data[1] = le_data[0];

        be_data[2] = le_data[3];
        be_data[3] = le_data[2];

        be_data[4] = le_data[4];
        be_data[5] = le_data[5];

        be_data[6] = le_data[7];
        be_data[7] = le_data[6];
    }
};

float move_speed_target = 0;
float rotate_speed_target = 0;

float move_current_speed = 0;
float rotate_current_speed = 0;

// bool move_direction = false;
// bool rotate_direction = false;

bool system_enabled = false;

Motor motor3508; // id=1
Motor motor6020; // id=2
int16_t current = 8000;
auto control = Control {};

/// @brief 电机控制
void motor_control_loop() {

    // analyze the messages of global
    // calculate target current
    // send can message

    // float cal_move_speed = calculate_speed(move_speed_target, move_current_speed);
    // current = speed_to_current(cal_move_speed);
    // set_motor_current(current, &motor3508);
    // set_motor_current(10000, &motor6020);

    control[0] = current;
    control[1] = current;
    control[2] = current;
    control[3] = current;

    uint8_t data[8];
    data << control;

    HAL_CAN_AddTxMessage(&hcan1, &motor6020.handle.header_tx,
        data, &motor6020.handle.mail);

    // float cal_rota_speed = calculate_speed(rotate_speed_target, rotate_current_speed);
    // current = speed_to_current(cal_rota_speed);

    // current = speed_to_current(rotate_current_speed);
    // set_motor_current(current, &motor6020);
    // HAL_Delay(10);
}

uint8_t can_data_buffer[8];
CAN_RxHeaderTypeDef can_rx_header_buffer;
/// @brief 电机信息处理
void handle_motor_data() {

    // deal with the data from can
    // update the global values
    // 回报文头判断
    HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &can_rx_header_buffer, can_data_buffer);
    if (can_rx_header_buffer.StdId == 0x201) {
        get_motor_status(can_data_buffer, &motor3508);
        move_speed_target = motor3508.status.speed;
        // if (move_speed_target > 0) {
        //     move_direction = true;
        // } else {
        //     move_direction = false;
        // }
    } else if (can_rx_header_buffer.StdId == 0x204) {
        get_motor_status(can_data_buffer, &motor6020);
        rotate_current_speed = motor6020.status.speed;
        // if (rotate_current_speed > 0) {
        //     rotate_direction = true;
        // } else {
        //     rotate_direction = false;
        // }
    }
}

/// @brief 用户命令处理
void handle_command_data() {
    // handle watchdog
    // speed_to_current();
    // handle data
    // update the target values and boolean values

    if (UART1_rx_buffer[rx_index] == '\n' || UART1_rx_buffer[rx_index] == '\r') {
        parse_commend();
        rx_index
            = 0;
    } else if (rx_index < RX_BUFFER_SIZE) {
        rx_index++;
    }

    HAL_UART_Receive_IT(&huart1, UART1_rx_buffer, sizeof(UART1_rx_buffer));
}

/// @brief 传感器处理
void handle_sensor_data() {
    // handle the data from serial
    // parse the data
    // update the values
    // uint16_t distance = get_distance();
    // if (distance < 30) {
    //     move_speed_target = -move_speed_target;
    // }
}

void entrypoint() {

    // init_motor(&motor3508);
    init_motor(&motor6020);
    motor_control_loop();
    // init_tof();
    HAL_UART_Receive_IT(&huart1, UART1_rx_buffer, sizeof(UART1_rx_buffer));
    // HAL_UART_Receive_IT(&huart2, &UART2_rx, sizeof(UART2_rx));
    while (true) {

        // keep the led running
        HAL_Delay(100);
        HAL_GPIO_TogglePin(LEDA_GPIO_Port, LEDA_Pin);
        // HAL_GPIO_TogglePin(LEDB_GPIO_Port, LEDB_Pin);
        motor_control_loop();
    }
}
/*100ms per*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
    if (htim->Instance == TIM14)
        motor_control_loop();
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan) {
    if (hcan == &hcan1)
        handle_motor_data();
}
/*改引脚*/
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t size) {

    if (huart == &huart1)
        handle_command_data();

    if (huart == &huart2)
        handle_sensor_data();
    HAL_UART_Receive_IT(&huart2, &UART2_rx, sizeof(UART2_rx));
    HAL_UART_Receive_IT(&huart1, UART1_rx_buffer, sizeof(UART1_rx_buffer));
}
