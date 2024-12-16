#pragma once
#include <stdint.h>

#include "module/VL53L0X.h"
statInfo_t_VL53L0X distanceStr1;
inline void init_tof() {
    initVL53L0X(1, &hi2c1);

    // Configure the sensor for high accuracy and speed in 20 cm.
    setSignalRateLimit(200);
    setVcselPulsePeriod(VcselPeriodPreRange, 10);
    setVcselPulsePeriod(VcselPeriodFinalRange, 14);
    setMeasurementTimingBudget(300 * 1000UL);
}
inline uint16_t get_distance() {
    uint16_t distance;
    distance = readRangeSingleMillimeters(&distanceStr1) / 10.0;
    return distance;
}