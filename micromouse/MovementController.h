/***************
* MovementController Class Declaration
*
* Sharang Phadke
* Sameer Chauhan
***************/

#ifndef MOVEMENTCONTROLLER_H
#define MOVEMENTCONTROLLER_H

#include "Arduino.h"
#include "SensorController.h"
#include "Motor.h"

#define LEFT 0
#define RIGHT 1
#define CENTER 2


class MovementController{

public:

  MovementController(SensorController * sensors); //initialize motors and save sensors
  void goStraight();
  void goLeft();
  void goRight();
  void goBack();
private:

  void go();
  void brake();
  void accel(int startPow, int endPow, int time);

  Motor * right;
  Motor * left;
  SensorController * sensors;
  int walls[3] = {0};
};

#endif
