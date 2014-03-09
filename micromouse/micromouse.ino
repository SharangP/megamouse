/***************
*  Maze Solving Micromouse
*  
*  Sharang Phadke
*  Sameer Chauhan
***************/

#include <MsTimer2.h>
#include "SensorController.h"
#include "MovementController.h"
#include "Maze.h"

Maze maze;

void solveMaze(){
}

void setup(){
  Serial.begin(9600);
  Serial.println("Micromouse Running...");
  
  MovementController::right = new Motor(3, 4, 5);
  MovementController::left = new Motor(6, 7, 8);
  
  MsTimer2::set(SAMPLE_PERIOD, SensorController::sample);
  MsTimer2::start();
}

void loop(){
  Serial.print("L: ");
  Serial.print(SensorController::irSmooth[LEFT]);
  Serial.print("R: ");
  Serial.print(SensorController::irSmooth[RIGHT]);
  Serial.print("C: ");
  Serial.println(SensorController::irSmooth[CENTER]);
  delay(1000);
}
