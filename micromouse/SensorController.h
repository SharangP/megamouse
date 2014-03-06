/***************
* SensorController Class Declaration
*
* Sharang Phadke
* Sameer Chauhan
***************/

#ifndef SENSORCONTROLLER_H
#define SENSORCONTROLLER_H

#include "Arduino.h"

#define LEFT 0
#define RIGHT 1
#define CENTER 2

class SensorController{

public:

  SensorController(); //initialize sensors
  void detectWalls(int (&walls)[3]);
  
private:

  int encoder[2];

};

#endif
