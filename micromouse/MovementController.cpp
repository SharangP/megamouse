/***************
* MovementController Class Implementation
*
* Sharang Phadke
* Sameer Chauhan
***************/

#ifndef MOVEMENTCONTROLLER_CPP
#define MOVEMENTCONTROLLER_CPP

#include "MovementController.h"


double MovementController::input = 1;
double MovementController::output = 1;
double MovementController::setpoint = 1;

Motor * MovementController::right = new Motor(7, 8, 9);
Motor * MovementController::left = new Motor(4, 5, 6);

PID * MovementController::pid = new PID(&MovementController::input,
                                        &MovementController::output,
                                        &MovementController::setpoint,
                                        2, 5, 1, DIRECT);


void MovementController::updatePID(int state){
  switch(state){
    case 1:  //straight
      MovementController::input = ((double)SensorController::leftEncoder.read())
          /SensorController::rightEncoder.read();
      break;
    default:
      //?
      break;
  }
  // Somehow include IR values in this...
 // input = ((double)(input + ((double)SensorController::irSmooth[LEFT]/SensorController::irSmooth[RIGHT]) ))/2;
  
  pid->Compute();

}

void MovementController::goStraight(int * state){
  // decode the output
  // Must take into account of IR sensor values. 
  double leftSpeed = output*50;
  double rightSpeed = 50;
  right->setState(1, rightSpeed);
  left->setState(1, leftSpeed);
  
  if( SensorController::irSmooth[CENTER] > CENTERTHRESH ){
    Serial.println(SensorController::irSmooth[CENTER]);
    *state = STOP;
  }
  
//  if ( SensorController::leftEncoder.read() >= 1100 || SensorController::rightEncoder.read() >=1100){
//      *state = STOP;
//  }
  
  //int shit = 0
//  while(SensorController::irSmooth[CENTER] < CENTERTHRESH  ){
//    delay(100);
//    if(SensorController::irSmooth[LEFT] > SensorController::irSmooth[RIGHT] && shit !=1){
//     // Right wall is farther than left wall
//     shit = 1;
//      right->setState(1,right->power-2);
//      left->setState(1,left->power+2);
//    }
//    else if(SensorController::irSmooth[LEFT] < SensorController::irSmooth[RIGHT] && shit !=2){
//     // Left wall is farther than right wall
//     shit = 2;
//      right->setState(1,right->power+2);
//      left->setState(1,left->power-2);
//    }
//    
//    SensorController::printSensors();
//  }
//  if(right->power != 0 || left->power != 0){
//     brake(); 
//  }
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

void MovementController::brake(){
  Serial.println("Stopping");
  //right->setState(0,0);
  //left->setState(0,0);
  digitalWrite(right->enablePin, LOW);
  digitalWrite(left->enablePin, LOW);
}

void MovementController::accel(int startPow, int endPow, int time){}

#endif
