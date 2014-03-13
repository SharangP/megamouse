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
#include <Encoder.h>

namespace SensorController {
  
  extern volatile int irSignal[3][N_IR];
  extern volatile double irSmooth[3];
  extern volatile int irThreshold[3];
  
  extern Encoder leftEncoder;
  extern Encoder rightEncoder;
  
  void sample();
  void printSensors();
  //void mlDetect(int (&walls)[3]); //detect walls using ML thresholds

}

#endif
