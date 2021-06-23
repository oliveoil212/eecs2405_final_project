#include "parallax_servo.h"

parallax_servo::parallax_servo (PwmOut& pin) {
    pin.period(0.02);
    pwm = &pin;
    factor = 1;
    target_pwm_value = 0;
    current_pwm_value = 0;
    ramping_factor = 0;
}

// set new target pwm value (speed = pwm value)
void parallax_servo::set_speed( double value ) {
    target_pwm_value = value * factor;
    // you can determine ramping factor formula by yourself
    ramping_factor = abs(target_pwm_value - current_pwm_value) / 50;
    if (target_pwm_value > 200) target_pwm_value = 200;
    else if (target_pwm_value < -200) target_pwm_value = -200;
}

void parallax_servo::set_factor( double value ){
    factor = value;
}

// control servo input pwm, also ramping is mainly done in here
void parallax_servo::control(){   
    if (current_pwm_value > target_pwm_value) {
        if (current_pwm_value < target_pwm_value + ramping_factor)
            current_pwm_value = target_pwm_value;
        else current_pwm_value -= ramping_factor;
    }
    else if (current_pwm_value < target_pwm_value) {
        if (current_pwm_value > target_pwm_value - ramping_factor)
            current_pwm_value = target_pwm_value;
        else current_pwm_value += ramping_factor;
    }
    pwm->write((CENTER_BASE + current_pwm_value) / 20000);
}

void parallax_servo::set_calib_table( int len, double pwm_calib_table[], double speed_calib_table[] ) {
    table_len = len;
    pwm_table = pwm_calib_table;
    speed_table = speed_calib_table;
}

// input value is speed : cm/s
void parallax_servo::set_speed_by_cm( double value ){
    int i;
    double pwm_speed;
    double pwm_diff;
    double speed_diff;


    for (i=0; i<table_len; i++) {
        // values of speed table should be stored from small to large
        if (value < speed_table[i]) {
            // if less then table, take smallest value
            if (i==0) {
                set_speed(pwm_table[0]);
                return;
            }
            // interpolation
            pwm_diff = pwm_table[i] - pwm_table[i-1];
            speed_diff = speed_table[i] - speed_table[i-1];
            pwm_speed = pwm_table[i-1] + pwm_diff * ((value-speed_table[i-1])/speed_diff);
            set_speed(pwm_speed);
            return;
        }
    }
    // if exceed the table, take largest value
    pwm_speed = pwm_table[table_len-1];
    set_speed(pwm_speed);
}
