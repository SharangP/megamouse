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
// Maze::Cell Maze::nodes[MAZE_SIZE][MAZE_SIZE];
int Maze::distanceValue[MAZE_SIZE][MAZE_SIZE] = {{0}};
int Maze::walls[MAZE_SIZE][MAZE_SIZE] ={{0}};


Maze::Cell::Cell(int x, int y){
  this->x = x;
  this->y = y;
}

Maze::Cell::Cell(){}


//figure out what walls the next square has
void Maze::detectWalls(){
  //TODO: Don't check for walls outside of border
  SensorController::printSensors();
  Cell newPos = nextPos();
  int wallLoc = 0;
  if( !( walls[curPos.x][curPos.y] & curDir ) ){
    if(SensorController::irSmooth[LEFT] > -SensorController::sensorSigma[LEFT]){
      Serial.println("Left sensor found the right wall");
      wallLoc = ROTATE(curDir,1);
      addWalls(newPos.x, newPos.y, wallLoc);
    }
    if(SensorController::irSmooth[RIGHT] > -SensorController::sensorSigma[RIGHT]){
      Serial.println("Rigth sensor found the left wall");
      wallLoc = ROTATE(curDir,3);
      addWalls(newPos.x, newPos.y, wallLoc);
    }
    //WE NEVER NORMALIZE THE CENTER
    if(SensorController::irSmooth[CENTER] > CENTERTHRESH){
      Serial.println("Found Center Wall");
      wallLoc = curDir;
      addWalls(newPos.x, newPos.y, wallLoc);
    }
  }
}


int Maze::decide(){
  vector<Cell*> neighbors = getNeighbors(&curPos);
  Cell* nextMove = &curPos;

  // Serial.println(neighbors.size());

  while (!neighbors.empty()){
    Cell *tmpCell = neighbors.back();
    // Serial.println(distanceValue[tmpCell->x][tmpCell->y]);
    // Serial.println(distanceValue[nextMove->x][nextMove->y]);
    if (distanceValue[tmpCell->x][tmpCell->y] < distanceValue[nextMove->x][nextMove->y]){
      nextMove = tmpCell;
    }
    neighbors.pop_back();
  }

  if (nextMove == &curPos){
    return 0;
  }

  int dir;
  int offsetX = curPos.x - nextMove->x;
  int offsetY = curPos.y - nextMove->y;

  if (offsetX == 1)
    return NORTH;
  else if( offsetX == -1)
    return SOUTH;

  if(offsetY == 1)
    return EAST;
  else if (offsetY == -1)
    return WEST;
}

//check whether the maze has been fully explored
boolean Maze::fullyExplored(){
  return false;
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
      offsetY = -1;
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

  // Serial.print("Next Walls: ");
  // Serial.println(nextWalls);

  int leftWall = !!(ROTATE(curDir, 3) & nextWalls);
  int rightWall = !!(ROTATE(curDir, 1) & nextWalls);

  int nWalls = rightWall + (leftWall << 1);
  
  // Serial.println(nWalls);
  return nWalls;
}

/*Adds wall at (row,col) in direction*/
void Maze::addWalls(int row, int col, int direction){
  //TODO: SAMEER CHECK THIS SHIT
    // walls[row][col] += direction;
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
 void Maze::initializeWalls(){
    for (int j = 0; j < MAZE_SIZE; j++) {
        walls[0][j] = walls[0][j] | NORTH;
        walls[j][0] = walls[j][0] | WEST;
        walls[MAZE_SIZE-1][MAZE_SIZE-1-j] = walls[MAZE_SIZE-1][MAZE_SIZE-1-j] | SOUTH;
        walls[MAZE_SIZE-1-j][MAZE_SIZE-1] = walls[MAZE_SIZE-1-j][MAZE_SIZE-1] | EAST;
    }
    //we also know that we start with 3 sides
    addWalls(MAZE_SIZE-1,0,EAST);
}


/*Fills node list with tuples (Probably not needed)*/
// void Maze::initializeGraph(){
//     for (int i = 0; i<MAZE_SIZE; i++){
//         for (int j =0 ; j<MAZE_SIZE; j++){
//              nodes[i][j] = Cell(i, j);
//         }
//     }
// }


/* Returns vector of neighbors not blocked by walls*/
vector<Maze::Cell*> Maze::getNeighbors(Maze::Cell* cell){
  vector<Cell*> neighbors;
  int row = cell->x;
  int col = cell->y;
  if ( walls[row][col] == 0 ){
    // neighbors.push_back(&nodes[row][col-1]);
    // neighbors.push_back(&nodes[row][col+1]);
    // neighbors.push_back(&nodes[row+1][col]);
    // neighbors.push_back(&nodes[row-1][col]);
    neighbors.push_back( new Cell(row, col-1));
    neighbors.push_back( new Cell(row, col+1));
    neighbors.push_back( new Cell(row+1, col));
    neighbors.push_back( new Cell(row-1, col));
    return neighbors;
  }

  if( (walls[row][col] & NORTH) == 0){
    // neighbors.push_back(&nodes[row-1][col]);
    neighbors.push_back( new Cell(row-1, col));
  }
  if( (walls[row][col] & SOUTH )== 0){
    // neighbors.push_back(&nodes[row+1][col]);
    neighbors.push_back( new Cell(row+1, col));

  }
  if( (walls[row][col] & EAST )== 0){
    // neighbors.push_back(&nodes[row][col+1]);
        neighbors.push_back( new Cell(row, col+1));

  }
  if( (walls[row][col] & WEST )== 0){
    // neighbors.push_back(&nodes[row][col-1]);
    neighbors.push_back( new Cell(row, col-1));
  }

  return neighbors;
}


/* updates distanceValue using floodFill*/
void Maze::recursiveFlood(vector<Maze::Cell *> currentLevel, int level){


  vector<Cell*> nextLevel;

  while(!currentLevel.empty()){
    Cell* tmp = currentLevel.back();

    if (distanceValue[tmp->x][tmp->y] == 255){

      distanceValue[tmp->x][tmp->y] = level;
      // Find all neighbors not blocked by walls and put into Next Level

      vector<Cell*> neighbors = getNeighbors(tmp);
      while (!neighbors.empty()){
        nextLevel.push_back(neighbors.back());
        neighbors.pop_back();
      }
    }
    currentLevel.pop_back();

  }

  if(!nextLevel.empty()){
    level++;
    currentLevel = nextLevel;
    recursiveFlood(currentLevel, level);
  }
  else{
    return;
  }
}

/*Flood fill initial setup*/
void Maze::floodGraph(){
    for(int i = 0; i < MAZE_SIZE; i++){
        for (int j = 0; j <MAZE_SIZE; j++){
            distanceValue[i][j] = 255;
        }
    }
    vector<Cell*> currentLevel;

    // currentLevel.push_back(&nodes[2][2]);
    currentLevel.push_back( new Cell(4,4));
    // currentLevel.push_back( new Cell(7,7));
    // currentLevel.push_back( new Cell(7,8));
    // currentLevel.push_back( new Cell(8,8));
    // currentLevel.push_back( new Cell(8,7));

    // currentLevel.push_back(&nodes[7][7]);
    // currentLevel.push_back(&nodes[7][8]);
    // currentLevel.push_back(&nodes[8][8]);
    // currentLevel.push_back(&nodes[8][7]);
    int level = 0;

    // Serial.print("Calling recursive.. with queue of size: ");
    // Serial.println(currentLevel.count());

    recursiveFlood(currentLevel, level);
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
      else
        Serial.print(" ");
      if ( (walls[i][j] & SOUTH ) != 0 )
        Serial.print("_");
      else
        Serial.print(" ");
    }
    Serial.print("|");
    Serial.println("");
  }
  Serial.println("");
}


void Maze::initialize(){
  initializeWalls();
  // initializeGraph();
}


void Maze::setupTest(){

    // Serial.print("Initializing Walls...");
    initializeWalls();
    // Serial.print("Init Graph...");
    // initializeGraph();
    // Serial.print("Create test...");
    // createTest();
    // Serial.println("Flooding...");
    floodGraph();
    // Serial.println("Flooded...");
    showWalls();
    printWalls();
    printDistance();


    // QueueArray<Cell*> n = getNeighbors(new Cell(1,1));
    // Serial.println(n.size());

    Serial.println("");
    Serial.println("");

}

#endif
