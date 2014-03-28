/***************
*  Maze Solving Micromouse
*
*  Sharang Phadke
*  Sameer Chauhan
***************/

#define ENCODER_USE_INTERRUPTS
#include <StandardCplusplus.h>
#include <EEPROM.h>
#include <PID_v1.h>
#include <Encoder.h>
#include "SensorController.h"
#include "MovementController.h"
#include "Maze.h"

#define nMoves 2

int sup = 0;
int moves[nMoves] = {TURN_AROUND, TURN_AROUND};//{TURN_LEFT, TURN_RIGHT, STRAIGHT};

int solvingMode = false;


void decision(int * state){
  Serial.println("Deciding");

  // delay(SAMPLE_PERIOD);
  for(int i = 0; i < 10; i++){
    delay(200);
    Serial.print(10-i);
    Serial.print(" ");
  }

  Serial.println("");

  if(!solvingMode)
    Maze::detectWalls();

  Serial.println("New Maze Layout");
  Maze::showWalls();
  Maze::printDistance();
  *state = Maze::decide();

  // *state = moves[(sup % nMoves)];
  // sup++;
  // if (*state == STRAIGHT)
  //   *state = STRAIGHT;
  // else{
  //   *state = moves[(sup % nMoves)];
  //   sup++;
  // }

  Serial.print("Current decision: ");
  Serial.println(*state);

  return;
}


//maze exploring function
void exploreMaze(){
  int state = DECIDE;
  boolean turn_went_straight = false;

  while (!Maze::fullyExplored()){
    SensorController::sample();

    switch(state){
      case DECIDE: //Make decision, reset encoder values
        decision(&state);
        break;

      case STRAIGHT:
        MovementController::goStraight();
        if (SensorController::rightEncoder.read() >= SQUARE_SIZE
              || SensorController::leftEncoder.read() >= SQUARE_SIZE
              || SensorController::irSmooth[CENTER] >= CENTERTHRESH_TOOCLOSE){
          MovementController::brake(state);
          state = DECIDE;
          Maze::incrementPos(); // Increment location on maze
        }
        break;

      case TURN_RIGHT: // Turn
        if (!turn_went_straight){
          // MovementController::forward(100);
          turn_went_straight = true;
        }
        MovementController::turn(RIGHT);
        if ((abs(SensorController::leftEncoder.read())
            + abs(SensorController::rightEncoder.read()))
            >= TURN_ENCODER_THRESH){
          // state= STOP;
          MovementController::brake(state);
          state = DECIDE;
          Maze::curDir = ROTATE(Maze::curDir, 1);
          turn_went_straight = false;
        }
        break;

      case TURN_LEFT: // Turn LEFT
        if (!turn_went_straight){
          // MovementController::forward(100);
          turn_went_straight = true;
        }
        MovementController::turn(LEFT);
        if ((abs(SensorController::leftEncoder.read())
            + abs(SensorController::rightEncoder.read()))
            >= TURN_ENCODER_THRESH){
          MovementController::brake(state);
          state = DECIDE;
          Maze::curDir = ROTATE(Maze::curDir, 3);
          turn_went_straight = false;
        }
        break;

      case TURN_AROUND: // Turn Around

        //TODO: two options:
        //  1. add conditions to turn
        //    - encoder condition && all ir must be less than ALL_TOOCLOSE
        //  2. k turn.
        //    - back up left motor 1/4 turn
        //    - turn left
        //    - back up left motor 1/4 turn


        MovementController::turn(LEFT);
        if (((abs(SensorController::leftEncoder.read())
            + abs(SensorController::rightEncoder.read()))
            >= TURN_AROUND_ENCODER_THRESH)
            && (SensorController::irSmooth[LEFT] < ALL_TOOCLOSE)
            && (SensorController::irSmooth[RIGHT] < ALL_TOOCLOSE)
            && (SensorController::irSmooth[CENTER] < ALL_TOOCLOSE)){

          MovementController::brake(state);
          state = DECIDE;
          Maze::curDir = ROTATE(Maze::curDir, 2);
        }
        break;

      case IDLE:
        //Maze::clear();
        Maze::save();
        break;
    }

    // Change motor speeds depending on current state
    MovementController::updatePID(state);
    //delay(32);
  }
}

void returnToStart(){}


void setup(){
  Serial.begin(9600);
  Serial.println("Micromouse Running...");
  // MovementController::pidEncoder->SetMode(AUTOMATIC);
  // MovementController::pidIR->SetMode(AUTOMATIC);
  SensorController::leftEncoder.write(1);
  SensorController::rightEncoder.write(1);

  Maze::initialize();
  Serial.println("Maze initialized..");

  delay(3000);
  SensorController::calibrate();
}

void loop(){
  for(int i = 0; i < 10; i++){
    if(Serial.available()){
      // Maze::save(); //checking whether we can read back a normal maze;

      Maze::load();
      Serial.println("Printing Maze from EEProm");
      Maze::showWalls();
      solvingMode = Maze::checkSolved();
      if(!solvingMode)
        Maze::initialize();
      break;
    }
    delay(200);
  }

  exploreMaze();
  //returnToStart();
  //gg wp

  Serial.println("Waiting...");
  delay(3000);
}
