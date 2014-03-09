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


namespace MovementController{
  extern Motor * right;
  extern Motor * left;
  
  void goStraight();
  void goLeft();
  void goRight();
  void goBack();
  void go();
  void brake();
  void turn(int dir);
  void accel(int startPow, int endPow, int time);
}

#endif
