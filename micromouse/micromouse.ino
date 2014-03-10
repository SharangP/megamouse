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


//maze exploring function
//returns true if maze has been fully explored
void exploreMaze(Maze maze){
  while (!maze.fullyExplored()){
  //explore:
    //detect walls
    //update maze
    //find shortest path to center
    //make a move in that direction
    MovementController::goStraight();
  }
}

void returnToStart(Maze maze){}

void solveMaze(Maze maze){}


Maze maze;

void setup(){
  Serial.begin(9600);
  Serial.println("Micromouse Running...");
  
  MsTimer2::set(SAMPLE_PERIOD, SensorController::sample);
  MsTimer2::start();
}

void loop(){
  delay(1000);  //calibrate
  exploreMaze(maze);
  returnToStart(maze);
  solveMaze(maze);
  //gg wp
  delay(100000);
}
