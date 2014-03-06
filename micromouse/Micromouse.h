/***************
* Micromouse Class Declaration
*
* Sharang Phadke
* Sameer Chauhan
***************/

#ifndef MICROMOUSE_H
#define MICROMOUSE_H

#include "Arduino.h"
#include "SensorController.h"
#include "MovementController.h"
#include "Maze.h"

class Micromouse{

public:

  Micromouse(); //initialize maze, intitialize sensors, initialize motors
  void solveMaze();

private:
  void makeMove();
  
  Maze * maze;
  SensorController * sensors;
  MovementController * movement;
};

#endif
