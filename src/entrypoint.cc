#include "entrypoint.hh"
#include "base/uart.hpp"
#include "module/motor.hpp"
#include "module/sensor.hpp"
#include "utility/pid.hpp"

#include "can.h"
// #include "iwdg.h"
#include "main.h"
#include "usart.h"

float move_speed_target = -0.15;
float rotate_speed_target = 0.0;

float move_current_speed = 0;
float rotate_current_speed = 0;

// bool move_direction = false;
// bool rotate_direction = false;

bool system_enabled = false;

// int16_t control_volume_6020;

float speed_output_6020 = 0;
float speed_output_3508 = 0;
/// @brief 电机控制
void motor_control_loop() {

    // analyze the messages of global
    // calculate target current
    // send can message

    speed_output_6020 = calculate_speed(rotate_speed_target, rotate_current_speed);
    speed_output_3508 = calculate_speed(move_speed_target, move_current_speed);
    set_motor_speed(speed_output_3508, 0);

    HAL_Delay(10);
}

uint8_t can_data_buffer[8];
CAN_RxHeaderTypeDef can_rx_header_buffer;
/// @brief 电机信息处理
void handle_motor_data() {

    // deal with the data from can
    // update the global values
    // 回报文头判断
    HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &can_rx_header_buffer, can_data_buffer);
    if (can_rx_header_buffer.StdId == id_3508) {
        get_motor_status(can_data_buffer, m3508);
        move_current_speed = m3508.speed / 400.0;
        // if (m3508.torque_current > 20000 || m3508.torque_current < -20000) {
        //     move_speed_target = -move_speed_target;
        // }
        // if (move_speed_target > 0) {
        //     move_direction = true;
        // } else {
        //     move_direction = false;
        // }
    } else if (can_rx_header_buffer.StdId == id_6020) {
        get_motor_status(can_data_buffer, m6020);
        rotate_current_speed = m6020.speed / 220.;
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
    // handle data
    // update the target values and boolean values

    HAL_UART_Receive_IT(&huart1, UART1_rx_buffer, sizeof(UART1_rx_buffer));
    parse_commend();
}

/// @brief 传感器处理
void handle_sensor_data() {
    // handle the data from serial
    // parse the data
    // update the values

    // uint16_t distance = get_distance();
    // if (distance < 30 || distance > 200)
    //     toggle_speed(move_speed_target);
}

void entrypoint() {

    init_motor();
    // init_tof();

    HAL_UART_Receive_IT(&huart1, UART1_rx_buffer, sizeof(UART1_rx_buffer));
    // HAL_UART_Receive_IT(&huart2, &UART2_rx, sizeof(UART2_rx));
    while (true) {
        motor_control_loop();
        // keep the led running
        HAL_Delay(100);
        HAL_GPIO_TogglePin(LEDA_GPIO_Port, LEDA_Pin);
        // motor_control_loop();
    }
}
/*1s per*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
    uint8_t uart1_tx;
    if (htim->Instance == TIM14) {
        motor_control_loop();
        // HAL_UART_Transmit_IT(&huart1, &uart1_tx, 1);
    }
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
void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart) {
    if (huart->Instance == USART1) {
        // HAL_IWDG_Refresh(&hiwdg);
    }
}
