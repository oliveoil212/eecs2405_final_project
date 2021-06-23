#ifndef BBCAR_RPC_H
#define BBCAR_RPC_H

#include "bbcar.h"
#include "mbed_rpc.h"

void RPC_stop(Arguments *in, Reply *out);
void RPC_goStraight(Arguments *in, Reply *out);
void RPC_turn(Arguments *in, Reply *out);
void RPC_spin(Arguments *in, Reply *out);
void RPC_park(Arguments *in, Reply *out);
#endif
