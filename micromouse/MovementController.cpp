/***************
* MovementController Class Implementation
*
* Sharang Phadke
* Sameer Chauhan
***************/

#ifndef MOVEMENTCONTROLLER_CPP
#define MOVEMENTCONTROLLER_CPP

#include "MovementController.h"


double MovementController::movementSpeed = 40;
double MovementController::moveSpeedRight = MovementController::movementSpeed;
double MovementController::moveSpeedLeft = MovementController::movementSpeed;
double MovementController::movementSpeedAdj = 0;
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
                                        10, 10, 5, DIRECT);

void MovementController::updatePID(int state){

  //TODO: Use pidEncoder and pidIR values (outputs) to inform locomotion
  //change left and right powers to go straight/turn etc. as needed

  //TODO: move forward/turn one block at a time

  switch(state){
    case 1:  //straight 
/* PID stuff that is not used
      input = ((double)SensorController::leftEncoder.read())
          /SensorController::rightEncoder.read();

      //SensorController::input = abs(SensorController::irSmooth[LEFT]
      //    - SensorController::irSmooth[RIGHT]);
      SensorController::setpoint = 0; // Sensor values are normalized so want 0
      SensorController::input = SensorController::irSmooth[RIGHT];
*/
      // Follow a wall that exists  
      if (SensorController::irSmooth[RIGHT] < 2*11 && SensorController::irSmooth[RIGHT] > -2*11){//2*SensorController::sensorSigma[RIGHT] ) {
        moveSpeedLeft  = movementSpeed + 0.75*SensorController::irSmooth[RIGHT];
        moveSpeedRight = movementSpeed - 0.75*SensorController::irSmooth[RIGHT];
      } else if (SensorController::irSmooth[LEFT] < 2*11 && SensorController::irSmooth[LEFT] > -2*11){//2*SensorController::sensorSigma[LEFT]){
        moveSpeedLeft  = movementSpeed - 0.75*SensorController::irSmooth[LEFT];
        moveSpeedRight = movementSpeed + 0.75*SensorController::irSmooth[LEFT];
      } else{
        moveSpeedRight = movementSpeed;
        moveSpeedLeft  = movementSpeed;
      }
      
      break;
    default:
      //?
      break;
  }
  // pidEncoder->Compute();
  // pidIR->Compute();
}

void MovementController::goStraight(int * state){
  // decode the output
  // Must take into account of IR sensor values. 
  // double leftSpeed = output*movementSpeed;
  // double rightSpeed = movementSpeed;

  //TODO: see what the output looks like!!
  //Serial.print("Encoder PID Output: ");
  //Serial.println(output);
  
  // Serial.print("Setpoint/Sensor Val/Sensor PID: ");
  // Serial.print(SensorController::setpoint);
  // Serial.print("/");
  // Serial.print(SensorController::irSmooth[RIGHT]);
  // Serial.print("/");
  // Serial.println(SensorController::output);

  // double leftSpeed = moveSpeedRight + movementSpeedAdj;
  // double rightSpeed = movementSpeed - movementSpeedAdj;

  // Serial.print("Motor Speed left/right: ");
  // Serial.print(leftSpeed);
  // Serial.print(" / ");
  // Serial.println(rightSpeed);
  // Serial.println("");

  left->setState(1, moveSpeedLeft);
  right->setState(1, moveSpeedRight);
  
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
  right->setState(0,0);
  left->setState(0,0);
  digitalWrite(right->enablePin, LOW);
  digitalWrite(left->enablePin, LOW);
}

void MovementController::accel(int startPow, int endPow, int time){}

#endif
