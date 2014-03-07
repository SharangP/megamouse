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


class MovementController{

public:

  MovementController(SensorController * sensors); //initialize motors and save sensors
  void goStraight();
  void goLeft();
  void goRight();
  void goBack();
  void turn(int dir);
  
private:

  void go();
  void brake();
  void accel(int startPow, int endPow, int time);

  Motor * right;
  Motor * left;
  SensorController * sensors;

};

#endif
