
#include <algorithm>
class pid_controller {
public:
    pid_controller(float kp, float ki, float kd = 0, float i_upper = 10, double i_lower = -10, float output_upper = 1, float output_lower = -1)
        : kp_(kp)
        , ki_(ki)
        , kd_(kd)
        , i_upper(i_upper)
        , i_lower(i_lower)
        , output_upper(output_upper)
        , output_lower(output_lower) { }

public:
    float calculate_speed(const float& target_speed, float& current_speed) {

        err = target_speed - current_speed;
        float a = 0.7;

        err_lowout = err * (1 - a) + err_lowout_last * a;
        diff = err_lowout - err_lowout_last;
        err_lowout_last = err_lowout;
        integral += err_lowout;

        integral = std::clamp(integral, i_lower, i_upper);

        output = kp_ * err_lowout + ki_ * integral + diff * kd_;
        output = std::clamp(output, output_lower, output_upper);
        return output;
    }

    void set_parameter(const float& kp, const float& ki) {
        kp_ = kp;
        ki_ = ki;
    }

private:
    float kp_ = 0, ki_ = 0, kd_ = 0;
    float i_upper = 10, i_lower = -10, output_upper = 1, output_lower = -1;
    float err = 0, err_lowout = 0, err_lowout_last = 0, integral = 0, diff = 0;
    float output;
};
