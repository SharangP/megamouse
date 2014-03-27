/***************
* MovementController Class Implementation
*
* Sharang Phadke
* Sameer Chauhan
***************/

#ifndef MOVEMENTCONTROLLER_CPP
#define MOVEMENTCONTROLLER_CPP

#include "MovementController.h"


double MovementController::movementSpeed = 80;
double MovementController::moveSpeedRight = MovementController::movementSpeed;
double MovementController::moveSpeedLeft = MovementController::movementSpeed;
double MovementController::movementSpeedAdj = 0;
double MovementController::input = 1;
double MovementController::output = 1;
double MovementController::setpoint = 1;


Motor * MovementController::right = new Motor(7, 8, 9);
Motor * MovementController::left  = new Motor(4, 5, 6);

// PID * MovementController::pidEncoder = new PID(&MovementController::input,
//                                         &MovementController::output,
//                                         &MovementController::setpoint,
//                                         2, 3, 1, DIRECT);

// PID * MovementController::pidIR = new PID(&SensorController::input,
//                                         &SensorController::output,
//                                         &SensorController::setpoint,
//                                         10, 10, 5, DIRECT);


void MovementController::updatePID(int state){

  //TODO: Use pidEncoder and pidIR values (outputs) to inform locomotion
  //change left and right powers to go straight/turn etc. as needed

  switch(state){

    case STRAIGHT:  //straight

      //TODO: TEST WALL FOLLOWING AGAIN

      switch(Maze::checkWalls()){
        case 0: //no walls
          moveSpeedRight = movementSpeed;
          moveSpeedLeft  = movementSpeed;
          break;
        case 1: //right wall only - follow right
          if (SensorController::irSmooth[LEFT]
              < -ADJUST_THRESH*SensorController::sensorSigma[LEFT]) {
            moveSpeedLeft  = movementSpeed + ADJUST_POWER;// 1*SensorController::irSmooth[RIGHT];
            moveSpeedRight = movementSpeed - ADJUST_POWER;//1*SensorController::irSmooth[RIGHT];
          } else if (SensorController::irSmooth[LEFT]
              > ADJUST_THRESH*SensorController::sensorSigma[LEFT]) {
            moveSpeedLeft  = movementSpeed - ADJUST_POWER;// 1*SensorController::irSmooth[RIGHT];
            moveSpeedRight = movementSpeed + ADJUST_POWER;//1*SensorController::irSmooth[RIGHT];
          } else {
            moveSpeedRight = movementSpeed;
            moveSpeedLeft  = movementSpeed;
          }
          break;
        case 2: //left wall
          //break;
          //do the same thing as both walls
        case 3: //both walls - follow left
          if (SensorController::irSmooth[RIGHT]
              < -ADJUST_THRESH*SensorController::sensorSigma[RIGHT]) {
            moveSpeedLeft  = movementSpeed - ADJUST_POWER;// 1*SensorController::irSmooth[RIGHT];
            moveSpeedRight = movementSpeed + ADJUST_POWER;//1*SensorController::irSmooth[RIGHT];
          } else if (SensorController::irSmooth[RIGHT]
            > ADJUST_THRESH*SensorController::sensorSigma[RIGHT]) {
            moveSpeedLeft  = movementSpeed + ADJUST_POWER;// 1*SensorController::irSmooth[RIGHT];
            moveSpeedRight = movementSpeed - ADJUST_POWER;//1*SensorController::irSmooth[RIGHT];
          } else {
            moveSpeedRight = movementSpeed;
            moveSpeedLeft  = movementSpeed;
          }
          break;
      }

      moveSpeedRight = (moveSpeedRight - SLOWEST)/SQUARE_SIZE * (SQUARE_SIZE - SensorController::rightEncoder.read()) + SLOWEST;
      moveSpeedLeft  = (moveSpeedLeft  - SLOWEST)/SQUARE_SIZE * (SQUARE_SIZE - SensorController::leftEncoder.read())  + SLOWEST;

      break;

    case TURN_RIGHT:
      break;
    case TURN_LEFT:
      break;
    default:
      //?
      break;
  }
  // pidEncoder->Compute();
  // pidIR->Compute();
}

void MovementController::goStraight(){
  // decode the output
  // Must take into account of IR sensor values.
  // double leftSpeed = output*movementSpeed;
  // double rightSpeed = movementSpeed;

  //TODO: see what the output looks like!!
  //Serial.print("Encoder PID Output: ");
  //Serial.println(output);

  // Serial.print("Setpoint/Sensor Val/Sensor PID: ");
  // Serial.print(SensorController::setpoint);
  // Serial.print("/");
  // Serial.print(SensorController::irSmooth[RIGHT]);
  // Serial.print("/");
  // Serial.println(SensorController::output);

  // double leftSpeed = moveSpeedRight + movementSpeedAdj;
  // double rightSpeed = movementSpeed - movementSpeedAdj;

  // Serial.print("Motor Speed left/right: ");
  // Serial.print(moveSpeedLeft);
  // Serial.print(" / ");
  // Serial.println(moveSpeedRight);
  // Serial.println("");

  left->setState(1, moveSpeedLeft);
  right->setState(1, moveSpeedRight);
}

void MovementController::goBack(){
  Serial.println("Going back...");
  right->setState(2,100);
  left->setState(2,100);
}

void MovementController::turn(int dir){
  right->setState(1*(dir==LEFT) + 2*(dir==RIGHT),75);
  left->setState(2*(dir==LEFT) + 1*(dir==RIGHT),75);
}

void MovementController::brake(){
  Serial.println("Stopping");
  right->setState(2,moveSpeedRight);
  left->setState(2,moveSpeedLeft);
  delay(50);
  right->setState(0,0);
  left->setState(0,0);
  digitalWrite(right->enablePin, LOW);
  digitalWrite(left->enablePin, LOW);
}

void MovementController::accel(int startPow, int endPow, int time){}

//TODO: Write this function
void MovementController::calibrate(){
  //called when there is a wall in front?
  //move a distance away from the wall
  //s.t. you know that when you turn around you'll know exactly where you are
  
  //may need to calibrate front sensor based on left and right
  //because every type of wall will be different
}


#endif
