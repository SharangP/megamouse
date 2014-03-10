/***************
* MovementController Class Implementation
*
* Sharang Phadke
* Sameer Chauhan
***************/

#ifndef MOVEMENTCONTROLLER_CPP
#define MOVEMENTCONTROLLER_CPP

#include "MovementController.h"


Motor * MovementController::left = new Motor(6, 7, 8);
Motor * MovementController::right = new Motor(3, 4, 5);


void MovementController::goStraight(){
  Serial.print("Going forward until ");
  Serial.println(CENTERTHRESH, DEC);
  right->setState(1, 50);
  left->setState(1, 50);
  int shit = 0;

  while(SensorController::irSmooth[CENTER] < CENTERTHRESH  ){
    delay(100);
    if(SensorController::irSmooth[LEFT] > SensorController::irSmooth[RIGHT] && shit !=1){
     // Right wall is farther than left wall
     shit = 1;
      right->setState(1,right->power-2);
      left->setState(1,left->power+2);
    }
    else if(SensorController::irSmooth[LEFT] < SensorController::irSmooth[RIGHT] && shit !=2){
     // Left wall is farther than right wall
     shit = 2;
      right->setState(1,right->power+2);
      left->setState(1,left->power-2);
    }
    
    SensorController::printSensors();
  }
  if(right->power != 0 || left->power != 0){
     brake(); 
  }
}

void MovementController::goLeft(){
}

void MovementController::goRight(){
}

void MovementController::goBack(){
  Serial.println("Going back...");
  right->setState(2,100);
  left->setState(2,100);
  delay(1000);
}

void MovementController::turn(int dir){
  dir = RIGHT;
  right->setState(1,50);
  left->setState(2,50);
  delay(750);
}


void MovementController::go(){
  analogWrite(right->enablePin, right->power);
  analogWrite(left->enablePin, left->power);
}

void MovementController::brake(){
  Serial.println("Stopping");
  right->setState(0,0);
  left->setState(0,0);
 // digitalWrite(right->enablePin, LOW);
  //digitalWrite(left->enablePin, LOW);
}

void MovementController::accel(int startPow, int endPow, int time){}

#endif
