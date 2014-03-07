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
void SensorController::sample(){
  
  int i, j;
  float irSmoothNew[3] = {0.0};
  
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < N_IR-1; j++){
      this->irSignal[i][j] = this->irSignal[i][j+1];
      irSmoothNew[i] += ((float)(this->irSignal[i][j]))*(j+1)*2/(N_IR*(N_IR+1));
    }
  }
  
  this->irSignal[LEFT][N_IR-1] = analogRead(LEFT);
  this->irSignal[RIGHT][N_IR-1] = analogRead(RIGHT);
  this->irSignal[CENTER][N_IR-1] = analogRead(CENTER);
  
  this->irSmooth[LEFT] = irSmoothNew[LEFT];
  this->irSmooth[RIGHT] = irSmoothNew[RIGHT];
  this->irSmooth[CENTER] = irSmoothNew[CENTER];
  
}

#endif
