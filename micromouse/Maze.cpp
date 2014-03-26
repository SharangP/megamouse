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


//figure out what walls the next square has
void Maze::peek(){
  // Don't check for walls outside of border
  int offsetX = 0;
  int offsetY = 0;
  int wallLoc = 0;


  if(SensorController::irSmooth[LEFT] > -4*SensorController::sensorSigma[LEFT]){

    //wallLoc = curDIR
    switch(curDir){
      case NORTH:
        wallLoc = EAST;
        offsetX = -1;
        offsetY = 0;
        break;
      case SOUTH:
        wallLoc = WEST;
        offsetX = 1;
        offsetY = 0;
        break;
      case WEST:
        wallLoc = NORTH;
        offsetX = 0;
        offsetY = -1;
        break;
      case EAST:
        wallLoc = SOUTH;
        offsetX = 0;
        offsetY = -1;
        break;
    }
    addWalls(curPos.x + offsetX, curPos.y + offsetY, wallLoc);
  }
  if(SensorController::irSmooth[RIGHT] > -4*SensorController::sensorSigma[RIGHT]){
    switch(curDir){
      case NORTH:
        wallLoc = WEST;
        offsetX = -1;
        offsetY = 0;
        break;
      case SOUTH:
        wallLoc = EAST;
        offsetX = 1;
        offsetY = 0;
        break;
      case WEST:
        wallLoc = SOUTH;
        offsetX = 0;
        offsetY = -1;
        break;
      case EAST:
        wallLoc = NORTH;
        offsetX = 0;
        offsetY = 1;
        break;
    }
    addWalls(curPos.x+ offsetX, curPos.y + offsetY, wallLoc);
  }
  if(SensorController::irSmooth[CENTER] > -4*SensorController::sensorSigma[CENTER]){
    switch(curDir){
      case NORTH:
        wallLoc = NORTH;
        offsetX = -1;
        offsetY = 0;
        break;
      case SOUTH:
        wallLoc = SOUTH;
        offsetX = 1;
        offsetY = 0;
        break;
      case WEST:
        wallLoc = WEST;
        offsetX = 0;
        offsetY = -1;
        break;
      case EAST:
        wallLoc = EAST;
        offsetX = 0;
        offsetY = 1;
        break;
    }
    addWalls(curPos.x+ offsetX, curPos.y + offsetY, wallLoc);
  }
  //look right
  //look straight
}

int Maze::decide(){
  return 0;
}

//check whether the maze has been fully explored
boolean Maze::fullyExplored(){
  return false;
}



void Maze::incrementPos(){
  int offsetY = 0;
  int offsetX = 0;
  switch(curDir){
    case NORTH:

      break;
    case SOUTH:
      break;
    case WEST:
      break;
    case EAST:
      break;
  }

}
Maze::Cell::Cell(int x, int y){
  this->x = x;
    this->y = y;
}

Maze::Cell::Cell(){}

Maze::Cell Maze::nodes[MAZE_SIZE][MAZE_SIZE];
int Maze::distanceValue[MAZE_SIZE][MAZE_SIZE] = {{0}};
int Maze::walls[MAZE_SIZE][MAZE_SIZE] ={{0}};

/*Adds wall at (row,col) in direction*/
void Maze::addWalls(int row, int col, int direction){

 5

    walls[row][col] += direction;
    switch(direction){
        case NORTH:
            walls[row-1][col] += SOUTH;
            break;
        case SOUTH:
            walls[row+1][col] += NORTH;
            break;
        case EAST:
            walls[row][col+1] += WEST;
            break;
        case WEST:
            walls[row][col-1] += EAST;
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
        walls[0][j] += NORTH;
        walls[j][0] += WEST;
        walls[MAZE_SIZE-1][MAZE_SIZE-1-j]+= SOUTH;
        walls[MAZE_SIZE-1-j][MAZE_SIZE-1] +=EAST;
    }
}


/*Fills node list with tuples (Probably not needed)*/
void Maze::initializeGraph(){
    for (int i = 0; i<MAZE_SIZE; i++){
        for (int j =0 ; j<MAZE_SIZE; j++){
             nodes[i][j] = Cell(i, j);
        }
    }
}


/* Returns vector of neighbors not blocked by walls*/
vector<Maze::Cell*> Maze::getNeighbors(Maze::Cell* cell){
    vector<Cell*> neighbors;
    int row = cell->x;
    int col = cell->y;
    if ( walls[row][col] == 0 ){
        neighbors.push_back(&nodes[row][col-1]);
        neighbors.push_back(&nodes[row][col+1]);
        neighbors.push_back(&nodes[row+1][col]);
        neighbors.push_back(&nodes[row-1][col]);
        return neighbors;
    }

    if( (walls[row][col] & NORTH) == 0){
        neighbors.push_back(&nodes[row-1][col]);
    }
    if( (walls[row][col] & SOUTH )== 0){
        neighbors.push_back(&nodes[row+1][col]);
    }
    if( (walls[row][col] & EAST )== 0){
        neighbors.push_back(&nodes[row][col+1]);
    }
    if( (walls[row][col] & WEST )== 0){
        neighbors.push_back(&nodes[row][col-1]);
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

    currentLevel.push_back(&nodes[2][2]);
    // currentLevel.push_back(&nodes[7][8]);
    // currentLevel.push_back(&nodes[8][8]);
    // currentLevel.push_back(&nodes[8][7]);
    int level = 0;

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

void Maze::setupTest(){
    initializeWalls();
    initializeGraph();
    createTest();
    floodGraph();
}





#endif
