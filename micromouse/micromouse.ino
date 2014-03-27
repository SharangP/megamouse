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
  
  Maze::detectWalls();
  Serial.println("New Maze Layout");
  Maze::showWalls();
  // int decision = Maze::decide();

  int decision = NORTH;

  delay(500);
  Serial.println(decision);

  if(decision == Maze::curDir){
    *state = STRAIGHT;
  } else{
    *state = IDLE;
  }


  if (SensorController::irSmooth[CENTER] >= TOOCLOSE) {
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
        MovementController::goStraight();
        if (SensorController::rightEncoder.read() >= SQUARE_SIZE
              || SensorController::leftEncoder.read() >= SQUARE_SIZE
              || SensorController::irSmooth[CENTER] >= TOOCLOSE){
          state = STOP;
          Maze::incrementPos(); // Increment location on maze
        }
        break;

      case TURN: // Turn
        MovementController::turn(RIGHT);
        if ((abs(SensorController::leftEncoder.read())
            + abs(SensorController::rightEncoder.read())) >= 1500){
            //MovementController::brake();
            state= STOP;
            //TODO: DONT FORGET TO UPDATE THE curDir!. Something like Maze::curDir = ROTATE(Maze::curDir, 1, 3, or 0) based on turn
        }
        break;

      case STOP: // Stop
          // If you haven't stopped before, stop.
          if(MovementController::left->state != 0 && MovementController::right->state != 0){
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
  
  // Maze::initialize();

  // delay(3000);
  // SensorController::calibrate();
  // delay(3000);
}

void loop(){

  MovementController::brake();

  // wait till youre ready
  while(!Serial.available()){}

  SensorController::leftEncoder.write(1);
  SensorController::rightEncoder.write(1);
  Maze::initialize();

  delay(3000);
  SensorController::calibrate();
  delay(3000);

  // SensorController::sample();
  // SensorController::printSensors();
  // delay(SAMPLE_PERIOD);

  // Maze::Cell nextPos = Maze::nextPos();
  // Serial.print("Next Pos X: ");
  // Serial.print(nextPos.x);
  // Serial.print(" Y: ");
  // Serial.println(nextPos.y);

  Serial.println("Initial Maze Layout");
  Maze::showWalls();
  Maze::printWalls();

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
