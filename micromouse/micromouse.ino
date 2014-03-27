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



void decision(int * state){
  Serial.println("Deciding");

  //TODO: test wall detection by looking @ showWalls as mouse explores
  Maze::detectWalls();
  // Serial.println("New Maze Layout");
  // Maze::showWalls();
  // *state = Maze::decide();

  *state = STRAIGHT;

  delay(500);
  Serial.print("Current decision: ");
  Serial.println(*state);

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
        MovementController::goStraight();
        if (SensorController::rightEncoder.read() >= SQUARE_SIZE
              || SensorController::leftEncoder.read() >= SQUARE_SIZE
              || SensorController::irSmooth[CENTER] >= TOOCLOSE){
          state = STOP;
          Maze::incrementPos(); // Increment location on maze
        }
        break;

      case TURN_RIGHT: // Turn
        MovementController::turn(RIGHT);
        if ((abs(SensorController::leftEncoder.read())
            + abs(SensorController::rightEncoder.read()))
            >= TURN_ENCODER_THRESH){
          state= STOP;
          Maze::curDir = ROTATE(Maze::curDir, 1);
        }
        break;

      case TURN_LEFT: // Turn LEFT
        MovementController::turn(LEFT);
        if ((abs(SensorController::leftEncoder.read())
            + abs(SensorController::rightEncoder.read()))
            >= TURN_ENCODER_THRESH){
          state= STOP;
          Maze::curDir = ROTATE(Maze::curDir, 3);
        }
        break;

      case TURN_AROUND: // Turn Around
        MovementController::turn(LEFT);
        if ((abs(SensorController::leftEncoder.read())
            + abs(SensorController::rightEncoder.read()))
            >= 2*TURN_ENCODER_THRESH){
          state= STOP;
          Maze::curDir = ROTATE(Maze::curDir, 2);
        }
        break;

      case STOP: // Stop
          // If you haven't stopped already, stop.
          if(MovementController::left->state != 0 && MovementController::right->state != 0){
            MovementController::brake();
            SensorController::leftEncoder.write(0);
            SensorController::rightEncoder.write(0);
            delay(10);
            state = DECIDE;
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

  Maze::initialize();
  Serial.println("Maze initialized..");

  delay(3000);
  SensorController::calibrate();
  delay(3000);
}

void loop(){

  //MovementController::brake();

  // wait till youre ready
  //while(!Serial.available()){}

  //SensorController::leftEncoder.write(0);
  //SensorController::rightEncoder.write(0);
   // Maze::initialize();

  // Maze::detectWalls();
  // Maze::checkWalls();
  // Maze::incrementPos();

  exploreMaze();
  //returnToStart();
  //solveMaze();
  //gg wp

  Serial.println("Waiting...");
  delay(3000);
}
