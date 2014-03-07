/***************
*  Maze Solving Micromouse
*  
*  Sharang Phadke
*  Sameer Chauhan
***************/

#include <Scheduler.h>
#include "SensorController.h"
#include "MovementController.h"
#include "Maze.h"

Scheduler scheduler;
Maze maze;
SensorController sensors = SensorController(&scheduler);
MovementController movement = MovementController(&scheduler, &sensors);

void solveMaze(){
}

void setup(){
  Serial.begin(9600);
  Serial.println("Micromouse Running...");
}

void loop(){
  scheduler.update();
  delay(100000);
}
