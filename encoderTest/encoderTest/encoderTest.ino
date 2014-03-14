/* Encoder Library - Basic Example
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * This example code is in the public domain.
 */

#include <Encoder.h>

#include <avr/interrupt.h>

#define p1 2
#define p2 10
#define p3 3
#define p4 11

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(p1, p2);
Encoder myEnc2(p3, p4);
//   avoid using pins with LEDs attached

volatile long oldPosition  = -999;
volatile long oldPosition2 = -999;
volatile long count = 0;
volatile long count2= 0;


void setup() {
  Serial.begin(9600);
  Serial.println("Basic Encoder Test:");
  // attachInterrupt(0, interFun, CHANGE);
  // attachInterrupt(1, interFun2, CHANGE);
}

void loop() {
  Serial.print("Enc 1: ");
  Serial.print(count);
  Serial.print(" Enc 2: ");
  Serial.println(count2);
  delay(100);
}


void interFun(){
  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    count++;
  }
  return;
}
void interFun2(){
  long newPosition = myEnc2.read();
  if (newPosition != oldPosition2){
    oldPosition2 = newPosition;
    count2++;
  }
}
