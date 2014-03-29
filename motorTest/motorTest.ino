/***************
*  Motor Test
*
*  Sharang Phadke
*  Sameer Chauhan
***************/

#include "Motor.h"

void slowToStop(Motor right, Motor left, int time){
  int i;
  float rightDecrement = ((float)right.power)/time;
  float leftDecrement = ((float)left.power)/time;
  float rightPowerFloat = right.power;
  float leftPowerFloat = left.power;

  for(i = 0; i < time; i++){
    rightPowerFloat -= rightDecrement;
    leftPowerFloat -= leftDecrement;
    right.power = (int)rightPowerFloat;
    left.power = (int)leftPowerFloat;

    analogWrite(right.enablePin, right.power);
    analogWrite(left.enablePin, left.power);
    delay(1);
  }
}

void timedMove(Motor right, Motor left, int time){
  analogWrite(right.enablePin, right.power);
  analogWrite(left.enablePin, left.power);
  delay(time-1000);
  slowToStop(right, left, 1000);
  digitalWrite(right.enablePin, LOW);
  digitalWrite(left.enablePin, LOW);
}


Motor rightMotor(7, 8, 9);
Motor leftMotor(4, 5, 6);


void setup(){
  rightMotor.setState(0, 0);
  leftMotor.setState(0, 0);
}


void loop(){
//  rightMotor.setState(1, 150);
//  leftMotor.setState(1, 200);
//  timedMove(rightMotor, leftMotor, 2000);
  rightMotor.setState(1, 50);
  leftMotor.setState(1, 50);
//  timedMove(rightMotor, leftMotor, 3000);
  delay(500);
  rightMotor.setState(0,0);
  leftMotor.setState(0,0);
  delay(5000);
}


