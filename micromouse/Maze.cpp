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
unsigned char Maze::distanceValue[MAZE_SIZE][MAZE_SIZE] = {{0}};
unsigned char Maze::walls[MAZE_SIZE][MAZE_SIZE] ={{0}};


Maze::Cell::Cell(int x, int y){
  this->x = x;
  this->y = y;
}

Maze::Cell::Cell(){}


//figure out what walls the next square has
void Maze::detectWalls(){

  SensorController::printSensors();
  Cell newPos = nextPos();
  int wallLoc = 0;
  if( !( walls[curPos.x][curPos.y] & curDir ) ){
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
  // Serial.print("CurPos/CurDir: ");
  // Serial.print(curPos.x);Serial.print(", "); Serial.print(curPos.y);
  // Serial.print(" / "); Serial.println(curDir);
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
    return STRAIGHT;
  }
  if((ROTATE(curDir, 1) & dir)){
    return TURN_RIGHT;
  }
  if((ROTATE(curDir, 3) & dir)){
    return TURN_LEFT;
  }
  if((ROTATE(curDir, 2) & dir)){
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
  return EEPROM.read(MAZE_SIZE*MAZE_SIZE);
}

void Maze::save(){
  //save the maze to EEprom
  //save whether you've solved it
  Serial.println("Saving current maze map to EEPROM!");
  for (int i = 0; i < MAZE_SIZE; i++){
    for (int j = 0; j < MAZE_SIZE; j++){
      EEPROM.write(MAZE_SIZE*i+j % 512, (byte)walls[i][j]);
      delay(5);
    }
  }

  EEPROM.write(MAZE_SIZE*MAZE_SIZE, fullyExplored());
}

void Maze::load(){
  //load maze from EEprom
  for (int i = 0; i < MAZE_SIZE; i++)
    for (int j = 0; j < MAZE_SIZE; j++)
      walls[i][j] = EEPROM.read(MAZE_SIZE*i+j % 512);
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

int Maze::checkWalls(){
  Cell newPos = nextPos();
  int nextWalls = walls[newPos.x][newPos.y];
  int leftWall = !!(ROTATE(curDir, 3) & nextWalls);
  int rightWall = !!(ROTATE(curDir, 1) & nextWalls);
  int rtnWall = rightWall + (leftWall << 1);
  return rtnWall;
}

/*Adds wall at (row,col) in direction*/
void Maze::addWalls(int row, int col, int direction){
    walls[row][col] = walls[row][col] | direction;
    switch(direction){
        case NORTH:
            walls[row-1][col] = walls[row-1][col] | SOUTH;
            break;
        case SOUTH:
            walls[row+1][col] = walls[row+1][col] | NORTH;
            break;
        case EAST:
            walls[row][col+1] = walls[row][col+1] | WEST;
            break;
        case WEST:
            walls[row][col-1] = walls[row][col-1] | EAST;
            break;
        default:
            break;
    }
}

/*Removes wall from row, col direction*/
void Maze::removeWalls(int row, int col, int direction){
    if ( (walls[row][col] & direction ) ==0) {
        return;
    }
    walls[row][col] -= direction;
    switch(direction){
        case NORTH:
            walls[row-1][col] -= SOUTH;
            break;
        case SOUTH:
            walls[row+1][col] -= NORTH;
            break;
        case EAST:
            walls[row][col+1] -= WEST;
            break;
        case WEST:
            walls[row][col-1] -= EAST;
            break;
        default:
            break;
    }
}

/*Sets maze borders*/
 void Maze::initialize(){
    for (int j = 0; j < MAZE_SIZE; j++) {
        walls[0][j] = walls[0][j] | NORTH;
        walls[j][0] = walls[j][0] | WEST;
        walls[MAZE_SIZE-1][MAZE_SIZE-1-j] = walls[MAZE_SIZE-1][MAZE_SIZE-1-j] | SOUTH;
        walls[MAZE_SIZE-1-j][MAZE_SIZE-1] = walls[MAZE_SIZE-1-j][MAZE_SIZE-1] | EAST;
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
  if ( walls[row][col] == 0 ){
    neighbors.push_back( Cell(row, col-1));
    neighbors.push_back( Cell(row, col+1));
    neighbors.push_back( Cell(row+1, col));
    neighbors.push_back( Cell(row-1, col));
    return neighbors;
  }


  if( (walls[row][col] & NORTH) == 0){
    neighbors.push_back( Cell(row-1, col));
  }
  if( (walls[row][col] & SOUTH )== 0){
    neighbors.push_back( Cell(row+1, col));

  }
  if( (walls[row][col] & EAST )== 0){
    neighbors.push_back( Cell(row, col+1));

  }
  if( (walls[row][col] & WEST )== 0){
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
    Q.push_back( Cell(2,2));
    distanceValue[2][2] = dist;

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
      Serial.print(walls[i][j]);
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
      if ( (walls[i][j] & WEST ) != 0 )
        Serial.print("|");
      else if(i == curPos.x && j == curPos.y){
        printDir();
      } else{
        Serial.print(" ");
      }

      if ( (walls[i][j] & SOUTH ) != 0 )
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
