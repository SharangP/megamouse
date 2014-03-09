/***************
* MovementController Class Implementation
*
* Sharang Phadke
* Sameer Chauhan
***************/

#ifndef MOVEMENTCONTROLLER_CPP
#define MOVEMENTCONTROLLER_CPP

#include "MovementController.h"


void MovementController::goStraight(){
  Serial.print("Going forward until ");
  Serial.println(CENTERTHRESH, DEC);
  right->setState(1, 50);
  left->setState(1, 50);
  
  while(SensorController::irSmooth[CENTER] < CENTERTHRESH){
    if(SensorController::irSmooth[LEFT] > SensorController::irSmooth[RIGHT]){
     // Right wall is farther than left wall
      right->setState(1,right->power-2);
      left->setState(1,left->power+2);
    } else if (SensorController::irSmooth[LEFT] < SensorController::irSmooth[RIGHT]){
     // Left wall is farther than right wall
      right->setState(1,right->power+2);
      left->setState(1,left->power-2);
    }
    Serial.println(SensorController::irSmooth[CENTER]);
  }

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
//  go();
  delay(1000);
}

void MovementController::turn(int dir){
  dir = RIGHT;
  right->setState(1,50);
  left->setState(2,50);
//  go();
  delay(750);
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


//void MovementController::accel(int startPow, int endPow, int time){}

#endif
