/***************
* Maze Class Implementation
*
* Sharang Phadke
* Sameer Chauhan
***************/

#ifndef MAZE_CPP
#define MAZE_CPP

#include "Maze.h"


//int Maze::grid = {{0}};//figure out what walls the next square has

//add a particular wall to coordinate x,y
void Maze::addWall(int x, int y, int wallPos){
}

//figure out what walls the next square has
int Maze::nextSquare(){
    return 2;
}

//check whether the maze has been fully explored
boolean Maze::fullyExplored(){
  return false;
}

#endif
