#include "parallax_encoder.h"

parallax_encoder::parallax_encoder ( DigitalIn& input, Ticker &encoder_ticker ) {
    din = &input;
    steps = 0;
    last = 0;
    encoder_ticker.attach(callback(this, &parallax_encoder::count_steps), 0.01);
}

void parallax_encoder::count_steps(){
    int value = din->read();
    if(!last && value) steps++;
    last = value;
}

int parallax_encoder::get_steps(){
    return steps;
}

float parallax_encoder::get_cm(){
    return steps*6.5*3.14/32;
}

void parallax_encoder::reset(){
    steps = 0;
}
