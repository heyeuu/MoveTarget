#pragma once
#include "usart.h"
#define RX_BUFFER_SIZE 4
inline uint8_t UART1_rx_buffer[RX_BUFFER_SIZE];
inline uint8_t UART2_rx;
inline int rx_index = 0;
extern float move_speed_target;
extern float rotate_speed_target;

inline void parse_commend() {
    uint8_t index = 1;
    uint8_t speed = 0;
    uint8_t cnt = 0;
    while (UART1_rx_buffer[index] != '\n' || UART1_rx_buffer[index] != '\r') {
        cnt++;
    }
    for (int i = cnt; i > 1; i--) {
        speed += 10 ^ (i - 1) * UART1_rx_buffer[cnt - i + 1];
    }
    if (UART1_rx_buffer[0] == 'm') {
        move_speed_target = speed;
    }
    if (UART1_rx_buffer[0] == 'r') {
        rotate_speed_target = speed;
    }
}
