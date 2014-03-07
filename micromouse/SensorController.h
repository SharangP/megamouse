/***************
* SensorController Class Declaration
*
* Sharang Phadke
* Sameer Chauhan
***************/

#ifndef SENSORCONTROLLER_H
#define SENSORCONTROLLER_H

#include <Scheduler.h>
#include "Arduino.h"
#include "const.h"

class SensorController{

public:

  SensorController(Scheduler * scheduler); //initialize sensors
  void sample();
  //void mlDetect(int (&walls)[3]); //detect walls using ML thresholds

  int irSignal[3][N_IR];
  int irSmooth[3];
  
private:

  int encoder[2];
  int irThreshold[3];
  Scheduler * scheduler;
};

#endif
