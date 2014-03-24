/***************
* MovementController Class Declaration
*
* Sharang Phadke
* Sameer Chauhan
***************/

#ifndef MOVEMENTCONTROLLER_H
#define MOVEMENTCONTROLLER_H

#include "Arduino.h"
#include "const.h"
#include "SensorController.h"
#include "Motor.h"
#include <Encoder.h>
#include <PID_v1.h>


namespace MovementController {
  
  extern double movementSpeed;
  extern double movementSpeedAdj;
  extern double input;
  extern double output;
  extern double setpoint;
  
  extern Motor * right;
  extern Motor * left;
  
  extern PID * pidEncoder;
  extern PID * pidIR;
  
  void updatePID(int state);
  void goStraight(int* state);
  void goLeft();
  void goRight();
  void goBack();
  void brake();
  void turn(int dir);
  void accel(int startPow, int endPow, int time);
}

#endif
