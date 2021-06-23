#ifndef BBCAR_H
#define BBCAR_H
#include "parallax_servo.h"
#include "parallax_encoder.h"
#include "parallax_ping.h"
#include "mbed.h"
class BBCar{
	public:
		BBCar( PwmOut &pin_servo0, PwmOut &pin_servo1, Ticker &servo_ticker );

		parallax_servo servo0;
		parallax_servo servo1;

		void controlWheel();
		void stop();
		void goStraight( double speed );
		// void goStraight_cm( double speed, double len );
		void spin( double speed );
		void setCalibTable( int len0, double pwm_table0[], double speed_table0[], int len1, double pwm_table1[], double speed_table1[] );
		void goStraightCalib( double speed );

		// turn left/right with a factor of speed
		void turn( double speed, double factor );

		// limit the value by max and min
		float clamp( float value, float max, float min );
		int turn2speed( float turn );
};

#endif
