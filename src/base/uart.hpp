#pragma once

#include <stdint.h>
#include <stdio.h>

#include "usart.h"

uint8_t const RX_BUFFER_SIZE = 10;
inline uint8_t UART1_rx_buffer[RX_BUFFER_SIZE];
inline uint8_t UART2_rx;
// inline int rx_index = 0;
extern float move_speed_target;
extern float rotate_speed_target;

inline void parse_commend() {
    uint8_t index = 1;
    uint8_t speed = 0;
    uint8_t cnt = 0;
    if (UART1_rx_buffer[0] != 'r' || UART1_rx_buffer[0] != 'm')
        ;
    else {
        while (UART1_rx_buffer[index] != '\n' || UART1_rx_buffer[index] != '\r') {
            cnt++;
        }

        for (int i = 1; i <= cnt; i++) {
            speed += UART1_rx_buffer[i] * 10 ^ (cnt - i);
        }

        if (UART1_rx_buffer[0] == 'm') {
            move_speed_target = speed;
        }
        if (UART1_rx_buffer[0] == 'r') {
            rotate_speed_target = speed;
        }
    }
}
