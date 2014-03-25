/***************
* Maze Class Declaration
*
* Sharang Phadke
* Sameer Chauhan
***************/

#ifndef MAZE_H
#define MAZE_H

#include "Arduino.h"


namespace Maze{
  extern int grid[16][16]; //maze representation

 // Maze(); //initialize maze to have no walls except border
  void addWall(int x, int y, int wallPos); //add a particular wall to coordinate x,y
  int nextSquare(); //figure out what walls the next square has
  boolean fullyExplored();
}

#endif
