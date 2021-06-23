#include "bbcar.h"
// #include "mbed.h"
BBCar::BBCar( PwmOut &pin_servo0, PwmOut &pin_servo1, Ticker &servo_ticker ):servo0(pin_servo0), servo1(pin_servo1){
    servo0.set_speed(0);
    servo1.set_speed(0);
    servo_ticker.attach(callback(this, &BBCar::controlWheel), 20ms);
}

void BBCar::controlWheel(){
    servo0.control();
    servo1.control();
}

void BBCar::stop(){
    servo0.set_factor(1);
    servo1.set_factor(1);
    servo0.set_speed(0);
    servo1.set_speed(0);
}

void BBCar::goStraight( double speed ){\
    servo0.set_factor(1);
    servo1.set_factor(1);
    servo0.set_speed(speed);
    servo1.set_speed(-speed);
}
// void BBCar::goStraight_cm( double speed, double len ){
//     servo0.set_factor(1);
//     servo1.set_factor(1);
//     servo0.set_speed(100);
//     servo1.set_speed(-100);
//     if(len > 19) {
//         // car.goStraight(100); /// 100 1 19.5 100 0.5 7.5cm 100 0.25 3c,
//         ThisThread::sleep_for((1000 * len /19.5)ms);
//     } else if (len > 7) {
//         // car.goStraight(100); /// 100 1 19.5 100 0.5 7.5cm 100 0.25 3c,
//         ThisThread::sleep_for((500 * len /7.5)ms);
//     }else {
//         // car.goStraight(100); /// 100 1 19.5 100 0.5 7.5cm 100 0.25 3c,
//         ThisThread::sleep_for((250 * len /3)ms);
//     }
// }
void BBCar::spin( double speed ){
    servo0.set_factor(1);
    servo1.set_factor(1);
    servo0.set_speed(speed);
    servo1.set_speed(speed);
}

void BBCar::setCalibTable( int len0, double pwm_table0[], double speed_table0[], int len1, double pwm_table1[], double speed_table1[] ){
    servo0.set_calib_table(len0, pwm_table0, speed_table0);
    servo1.set_calib_table(len1, pwm_table1, speed_table1);
}
void BBCar::goStraightCalib ( double speed ){
    servo0.set_factor(1);
    servo1.set_factor(1);
    servo0.set_speed_by_cm(speed);
    servo1.set_speed_by_cm(-speed);
}

/*	speed : speed value of servo
    factor: control the speed value with 0~1
            control left/right turn with +/-
*/
void BBCar::turn( double speed, double factor ){
    if(factor>0){
        servo0.set_factor(factor);
        servo1.set_factor(1);
    }
    else if(factor<0){
        servo0.set_factor(1);
        servo1.set_factor(-factor);
    }
    servo0.set_speed(speed);
    servo1.set_speed(-speed);

}

float BBCar::clamp( float value, float max, float min ){
    if (value > max) return max;
    else if (value < min) return min;
    else return value;
}

int BBCar::turn2speed( float turn ){
    return 25+abs(25*turn);
}


