#pragma once
#include "base/can.hpp"
#include "can.h"
#include <algorithm>

typedef struct MotorStatus {
    int16_t angle;
    int16_t speed;
    int16_t torque;
    int16_t torque_current;
} MotorStatus;

inline auto m3508 = MotorStatus {};
inline constexpr auto id_3508 = 517;

inline auto m6020 = MotorStatus {};
inline constexpr auto id_6020 = 518;

inline auto header_tx = CAN_TxHeaderTypeDef {};

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
inline auto control = Control {};

inline void init_motor() {
    init_can(&hcan1);
    set_can_tx_header(header_tx);
}

inline void get_motor_status(const uint8_t data[8], MotorStatus& status) {
    status.angle = (data[0] << 8) | data[1];
    status.speed = (data[2] << 8) | data[3];
    status.torque = (data[4] << 8) | data[5];
    status.torque_current = (data[6] << 8) | data[7];
}

inline void set_motor_speed(double speed_3508, double speed_6020) {

    auto current_3508 = static_cast<int16_t>(std::clamp(speed_3508, -1., 1.) * 16384.);
    auto current_6020 = static_cast<int16_t>(std::clamp(speed_6020, -1., 1.) * 25000.);

    control[0] = current_3508;
    control[1] = current_6020;

    uint8_t data[8];
    data << control;

    uint32_t mail;

    HAL_CAN_AddTxMessage(&hcan1, &header_tx, data, &mail);
}

// // -25000~0~25000 for M6020
// inline void set_motor_current(const int16_t current, Motor* motor) {
//     uint8_t data[8];

//     data[2] = current >> 8;
//     data[3] = current | 0xff00;

//     HAL_CAN_AddTxMessage(motor->handle.hcan, &motor->handle.header_tx,
//         data, &motor->handle.mail);
// }
