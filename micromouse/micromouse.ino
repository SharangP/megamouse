/***************
*  Maze Solving Micromouse
*  
*  Sharang Phadke
*  Sameer Chauhan
***************/

#define ENCODER_USE_INTERRUPTS
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
         Serial.println(SensorController::irSmooth[CENTER]);
        break;
      case TURN: // Turn
        break;
      case STOP: // Stop
          if(MovementController::left->state != 0 && MovementController::right->state != 0){

            Serial.print("Left Encoder ");
            Serial.println(SensorController::leftEncoder.read()); // This is always 1...
            Serial.print("Right Encoder ");
            Serial.println(SensorController::rightEncoder.read());
            Serial.print("Left Speed ");
            Serial.println(MovementController::left->power);
            Serial.print("Right Speed ");
            Serial.println(MovementController::right->power);
           MovementController::brake();
         }
       break;
    }
    
    MovementController::updatePID(state);
    delay(32);
  }
}


void decision(int* state){
       //   if(MovementController::leftMotor.state != 0 && MovementController::rightMotor.state != 0){
         // 
          //}
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
  SensorController::leftEncoder.write(1);
  SensorController::rightEncoder.write(1);
  delay(2000);
}

void loop(){
  delay(1000);  //calibrate
  exploreMaze(maze);
  returnToStart(maze);
  solveMaze(maze);
  //gg wp
  delay(100000);
}
