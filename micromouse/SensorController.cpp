/***************
* SensorController Class Implementation
*
* Sharang Phadke
* Sameer Chauhan
***************/

#ifndef SENSORCONTROLLER_CPP
#define SENSORCONTROLLER_CPP

#include "SensorController.h"


volatile int SensorController::irSignal[3][N_IR] = {{0}};
volatile double SensorController::irSmooth[3] = {0};
volatile int SensorController::irThreshold[3] = {LEFTTHRESH, RIGHTTHRESH, CENTERTHRESH};

Encoder SensorController::leftEncoder = Encoder(LEFT_ENCODER_1, LEFT_ENCODER_2);
Encoder SensorController::rightEncoder = Encoder(RIGHT_ENCODER_1, RIGHT_ENCODER_2);


void SensorController::sample(){

//  int i, j;
//  double irSmoothNew[3] = {0.0};
//  
//  for(int i = 0; i < 3; i++){
//    for(int j = 0; j < N_IR-1; j++){
//      irSignal[i][j] = irSignal[i][j+1];
//      // non-recursive linear filter implementation
//      irSmoothNew[i] += ((double)(irSignal[i][j]))*(j+1)*2/(N_IR*(N_IR+1));
//    }
//  }
//  
//  irSignal[LEFT][N_IR-1] = analogRead(LEFT_IR);
//  irSignal[RIGHT][N_IR-1] = analogRead(RIGHT_IR);
//  irSignal[CENTER][N_IR-1] = analogRead(CENTER_IR);
  
  int l = analogRead(LEFT_IR);
  int r = analogRead(RIGHT_IR);
  int c = analogRead(CENTER_IR);
  
  irSmooth[LEFT]   = ALPHA*l + (1 - ALPHA)*irSmooth[LEFT];//irSmoothNew[LEFT];
  irSmooth[RIGHT]  = ALPHA*r + (1 - ALPHA)*irSmooth[RIGHT];//irSmoothNew[RIGHT];
  irSmooth[CENTER] = ALPHA*c + (1 - ALPHA)*irSmooth[CENTER];//irSmoothNew[CENTER];
}

void SensorController::printSensors(){
  Serial.print("L: ");
  Serial.print(irSmooth[LEFT]);
  Serial.print(" R: ");
  Serial.print(irSmooth[RIGHT]);
  Serial.print(" C: ");
  Serial.println(irSmooth[CENTER]);
}

#endif
