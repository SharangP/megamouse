/***************
* Micromouse Class Implementation
*
* Sharang Phadke
* Sameer Chauhan
***************/

#ifndef MICROMOUSE_CPP
#define MICROMOUSE_CPP

#include "Micromouse.h"

Micromouse::Micromouse(){
  this->maze = new Maze();
  this->sensors = new SensorController();
  this->movement = new MovementController(this->sensors);
}

void Micromouse::solveMaze(){
  this->movement->goStraight();
}

#endif
