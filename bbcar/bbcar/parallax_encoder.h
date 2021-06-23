#ifndef PARALLAX_ENCODER_H
#define PARALLAX_ENCODER_H

#include "mbed.h"

class parallax_encoder {
    public:
        parallax_encoder( DigitalIn& input, Ticker &encoder_ticker );

        void count_steps();
        int get_steps();
        float get_cm();
        void reset();
        operator int() { return steps; };
        
    private:
        int steps;
        int last;
        DigitalIn *din;
};

#endif
