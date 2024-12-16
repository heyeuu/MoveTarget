
inline float kp, ki;
inline float calculate_speed(float target_speed, float current_speed) {
    static float err, err_lowout, err_lowout_last, integral = 0;
    float output;
    err = target_speed - current_speed;
    float a = 0.7;

    err_lowout = err * (1 - a) + err_lowout_last * a;
    err_lowout_last = err_lowout;
    integral += err_lowout;

    // 限幅
    output = kp * err_lowout + ki * integral;
    if (output > 100)
        output = 100;
    if (output < -100)
        output = -100;
    return output;
}