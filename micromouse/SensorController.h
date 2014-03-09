/***************
* SensorController Class Declaration
*
* Sharang Phadke
* Sameer Chauhan
***************/

#ifndef SENSORCONTROLLER_H
#define SENSORCONTROLLER_H


#include "Arduino.h"
#include "const.h"

namespace SensorController{

  extern int irSignal[3][N_IR];
  extern int irSmooth[3];
  extern int encoder[2];
  extern int irThreshold[3];
  
  void sample();
  //void mlDetect(int (&walls)[3]); //detect walls using ML thresholds

}

#endif
