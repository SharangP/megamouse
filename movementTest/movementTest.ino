#define ENCODER_USE_INTERRUPTS
#include <Encoder.h>
#include "Motor.h"

Encoder leftEnc(3,11);
Encoder rightEnc(2,10);
Motor rightMotor(7, 8, 9);
Motor leftMotor(4, 5, 6);


int irPins[3] = {3,4,5};
int leftEncVal = 0;
int rightEncVal = 0;

void stopAndReset(){
  leftMotor.setState(0,0);
  rightMotor.setState(0,0);
  leftEnc.write(0);
  rightEnc.write(0);
}



void setup(){
  Serial.begin(9600);
  Serial.println("MicroMouse Test Begin");
  leftMotor.setState(0,0);
  rightMotor.setState(0,0);
}



void loop(){
  leftMotor.setState(1,50);
  rightMotor.setState(1,50);
 // leftEncVal = leftEnc.read();
  //rightEncVal = rightEnc.read();
  
  while(leftEnc.read() < 1400 && rightEnc.read() < 1400){ 
//    if(leftEncVal< rightEncVal){
//       // Left side is spinnign slower 
//       leftMotor.setState(1,leftMotor.power+1);
//       rightMotor.setState(1,leftMotor.power-1);
//    } else if ( leftEncVal > rightEncVal ){
//       leftMotor.setState(1,leftMotor.power-1);
//       rightMotor.setState(1,leftMotor.power+1);
//    }
//    leftEncVal = leftEnc.read();
//    rightEncVal = rightEnc.read();
//    Serial.println(leftEnc.read());
//    Serial.println(rightEnc.read());

  }
  stopAndReset();
  delay(10000);
  
  
}



