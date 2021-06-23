#include "bbcar_rpc.h"
RPCFunction rpcStop(&RPC_stop, "stop");
RPCFunction rpcCtrl(&RPC_goStraight, "goStraight");
RPCFunction rpcTurn(&RPC_turn, "turn");
RPCFunction rpcSpin(&RPC_spin, "spin");
RPCFunction rpcPark(&RPC_park, "goParking");
RpcDigitalOut mode_pin(D7,"mode_pin");
RpcDigitalOut enalbe_pin(D6,"enable_pin");
extern BBCar car;
extern int steps, last;
extern void encoder_control();
extern Ticker encoder_ticker;
void RPC_stop (Arguments *in, Reply *out)   {
    car.stop();
    return;
}

void RPC_goStraight (Arguments *in, Reply *out)   {
    int speed = in->getArg<double>();
    car.goStraight(speed);
    return;
}

void RPC_turn (Arguments *in, Reply *out)   {
    int speed = in->getArg<double>();
    double turn = in->getArg<double>();
    car.turn(speed,turn);
    return;
}
void RPC_spin(Arguments *in, Reply *out) {
    int speed = in->getArg<double>();
    car.spin(speed);
    return;
}
// void RPC_park(Arguments *in, Reply *out) {
//     int speed = in->getArg<double>();
//     car.spin(speed);
//     return;
// }
void RPC_park(Arguments *in, Reply *out)   {
    int d1 = in->getArg<double>();
    int d2 = in->getArg<double>();
    int position= in->getArg<int>();
    int delay;
    encoder_ticker.attach(&encoder_control, 10ms);
   steps = 0;
   last = 0;

   if(position == -1){
        car.spin(-200);
        ThisThread::sleep_for(350ms); //300 90 600 180
        car.stop();
    }
    else if (position > 0) {
        car.spin(200);
        delay = 350 * position;
        ThisThread::sleep_for(delay); //300 90 600 180
        car.stop();
    }
    else {
        position = 0;
    }
    ThisThread::sleep_for(delay);
   car.goStraight(-100); /// 100 1 19.5 100 0.5 7.5cm 100 0.25 3c,
   delay = 1000.0 / 15 * d1;
   ThisThread::sleep_for(delay);
   car.stop();
   ThisThread::sleep_for(150ms);
    
        car.spin(-200);
        delay =450;
        ThisThread::sleep_for(delay); //300 90 600 180
        car.stop();
    ThisThread::sleep_for(150ms);
   car.goStraight(-100); /// 100 1 19.5 100 0.5 7.5cm 100 0.25 3c,
   delay = 1000.0 / 15 * (d2+5);
   ThisThread::sleep_for(delay);
   car.stop();
}
