#pragma once

#include "can.h"

inline void set_can_tx_header(CAN_TxHeaderTypeDef& header) {
    header.IDE = CAN_ID_STD;
    header.StdId = 0X1FF;
    header.RTR = CAN_RTR_DATA;
    header.DLC = 8;
    header.TransmitGlobalTime = DISABLE;
}

// todo
inline HAL_StatusTypeDef can_filter_init(CAN_HandleTypeDef* hcan) {

    CAN_FilterTypeDef config;

    // 报文头结构体的赋值
    // 此处配置为接收全部报文，以便于测试
    config.FilterActivation = ENABLE; // 启用过滤器
    config.FilterBank = 0; // 将要初始化的过滤器组
    config.SlaveStartFilterBank = 0; // 从模式下的过滤器组
    config.FilterMode = CAN_FILTERMODE_IDMASK; // 掩码模式
    config.FilterScale = CAN_FILTERSCALE_32BIT; // 32位宽
    config.FilterFIFOAssignment = CAN_FILTER_FIFO0; // 配置邮箱0
    config.FilterIdHigh = 0x0000; // 高位0
    config.FilterIdLow = 0x0000; // 低位0
    config.FilterMaskIdHigh = 0x0000; // 掩码高位不检测
    config.FilterMaskIdLow = 0x0000; // 掩码低位不检测

    // 将配置加载进CAN 1中
    return HAL_CAN_ConfigFilter(hcan, &config);
}
inline void init_can(CAN_HandleTypeDef* hcan) {

    if (can_filter_init(hcan))
        Error_Handler();

    if (HAL_CAN_Start(hcan))
        Error_Handler();

    if (HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING))
        Error_Handler();
}
