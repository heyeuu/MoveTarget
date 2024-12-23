
inline float kp = 0.4, ki = 0.1;
inline float calculate_speed(const float& target_speed, float& current_speed) {
    static float err, err_lowout, err_lowout_last, integral = 0;
    float output;
    err = target_speed - current_speed;
    float a = 0.7;

    err_lowout = err * (1 - a) + err_lowout_last * a;
    err_lowout_last = err_lowout;
    integral += err_lowout;

    integral += err_lowout;
    if (integral > 10)
        integral = 10;
    if (integral < -10)
        integral = -10;

    // 限幅
    output = kp * err_lowout + ki * integral;
    if (output > 1)
        output = 1;
    if (output < -1)
        output = -1;
    return output;
}