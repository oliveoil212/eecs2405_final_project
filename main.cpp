#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"
BufferedSerial pc(USBTX, USBRX);
BufferedSerial xbee(D1, D0);
BufferedSerial openmv(A1, A0);
Ticker servo_ticker;
Ticker encoder_ticker;
PwmOut pin5(D11), pin6(D13);
DigitalIn encoder(D12);
volatile int steps;
volatile int last;
BBCar car(pin5, pin6, servo_ticker);
DigitalInOut ping(D2);
DigitalOut ping_to_openmv(D5);
parallax_ping PNG(ping);
void encoder_control() {
   int value = encoder;
   if (!last && value) steps++;
   last = value;
}
void pingpingpongpong(){
   float distance;
   while(1) {
      distance = PNG.ping_cm();
      if(distance < 15) {
         ping_to_openmv = 1;
      }
      else
         ping_to_openmv = 0;
      // wait_us(500);
      printf("ping = %f\n" ,distance);
   }
}
void openmvserial(){
   char buf[256], outbuf[256];
   FILE *devin = fdopen(&openmv, "r");
   FILE *devout = fdopen(&openmv, "w");
   while (1) {
      memset(buf, 0, 256);
      for( int i = 0; ; i++ ) {
         char recv = fgetc(devin);
         if(recv == '\n') {
            printf("\r\n");
            break;
         }
         buf[i] = fputc(recv, devout);
      }
   RPC::call(buf, outbuf);
   }
}
int main() {
   // float distance;
   // while(1) {
   //    distance = PNG.ping_cm();
   //    if(distance < 15) {
   //       ping_to_openmv = 0;
   //    }
   //    else
   //       ping_to_openmv = 1;
   //       printf("ping = %f" ,distance);
   // }
   encoder_ticker.attach(&encoder_control, 10ms);
   Thread t;
   Thread p;
   p.start(&pingpingpongpong);
   t.start(&openmvserial);
   char buf[256], outbuf[256];
   FILE *devin = fdopen(&xbee, "r");
   FILE *devout = fdopen(&xbee, "w");
   while (1) {
      memset(buf, 0, 256);
      for( int i = 0; ; i++ ) {
         char recv = fgetc(devin);
         if(recv == '\n') {
            printf("\r\n");
            break;
         }
         buf[i] = fputc(recv, devout);
      }
   RPC::call(buf, outbuf);
   }
}
