/***************
* MovementController Class Implementation
*
* Sharang Phadke
* Sameer Chauhan
***************/

#ifndef MOVEMENTCONTROLLER_CPP
#define MOVEMENTCONTROLLER_CPP

#include "MovementController.h"
#define THRESHOLD 350

//initialize motors and save sensors
MovementController::MovementController(SensorController * sensors){
 right = new Motor(3, 4, 5);
 left = new Motor(6, 7, 8);
 sensors->detectWalls(this->walls);
}

void MovementController::goStraight(){
  Serial.println("Going forward...");
  right->setState(1, 50);
  left->setState(1, 50);
  go();
  
  while(this->walls[CENTER] < THRESHOLD || this->walls[LEFT] < THRESHOLD || this->walls[RIGHT] < THRESHOLD){
    delay(64);
    sensors->detectWalls(this->walls);    
  }

  goBack();
  brake();
}

void MovementController::goLeft(){
}

void MovementController::goRight(){
}

void MovementController::goBack(){
  Serial.println("Going back...");
  right->setState(2,100);
  left->setState(2,100);
  go();
  delay(1000);
}

void MovementController::go(){
  analogWrite(right->enablePin, right->power);
  analogWrite(left->enablePin, left->power);
}

void MovementController::brake(){
  Serial.println("Stopping");
  digitalWrite(right->enablePin, LOW);
  digitalWrite(left->enablePin, LOW);
}

void MovementController::accel(int startPow, int endPow, int time){
    
}

#endif
