/***************
* SensorController Class Implementation
*
* Sharang Phadke
* Sameer Chauhan
***************/

#ifndef SENSORCONTROLLER_CPP
#define SENSORCONTROLLER_CPP

#include "SensorController.h"


//initialize sensors
SensorController::SensorController(){
}

//attempt to detect walls
void SensorController::detectWalls(int (&walls)[3]){
  walls[LEFT] = analogRead(LEFT);
  walls[RIGHT] = analogRead(RIGHT);
  walls[CENTER] = analogRead(CENTER);
}

#endif
