/***************
* MovementController Class Implementation
*
* Sharang Phadke
* Sameer Chauhan
***************/

#ifndef MOVEMENTCONTROLLER_CPP
#define MOVEMENTCONTROLLER_CPP

#include "MovementController.h"


double MovementController::movementSpeed = 75;
double MovementController::input = 1;
double MovementController::output = 1;
double MovementController::setpoint = 1;

Motor * MovementController::right = new Motor(7, 8, 9);
Motor * MovementController::left  = new Motor(4, 5, 6);

PID * MovementController::pidEncoder = new PID(&MovementController::input,
                                        &MovementController::output,
                                        &MovementController::setpoint,
                                        2, 3, 1, DIRECT);

PID * MovementController::pidIR = new PID(&SensorController::input,
                                        &SensorController::output,
                                        &SensorController::setpoint,
                                        2, 3, 1, DIRECT);

void MovementController::updatePID(int state){

  //TODO: Use pidEncoder and pidIR values (outputs) to inform locomotion
  //change left and right powers to go straight/turn etc. as needed

  //TODO: move forward/turn one block at a time

  switch(state){
    case 1:  //straight
      input = ((double)SensorController::leftEncoder.read())
          /SensorController::rightEncoder.read();

      SensorController::input = abs(SensorController::irSmooth[LEFT]
          - SensorController::irSmooth[RIGHT]);
      break;
    default:
      //?
      break;
  }
  pidEncoder->Compute();
  pidIR->Compute();
}

void MovementController::goStraight(int * state){
  // decode the output
  // Must take into account of IR sensor values. 
  // double leftSpeed = output*movementSpeed;
  // double rightSpeed = movementSpeed;

  //TODO: see what the output looks like!!

  double leftSpeed = SensorController::output*movementSpeed;
  double rightSpeed = movementSpeed;
  left->setState(1, leftSpeed);
  right->setState(1, rightSpeed);
  
  if( SensorController::irSmooth[CENTER] > CENTERTHRESH ){
    *state = STOP;
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

void MovementController::brake(){
  Serial.println("Stopping");
  digitalWrite(right->enablePin, LOW);
  digitalWrite(left->enablePin, LOW);
}

void MovementController::accel(int startPow, int endPow, int time){}

#endif
