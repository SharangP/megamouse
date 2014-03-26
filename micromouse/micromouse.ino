/***************
*  Maze Solving Micromouse
*
*  Sharang Phadke
*  Sameer Chauhan
***************/

#define ENCODER_USE_INTERRUPTS
#include <StandardCplusplus.h>
#include <PID_v1.h>
#include <Encoder.h>
#include "SensorController.h"
#include "MovementController.h"
#include "Maze.h"




void detectWalls(){
}

void decision(int * state){
  Serial.println("Deciding");
  Maze::peek();
  int decision = Maze::decide();
  delay(500);
  if (SensorController::irSmooth[CENTER] >= CENTERTHRESH) {
    *state = TURN;
  } else{
     *state = STRAIGHT;
  }

  return;
}


//maze exploring function
void exploreMaze(){
  int state = DECIDE;

  while (!Maze::fullyExplored()){
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
      if (SensorController::rightEncoder.read() >= SQUARE_SIZE || SensorController::leftEncoder.read()>=SQUARE_SIZE){
          state = STOP;
          Maze::incrementPos(); // Increment location on maze
        } else if (SensorController::irSmooth[CENTER] >= CENTERTHRESH) {
          state = STOP;
        }
        else {
          MovementController::goStraight(&state); //decode output -> motors, keep track of execution of current state
        }
        break;

      case TURN: // Turn
        MovementController::turn(RIGHT);
        if ((abs(SensorController::leftEncoder.read()) + abs(SensorController::rightEncoder.read())) >= 1300){
            //MovementController::brake();
            state= STOP;
        }
        break;
      case STOP: // Stop
          // If you haven't stopped before, stop.
          if(MovementController::left->state != 0 && MovementController::right->state != 0){

            // Serial.print("Left Encoder: ");
            // Serial.println(SensorController::leftEncoder.read()); // This is always 1...
            // Serial.print("Right Encoder: ");
            // Serial.println(SensorController::rightEncoder.read());
            // Serial.print("Left Speed: ");
            // Serial.println(MovementController::left->power);
            // Serial.print("Right Speed: ");
            // Serial.println(MovementController::right->power);
            MovementController::brake();
            SensorController::leftEncoder.write(0);
            SensorController::rightEncoder.write(0);
            delay(10);
            state = DECIDE;
            // state = TURN;
         }
       break;
       case IDLE:
         break;

    }

    // Change motor speeds depending on current state
    MovementController::updatePID(state);
    //delay(32);
  }
}

void returnToStart(){}

void solveMaze(){}


void setup(){
  Serial.begin(9600);
  Serial.println("Micromouse Running...");
  MovementController::pidEncoder->SetMode(AUTOMATIC);
  MovementController::pidIR->SetMode(AUTOMATIC);
  SensorController::leftEncoder.write(1);
  SensorController::rightEncoder.write(1);
  Maze::setupTest();
  delay(2000);
  Serial.print("Calibrating...");
  SensorController::calibrate();
  Serial.println("Done Calibrating!");
}

void loop(){

  // SensorController::sample();
  // SensorController::printSensors();
  // delay(SAMPLE_PERIOD);

  exploreMaze();
  returnToStart();
  solveMaze();
  //gg wp
  delay(100000);
}
