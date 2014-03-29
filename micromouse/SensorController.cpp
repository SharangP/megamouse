/***************
* SensorController Class Implementation
*
* Sharang Phadke
* Sameer Chauhan
***************/

#ifndef SENSORCONTROLLER_CPP
#define SENSORCONTROLLER_CPP

#include "SensorController.h"


double SensorController::irSmooth[3];
double SensorController::sensorMean[3];
double SensorController::sensorSigma[3];

double SensorController::input = 0;
double SensorController::output = 0;
double SensorController::setpoint = 0;

Encoder SensorController::leftEncoder = Encoder(LEFT_ENCODER_1, LEFT_ENCODER_2);
Encoder SensorController::rightEncoder = Encoder(RIGHT_ENCODER_1, RIGHT_ENCODER_2);


void SensorController::sample(int nSamples){
  for(int i = 0; i < nSamples; i++){
    int l = analogRead(LEFT_IR);
    int r = analogRead(RIGHT_IR);
    int c = analogRead(CENTER_IR);

    l = (l - sensorMean[LEFT])/sensorSigma[LEFT];
    r = (r - sensorMean[RIGHT])/sensorSigma[RIGHT];
    // irSmooth[CENTER] = (irSmooth[CENTER] - sensorMean[CENTER])/sensorSigma[CENTER];

    irSmooth[LEFT]   = ALPHA*l + (1 - ALPHA)*irSmooth[LEFT];
    irSmooth[RIGHT]  = ALPHA*r + (1 - ALPHA)*irSmooth[RIGHT];
    irSmooth[CENTER] = ALPHA*c + (1 - ALPHA)*irSmooth[CENTER];

    delay(SAMPLE_PERIOD);
  }
}

void SensorController::printSensors(){
  Serial.print("L: ");
  Serial.print(irSmooth[LEFT]);
  Serial.print(" R: ");
  Serial.print(irSmooth[RIGHT]);
  Serial.print(" C: ");
  Serial.println(irSmooth[CENTER]);
}

//estimate parameters of each ir sensor's distribution
void SensorController::calibrate(){
  Serial.print("Calibrating...");

  const int nSamples = 50;  // 100 samples failed
  double samples[3][nSamples];

  sensorMean[LEFT] = 0;
  sensorMean[RIGHT] = 0;
  sensorMean[CENTER] = 0;
  sensorSigma[LEFT] = 0;
  sensorSigma[RIGHT] = 0;
  sensorSigma[CENTER] = 0;


  //Serial.print("Finding mean...");
  for(int i = 0; i < nSamples; i++){
    //Serial.println(i);
    samples[LEFT][i]   = analogRead(LEFT_IR);
    samples[RIGHT][i]  = analogRead(RIGHT_IR);
    // samples[CENTER][i] = analogRead(CENTER_IR);

    sensorMean[LEFT]   += samples[LEFT][i]/nSamples;
    sensorMean[RIGHT]  += samples[RIGHT][i]/nSamples;
    // sensorMean[CENTER] += samples[CENTER][i]/nSamples;

    delay(SAMPLE_PERIOD);
  }
  for(int i = 0; i < nSamples; i++){
    sensorSigma[LEFT]   += sq(samples[LEFT][i] - sensorMean[LEFT]);
    sensorSigma[RIGHT]  += sq(samples[RIGHT][i] - sensorMean[RIGHT]);
    // sensorSigma[CENTER] += sq(samples[CENTER][i] - sensorMean[CENTER]);
  }
  // Serial.print("Final Calculation..");
  sensorSigma[LEFT]   = max(sqrt(sensorSigma[LEFT]/nSamples), BASE_IR_SIGMA);
  sensorSigma[RIGHT]  = max(sqrt(sensorSigma[RIGHT]/nSamples), BASE_IR_SIGMA);
  // sensorSigma[CENTER] = sqrt(sensorSigma[CENTER]/nSamples);

  //dont calibrate right wall if it doesnt exist
  int l = analogRead(LEFT_IR);
  delay(SAMPLE_PERIOD);
  l += analogRead(LEFT_IR);
  delay(SAMPLE_PERIOD);
  l += analogRead(LEFT_IR);
  delay(SAMPLE_PERIOD);

  if(l/3 < SIDE_WALL_THRESH){
    sensorMean[LEFT] = sensorMean[RIGHT];
    sensorSigma[LEFT] = sensorSigma[RIGHT];
  }

  Serial.println("");
  Serial.print("Sigma Right/Left: ");
  Serial.print(sensorSigma[RIGHT]);
  Serial.print(" / ");
  Serial.println(sensorSigma[LEFT]);
  Serial.print("Mean Right/Left: ");
  Serial.print(sensorMean[RIGHT]);
  Serial.print(" / ");
  Serial.println(sensorMean[LEFT]);

  Serial.println("Done Calibrating!");
}


#endif