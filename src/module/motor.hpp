#pragma once
#include "base/can.hpp"
#include "can.h"
typedef struct MotorStatus {
    int16_t angle;
    int16_t speed;
    int16_t torque;
} MotorStatus;

typedef struct MotorHandle {
    CAN_HandleTypeDef* hcan;
    CAN_RxHeaderTypeDef header_rx;
    CAN_TxHeaderTypeDef header_tx;
    uint32_t mail;
} MotorHandle;

typedef struct Motor {
    MotorHandle handle;
    MotorStatus status;
} Motor;
inline void init_motor(Motor* motor) {
    motor->handle.hcan = &hcan1;

    init_can(motor->handle.hcan);
    set_can_tx_header(&motor->handle.header_tx);
}

inline void get_motor_status(const uint8_t data[8], Motor* motor) {
    motor->status.angle = (data[0] << 8) | data[1];
    motor->status.speed = (data[2] << 8) | data[3];
    motor->status.torque = (data[4] << 8) | data[5];
}
// -25000~0~25000 for M6020
inline void set_motor_current(const int16_t current, Motor* motor) {
    uint8_t data[8];

    data[2] = current >> 8;
    data[3] = current | 0xff00;

    HAL_CAN_AddTxMessage(motor->handle.hcan, &motor->handle.header_tx,
        data, &motor->handle.mail);
}

/*speed:-100~100*/
inline uint16_t speed_to_current(float& speed) {
    return 16384 / 100.0 * speed;
}
// inline void change_speed(const float& target, float& now) {
//     if (now < target) {
//         now += 1;
//     } else if (now > target) {
//         now -= 1;
//     }
// }