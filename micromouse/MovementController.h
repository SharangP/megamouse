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
#include "Maze.h"
#include <Encoder.h>
#include <PID_v1.h>
#include <Math.h>

namespace MovementController {

  extern double moveSpeedRight;
  extern double moveSpeedLeft;
  extern double input;
  extern double output;
  extern double setpoint;

  extern Motor * right;
  extern Motor * left;

  // extern PID * pidEncoder;
  // extern PID * pidIR;

  void updatePID(int state);
  double adjustPower(int follow);

  void go(int forward, int duration, int power);
  void goStraight();
  void goBack();
  void brake(int state);
  void turn(int dir);
  void accel(int startPow, int endPow, int time);
  void straighten();
  void calibrate();
}

#endif
