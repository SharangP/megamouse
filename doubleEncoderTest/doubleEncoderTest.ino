/* Encoder Library - TwoKnobs Example
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * This example code is in the public domain.
 */
#define ENCODER_USE_INTERRUPTS
#include <Encoder.h>

// Change these pin numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder knobLeft(3, 11);
Encoder knobRight(2, 10);
//   avoid using pins with LEDs attached


long positionLeft  = -999;
long positionRight = -999;
long countLeft = 0;
long countRight =0;



void interFun(){
 countLeft++; 
}

void setup() {
  Serial.begin(9600);
  Serial.println("TwoKnobs Encoder Test:");
//  attachInterrupt(0,interFun,CHANGE);
//  attachInterrupt(1,interFun,CHANGE);
}


void loop() {
  long newLeft, newRight;
  newLeft = knobLeft.read();
  newRight = knobRight.read();
  if (newLeft != positionLeft || newRight != positionRight) {
    Serial.print("Left = ");
    Serial.print(knobLeft.read());
    Serial.print(", Right = ");
    Serial.print(knobRight.read());
    Serial.println();
    positionLeft = newLeft;
    positionRight = newRight;
  }
  // if a character is sent from the serial monitor,
  // reset both back to zero.
  if (Serial.available()) {
    Serial.read();
    Serial.println("Reset both knobs to zero");
//    Serial.println(knobLeft.read());
//    Serial.println(knobRight.read());
    knobLeft.write(0);
    knobRight.write(0);
  }
}


