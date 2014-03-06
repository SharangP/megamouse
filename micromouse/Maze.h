/***************
* Maze Class Declaration
*
* Sharang Phadke
* Sameer Chauhan
***************/

#ifndef MAZE_H
#define MAZE_H

#include "Arduino.h"

class Maze{

public:

  Maze(); //initialize maze to have no walls except border
  void addWall(int x, int y, int wallPos); //add a particular wall to coordinate x,y
 
private:

  int grid[16][16]; //maze representation
  
};

#endif
