/***************
*  Motor Test
*
*  Sharang Phadke
*  Sameer Chauhan
***************/

#include "Motor.h"

void timedMove(Motor right, Motor left, int time){
  analogWrite(right.enablePin, right.power);
  analogWrite(left.enablePin, left.power);
  delay(time);
  digitalWrite(right.enablePin, LOW);
  digitalWrite(left.enablePin, LOW);
}


Motor rightMotor(3, 4, 5);
Motor leftMotor(6, 7, 8);


void setup(){
  rightMotor.setState(0, 0);
  leftMotor.setState(0, 0);
}


void loop(){
  rightMotor.setState(1, 100);
  leftMotor.setState(1, 100);
  timedMove(rightMotor, leftMotor, 1000);
  rightMotor.setState(2, 100);
  leftMotor.setState(2, 100);
  timedMove(rightMotor, leftMotor, 1000);
  rightMotor.setState(0, 100);
  leftMotor.setState(0, 100);
  timedMove(rightMotor, leftMotor, 1000);
}


