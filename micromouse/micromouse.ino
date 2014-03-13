/***************
*  Maze Solving Micromouse
*  
*  Sharang Phadke
*  Sameer Chauhan
***************/

#include <PID_v1.h>
#include <Encoder.h>
#include "SensorController.h"
#include "MovementController.h"
#include "Maze.h"

//maze exploring function
void exploreMaze(Maze maze){
  int state = 0;
  state = DECIDE; //go straight  
  
  while (!maze.fullyExplored()){
    SensorController::sample();
  
      //decision
        //update maze
        //find shortest path to center
        //make a move in that direction
    
    switch(state){
      case DECIDE: //Make decision, reset encoder values
        decision(&state);
          break;
      case STRAIGHT:
        MovementController::goStraight(&state); //decode output -> motors, keep track of execution of current state
        break;
      case TURN: // Turn
        break;
      case STOP: // Stop
        Serial.println("Stopped");
        break;
    }
    
    MovementController::updatePID(state);
  }
}


void decision(int* state){
  Serial.println("Deciding");
  *state = STRAIGHT;
  return;
} 

void returnToStart(Maze maze){}

void solveMaze(Maze maze){}


Maze maze;

void setup(){
  Serial.begin(9600);
  Serial.println("Micromouse Running...");
  MovementController::pid->SetMode(AUTOMATIC);
  SensorController::leftEncoder->write(1);
  SensorController::rightEncoder->write(1);
}

void loop(){
  delay(1000);  //calibrate
  exploreMaze(maze);
  returnToStart(maze);
  solveMaze(maze);
  //gg wp
  delay(100000);
}
