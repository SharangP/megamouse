/***************
* Motor Class Implementation
*
* Sharang Phadke
* Sameer Chauhan
***************/

#ifndef MOTOR_CPP
#define MOTOR_CPP

#include "Motor.h"


Motor::Motor(int pin1, int pin2, int enablePin){
  this->enablePin = enablePin;
  this->pin1 = pin1;
  this->pin2 = pin2;
  this->power = 0;
  this->state = -1;
  
  pinMode(enablePin, OUTPUT);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  
  digitalWrite(enablePin, LOW);
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
}

void Motor::setState(int state, double power){
  this->power = power;
  this->state = state;
  
  switch (state){
    case 0:  //lock
      digitalWrite(this->pin1, LOW);
      digitalWrite(this->pin2, LOW);
      break;
    case 1:  //forward
      digitalWrite(this->pin1, HIGH);
      digitalWrite(this->pin2, LOW);
      break;
    case 2:  //backward
      digitalWrite(this->pin1, LOW);
      digitalWrite(this->pin2, HIGH);
      break;
  }
  
  analogWrite(this->enablePin, power);
}

#endif
