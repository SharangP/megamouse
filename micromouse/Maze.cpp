/***************
* Maze Class Implementation
*
* Sharang Phadke
* Sameer Chauhan
***************/

#ifndef MAZE_CPP
#define MAZE_CPP

#include "Maze.h"


int Maze::curDir = NORTH;
Maze::Cell Maze::curPos = Maze::Cell(MAZE_SIZE-1, 0);
// unsigned char Maze::distanceValue[MAZE_SIZE][MAZE_SIZE] = {{0}};
unsigned char Maze::distanceValue[MAZE_SIZE][MAZE_SIZE] = {{0}};
unsigned char Maze::walls[MAZE_SIZE][MAZE_SIZE/2] ={{0}};


Maze::Cell::Cell(int x, int y){
  this->x = x;
  this->y = y;
}

Maze::Cell::Cell(){}

unsigned char Maze::getWalls(int x, int y){
  if(y % 2)
    return walls[x][y/2] & 15;
  else
    return ((walls[x][y/2] & 240) >> 4);
}

void Maze::setWalls(int x, int y, int val){
  // Serial.print("Setting wall at ");
  // Serial.print(x);
  // Serial.print(",");
  // Serial.print(y);
  // Serial.print(" to ");
  // Serial.println(val);

  if(y % 2){
    walls[x][y/2] = walls[x][y/2] | val;
  }
  else{
    walls[x][y/2] = walls[x][y/2] | (val << 4);
  }

  Serial.print("Wall at that pos is now: ");
  Serial.println(walls[x][y/2]);
}

//figure out what walls the next square has
void Maze::detectWalls(){

  SensorController::printSensors();
  Cell newPos = nextPos();
  int wallLoc = 0;

  if( !( getWalls(curPos.x,curPos.y) & curDir ) ){
    if(SensorController::irSmooth[CENTER] > CENTERTHRESH_CLOSE){
      Serial.println("Found wall right in front of me");
      wallLoc = curDir;
      addWalls(curPos.x, curPos.y, wallLoc);
      MovementController::calibrate();
    }
    else {
      if(SensorController::irSmooth[CENTER] > CENTERTHRESH_FAR){
        Serial.println("Found wall in square in front of me");
        wallLoc = curDir;
        addWalls(newPos.x, newPos.y, wallLoc);
      }
      if(SensorController::irSmooth[LEFT] > -DETECT_THRESH*SensorController::sensorSigma[LEFT]){
        Serial.println("Left sensor found the right wall");
        wallLoc = ROTATE(curDir,1);
        addWalls(newPos.x, newPos.y, wallLoc);
      }
      if(SensorController::irSmooth[RIGHT] > -DETECT_THRESH*SensorController::sensorSigma[RIGHT]){
        Serial.println("Rigth sensor found the left wall");
        wallLoc = ROTATE(curDir,3);
        addWalls(newPos.x, newPos.y, wallLoc);
      }
    }
  }
  floodGraph();
}


int Maze::decide(){
  vector<Cell> neighbors = getNeighbors(curPos);
  Cell nextMove = curPos;
  Cell tmpCell;

  for(int i = 0; i < neighbors.size(); i++){
    tmpCell = neighbors[i];
    if (distanceValue[tmpCell.x][tmpCell.y] < distanceValue[nextMove.x][nextMove.y]){
      nextMove = tmpCell;
    }
  }

  if (nextMove.x == curPos.x && nextMove.y == curPos.y){
    return IDLE;
  }

  int dir;

  if (curPos.x > nextMove.x)
    dir = NORTH;
  else if(curPos.x < nextMove.x)
    dir = SOUTH;

  if(curPos.y > nextMove.y)
    dir = WEST;
  else if (curPos.y < nextMove.y)
    dir = EAST;

  if(dir == curDir){
    MovementController::moveSpeedRight = BASE_POWER;
    MovementController::moveSpeedLeft = BASE_POWER;
    return STRAIGHT;
  }
  if((ROTATE(curDir, 1) & dir)){
    MovementController::moveSpeedRight = TURN_POWER;
    MovementController::moveSpeedLeft = TURN_POWER;
    return TURN_RIGHT;
  }
  if((ROTATE(curDir, 3) & dir)){
    MovementController::moveSpeedRight = TURN_POWER;
    MovementController::moveSpeedLeft = TURN_POWER;
    return TURN_LEFT;
  }
  if((ROTATE(curDir, 2) & dir)){
    MovementController::moveSpeedRight = TURN_POWER;
    MovementController::moveSpeedLeft = TURN_POWER;
    return TURN_AROUND;
  }

  return IDLE;
}

//check whether the maze has been fully explored
boolean Maze::fullyExplored(){
  if(curPos.x == CENTER_X && curPos.y == CENTER_Y){
    return true;
  }
  return false;
}

void Maze::clear(){
  //clear EEprom
  for (int i = 0; i < 512; i++)
    EEPROM.write(i, 0);
}

boolean Maze::checkSolved(){
  //check whether the solved bit in EEprom
  //and the solve mode switch are set
  return EEPROM.read(MAZE_SIZE*MAZE_SIZE/2);
}

void Maze::save(){
  //save the maze to EEprom
  //save whether you've solved it
  Serial.println("Saving current maze map to EEPROM!");
  for (int i = 0; i < MAZE_SIZE; i++){
    for (int j = 0; j < MAZE_SIZE/2; j++){
      EEPROM.write(MAZE_SIZE*i+j, walls[i][j]);
      delay(5);
    }
    Serial.println();
  }

  // EEPROM.write(MAZE_SIZE*MAZE_SIZE/2, fullyExplored());
  // delay(5);
  // EEPROM.write(MAZE_SIZE*MAZE_SIZE/2 + 1, (byte)curPos.x);
  // delay(5);
  // EEPROM.write(MAZE_SIZE*MAZE_SIZE/2 + 2, (byte)curPos.y);
  // delay(5);
  // EEPROM.write(MAZE_SIZE*MAZE_SIZE/2 + 3, (byte)curDir);
  // delay(5);

  // for (int i = 0; i < MAZE_SIZE; i++){
  //   for (int j = 0; j < MAZE_SIZE; j++){
  //     EEPROM.write(MAZE_SIZE*MAZE_SIZE + MAZE_SIZE*i+j % 512, (byte)distanceValue[i][j]);
  //     delay(5);
  //   }
  // }
}

void Maze::load(){
  //load maze from EEprom
  for (int i = 0; i < MAZE_SIZE; i++)
    for (int j = 0; j < MAZE_SIZE/2; j++)
      walls[i][j] = EEPROM.read(MAZE_SIZE*i+j);

  int cpx = EEPROM.read(MAZE_SIZE*MAZE_SIZE/2 + 1);
  int cpy = EEPROM.read(MAZE_SIZE*MAZE_SIZE/2 + 2);
  int cd = EEPROM.read(MAZE_SIZE*MAZE_SIZE/2 + 3);
  Serial.print("Current Pos/Dir when last idle: ");
  Serial.print(cpx);
  Serial.print(" , ");
  Serial.print(cpy);
  Serial.print(" / ");
  Serial.println(cd);

  for (int i = 0; i < MAZE_SIZE; i++){
    for (int j = 0; j < MAZE_SIZE; j++){
      distanceValue[i][j] = EEPROM.read(MAZE_SIZE*MAZE_SIZE + MAZE_SIZE*i+j);
      delay(5);
    }
  }
}

Maze::Cell Maze::nextPos(){
  int offsetY = 0;
  int offsetX = 0;
  switch(curDir){
    case NORTH:
      offsetX = -1;
      offsetY = 0;
      break;
    case SOUTH:
      offsetX = 1;
      offsetY = 0;
      break;
    case WEST:
      offsetX = 0;
      offsetY = -1;
      break;
    case EAST:
      offsetX = 0;
      offsetY = 1;
      break;
  }
  return Cell(curPos.x + offsetX,curPos.y + offsetY);
}

void Maze::incrementPos(){
  Cell newPos = nextPos();
  curPos.x = newPos.x;
  curPos.y = newPos.y;
}

void Maze::decrementPos(){
  int offsetY = 0;
  int offsetX = 0;
  switch(ROTATE(curDir,2)){
    case NORTH:
      offsetX = -1;
      offsetY = 0;
      break;
    case SOUTH:
      offsetX = 1;
      offsetY = 0;
      break;
    case WEST:
      offsetX = 0;
      offsetY = -1;
      break;
    case EAST:
      offsetX = 0;
      offsetY = 1;
      break;
  }
  curPos.x = curPos.x + offsetX;
  curPos.y = curPos.y + offsetY;
}

int Maze::checkWalls(boolean next){
  Cell newPos = nextPos();
  int nextWalls = getWalls(newPos.x,newPos.y);
  int leftWall = !!(ROTATE(curDir, 3) & nextWalls);
  int rightWall = !!(ROTATE(curDir, 1) & nextWalls);
  int rtnWall = rightWall + (leftWall << 1);
  return rtnWall;
}

/*Adds wall at (row,col) in direction*/
void Maze::addWalls(int row, int col, int direction){
  setWalls(row, col, getWalls(row, col) | direction);
  switch(direction){
    case NORTH:
      if(row-1 >= 0)
        setWalls(row-1, col, getWalls(row-1, col) | SOUTH);
      break;
    case SOUTH:
      if(row+1 < MAZE_SIZE)
        setWalls(row+1, col, getWalls(row+1, col) | NORTH);
      break;
    case EAST:
      if(col+1 < MAZE_SIZE)
        setWalls(row, col+1, getWalls(row, col+1) | WEST);
      break;
    case WEST:
      if(col-1 >= 0)
        setWalls(row, col-1, getWalls(row, col-1) | EAST);
      break;
  }
}

/*Removes wall from row, col direction*/
// void Maze::removeWalls(int row, int col, int direction){
//     if ( (getWalls(row, col) & direction ) ==0) {
//         return;
//     }
//     walls[row][col] -= direction;
//     switch(direction){
//         case NORTH:
//             walls[row-1][col] -= SOUTH;
//             break;
//         case SOUTH:
//             walls[row+1][col] -= NORTH;
//             break;
//         case EAST:
//             walls[row][col+1] -= WEST;
//             break;
//         case WEST:
//             walls[row][col-1] -= EAST;
//             break;
//         default:
//             break;
//     }
// }

/*Sets maze borders*/
 void Maze::initialize(){
    for (int j = 0; j < MAZE_SIZE; j++) {
        setWalls(0,j , getWalls(0,j) | NORTH);
        setWalls(j, 0 , getWalls(j,0) | WEST);
        setWalls(MAZE_SIZE-1, MAZE_SIZE-1-j , getWalls(MAZE_SIZE-1,MAZE_SIZE-1-j) | SOUTH);
        setWalls(MAZE_SIZE-1-j,MAZE_SIZE-1 , getWalls(MAZE_SIZE-1-j,MAZE_SIZE-1) | EAST);
    }
    //we also know that we start with 3 sides
    addWalls(MAZE_SIZE-1,0,EAST);
    showWalls();
}

/* Returns list of neighbors not blocked by walls*/
vector<Maze::Cell> Maze::getNeighbors(Maze::Cell cell){
  vector<Cell> neighbors;
  int row = cell.x;
  int col = cell.y;

  // Serial.print("getWalls of ");
  // Serial.print(row);
  // Serial.print(",");
  // Serial.print(col);
  // Serial.print(" is ");
  // Serial.println(getWalls(row,col));

  if ( getWalls(row,col) == 0 ){
    neighbors.push_back( Cell(row, col-1));
    neighbors.push_back( Cell(row, col+1));
    neighbors.push_back( Cell(row+1, col));
    neighbors.push_back( Cell(row-1, col));
    return neighbors;
  }


  if( (getWalls(row,col) & NORTH) == 0){
    neighbors.push_back( Cell(row-1, col));
  }
  if( (getWalls(row,col) & SOUTH )== 0){
    neighbors.push_back( Cell(row+1, col));

  }
  if( (getWalls(row,col) & EAST )== 0){
    neighbors.push_back( Cell(row, col+1));

  }
  if( (getWalls(row,col) & WEST )== 0){
    neighbors.push_back( Cell(row, col-1));
  }

  return neighbors;
}


/*Flood fill initial setup*/
void Maze::floodGraph(){
    for(int i = 0; i < MAZE_SIZE; i++){
        for (int j = 0; j <MAZE_SIZE; j++){
            distanceValue[i][j] = 255;
        }
    }
    list<Cell> Q;
    int dist = 0;
    Q.push_back( Cell(CENTER_X,CENTER_Y));
    distanceValue[CENTER_X][CENTER_Y] = dist;

    while(!Q.empty()){
      dist = distanceValue[Q.front().x][Q.front().y]+1;

      vector<Cell> neighbors = getNeighbors(Q.front());

      for( int i = 0 ; i < neighbors.size(); i++){
        if ( distanceValue[neighbors[i].x][neighbors[i].y] == 255 ){
          distanceValue[neighbors[i].x][neighbors[i].y] = dist;
          Q.push_back(neighbors[i]);
        }
      }

      Q.pop_front();
    }
}

void Maze::createTest(){

    addWalls(0, 0, EAST);
    addWalls(0, 2, SOUTH);
    addWalls(0, 3, SOUTH);
    addWalls(0, 4, SOUTH);

    addWalls(1, 1, SOUTH);
    addWalls(1, 2, SOUTH);
    addWalls(1, 3, SOUTH);

    addWalls(2, 0, EAST);
    addWalls(2, 2, SOUTH);
    addWalls(2, 3, SOUTH);
    addWalls(2, 4, WEST);

    addWalls(3, 0, EAST);
    addWalls(3, 2, EAST);
    addWalls(3, 4, SOUTH);

    addWalls(4, 0, EAST);
    addWalls(4, 1, EAST);
}


/* Debug Function*/

void Maze::printDir(){
  switch(curDir){
    case NORTH:
      Serial.print("^");
      break;
    case EAST:
      Serial.print(">");
      break;
    case SOUTH:
      Serial.print(",");
      break;
    case WEST:
      Serial.print("<");
      break;
  }
}

/* Prints the distance from cell to center */
void Maze::printDistance(){

  for (int i = 0; i<MAZE_SIZE; i++ ) {
    for (int j = 0; j<MAZE_SIZE; j++){
      // cout.width(3);
      Serial.print(distanceValue[i][j]);
      Serial.print(" ");
    }
    Serial.println("");
  }
  Serial.println("");
}

/* Displays the numeric representation of maze*/
void Maze::printWalls(){
  for (int i = 0; i<MAZE_SIZE; i++ ) {
    for (int j = 0; j<MAZE_SIZE; j++){
      Serial.print(getWalls(i,j));
      Serial.print(" ");
    }
    Serial.println("");
  }
  Serial.println("");
}



/* Visual Representation of maze*/
void Maze::showWalls(){
  for ( int m = 0; m < MAZE_SIZE; m++){
      Serial.print(" _");
    }
  Serial.println("");

  for (int i = 0; i<MAZE_SIZE; i++ ) {
    for (int j = 0; j<MAZE_SIZE; j++){
      if ( (getWalls(i,j) & WEST ) != 0 )
        Serial.print("|");
      else if(i == curPos.x && j == curPos.y){
        printDir();
      } else{
        Serial.print(" ");
      }

      if ( (getWalls(i,j) & SOUTH ) != 0 )
        Serial.print("_");
      else if(i == curPos.x && j == curPos.y){
        printDir();
      } else{
        Serial.print(" ");
      }
    }
    Serial.print("|");
    Serial.println("");
  }
  Serial.println("");
}


void Maze::setupTest(){

    // Serial.print("Initializing Walls...");
    initialize();
    // Serial.print("Create test...");
    createTest();
    // Serial.println("Flooding...");
    showWalls();
    //printWalls();
    floodGraph();
    // Serial.println("Flooded...");

    // showWalls();
    // printWalls();

    //printDistance();

    //Serial.println("");
    //Serial.println("");
}

#endif
