/***************
* MovementController Class Implementation
*
* Sharang Phadke
* Sameer Chauhan
***************/

#ifndef MOVEMENTCONTROLLER_CPP
#define MOVEMENTCONTROLLER_CPP

#include "MovementController.h"


double MovementController::moveSpeedRight = BASE_POWER;
double MovementController::moveSpeedLeft = BASE_POWER;
// double MovementController::input = 1;
// double MovementController::output = 1;
// double MovementController::setpoint = 1;


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


double MovementController::adjustPower(int follow){
  // double mean = SensorController::sensorMean[follow];
  double sigma = SensorController::sensorSigma[follow];
  double current = SensorController::irSmooth[follow];
  return ADJUST_POWER*current/sigma;
}

void MovementController::updatePID(int state){

  double ap;

  //TODO: Use pidEncoder and pidIR values (outputs) to inform locomotion
  //change left and right powers to go straight/turn etc. as needed
  switch(state){

    case STRAIGHT:  //straight
      switch(Maze::checkWalls()){
        case 0: //no walls
          moveSpeedRight = BASE_POWER;
          moveSpeedLeft  = BASE_POWER;
          break;
        case 1: //right wall only - follow right
          ap = adjustPower(LEFT);
          moveSpeedLeft  = BASE_POWER - ap;
          moveSpeedRight = BASE_POWER + ap;
          break;
        case 2: //left wall
          //break;
          //do the same thing as both walls
          //TODO: actually use both walls!
        case 3: //both walls - follow left
          ap = adjustPower(RIGHT);
          moveSpeedLeft  = BASE_POWER + ap;
          moveSpeedRight = BASE_POWER - ap;
          break;
      }

      //ramp down speed linearly
      moveSpeedRight = (moveSpeedRight - SLOWEST)/SQUARE_SIZE * (SQUARE_SIZE - SensorController::rightEncoder.read()) + SLOWEST;
      moveSpeedLeft  = (moveSpeedLeft  - SLOWEST)/SQUARE_SIZE * (SQUARE_SIZE - SensorController::leftEncoder.read())  + SLOWEST;

      break;

    //TODO: ramp down turn
    //TODO: fine tune turning
    case TURN_RIGHT:
      // turnSum = abs(SensorController::rightEncoder.read()) + abs(SensorController::leftEncoder.read());
      // moveSpeedRight = (BASE_POWER+20 - SLOWEST)/TURN_ENCODER_THRESH * (TURN_ENCODER_THRESH-turnSum) + SLOWEST;
      // moveSpeedLeft = (BASE_POWER+20- SLOWEST)/TURN_ENCODER_THRESH * (TURN_ENCODER_THRESH-turnSum) + SLOWEST;
      // if( - SensorController::rightEncoder.read() > SensorController::leftEncoder.read()){
      //   moveSpeedRight = TURN_POWER - 5;
      //   moveSpeedLeft  = TURN_POWER + 5;
      // } else if(-SensorController::rightEncoder.read()< SensorController::leftEncoder.read()){
      //   moveSpeedRight = TURN_RIGHT + 5;
      //   moveSpeedLeft  = TURN_POWER - 5;
      // } else {
        moveSpeedRight = TURN_POWER;
        moveSpeedLeft  = TURN_POWER+15;
      // }
      break;
    
    case TURN_LEFT:
        // turnSum = abs(SensorController::rightEncoder.read()) + abs(SensorController::leftEncoder.read());
        // moveSpeedRight = (BASE_POWER+20- SLOWEST)/TURN_ENCODER_THRESH * (TURN_ENCODER_THRESH-turnSum) + SLOWEST;
        // moveSpeedLeft = (BASE_POWER+20 - SLOWEST)/TURN_ENCODER_THRESH * (TURN_ENCODER_THRESH-turnSum) + SLOWEST;
      // if( -SensorController::leftEncoder.read() >  SensorController::rightEncoder.read()){
      //   moveSpeedRight = TURN_POWER + 5;
      //   moveSpeedLeft  = TURN_POWER - 5;
      // } else if( -SensorController::leftEncoder.read() <  SensorController::rightEncoder.read()){
      //   moveSpeedRight = TURN_RIGHT - 5;
      //   moveSpeedLeft  = TURN_POWER + 5;
      // } else {
        moveSpeedRight = TURN_POWER+15;
        moveSpeedLeft  = TURN_POWER;
      // }
      break;
    case TURN_AROUND: //turning around is just two left turns
      // moveSpeedRight = TURN_POWER+10;
      // moveSpeedLeft  = TURN_POWER-10;
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
  // double leftSpeed = output*BASE_POWER;
  // double rightSpeed = BASE_POWER;

  //Serial.print("Encoder PID Output: ");
  //Serial.println(output);

  // Serial.print("Setpoint/Sensor Val/Sensor PID: ");
  // Serial.print(SensorController::setpoint);
  // Serial.print("/");
  // Serial.print(SensorController::irSmooth[RIGHT]);
  // Serial.print("/");
  // Serial.println(SensorController::output);

  // double leftSpeed = moveSpeedRight + BASE_POWERAdj;
  // double rightSpeed = BASE_POWER - BASE_POWERAdj;

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

void MovementController::go(int forward, int duration, int power = BASE_POWER){
  Serial.println("Going forward");
  right->setState(forward, power);
  left->setState(forward, power);
  delay(duration);
  right->setState(0,0);
  left->setState(0,0);
  digitalWrite(right->enablePin, LOW);
  digitalWrite(left->enablePin, LOW);
  SensorController::leftEncoder.write(0);
  SensorController::rightEncoder.write(0);
}


void MovementController::turn(int dir){
  right->setState(1*(dir==LEFT) + 2*(dir==RIGHT), moveSpeedRight);
  left->setState(2*(dir==LEFT) + 1*(dir==RIGHT), moveSpeedLeft);
}

void MovementController::brake(int state){

  if(MovementController::left->state != 0 && MovementController::right->state != 0){

    Serial.println("Stopping");
    switch(state){
      case STRAIGHT:
        right->setState(2,moveSpeedRight);
        left->setState(2,moveSpeedLeft);
        break;
      case TURN_LEFT:
        right->setState(2,moveSpeedRight);
        left->setState(1,moveSpeedLeft);
        break;
      case TURN_RIGHT:
        right->setState(1,moveSpeedRight);
        left->setState(2,moveSpeedLeft);
        break;
      case REVERSED:
        right->setState(1,moveSpeedRight);
        left->setState(1,moveSpeedLeft);
        break;
    }

    delay(BRAKE_PERIOD);
    right->setState(0,0);
    left->setState(0,0);
    digitalWrite(right->enablePin, LOW);
    digitalWrite(left->enablePin, LOW);

    SensorController::leftEncoder.write(0);
    SensorController::rightEncoder.write(0);
  }
}

void MovementController::accel(int startPow, int endPow, int time){}

void MovementController::calibrate(){
  //called when there is a wall in front?
  //move a distance away from the wall
  //s.t. you know that when you turn around you'll know exactly where you are

  //way too close -> move back
  while((SensorController::irSmooth[LEFT] < ALL_TOOCLOSE)
    && (SensorController::irSmooth[RIGHT] < ALL_TOOCLOSE)
    && (SensorController::irSmooth[CENTER] < ALL_TOOCLOSE)){
    go(2, 5, 50);
    SensorController::sample();
  }
  brake(REVERSED);

  // while(SensorController::irSmooth[CENTER] > CENTERTHRESH_TOOCLOSE){
  //   go(2, 5, 50);
  //   SensorController::sample();
  // }

  //may need to calibrate front sensor based on left and right
  //because every type of wall will be different
}


#endif
