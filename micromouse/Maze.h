/***************
* Maze Class Declaration
*
* Sharang Phadke
* Sameer Chauhan
***************/

#ifndef MAZE_H
#define MAZE_H

#include "Arduino.h"
#include <EEPROM.h>
#include <StandardCplusplus.h>
#include <vector>
#include <list>
#include "SensorController.h"
#include "MovementController.h"
#include "const.h"


using namespace std;

namespace Maze{
  class Cell {
    public:
        Cell();
        Cell(int x, int y);
        int x;
        int y;
};


  extern unsigned char distanceValue[MAZE_SIZE][MAZE_SIZE];
  extern unsigned char walls[MAZE_SIZE][MAZE_SIZE/2];
  extern int curDir;
  extern Cell curPos;
  //TODO: idea: remember an x,y or big int for curPos
  // and keep track of an analog position in the maze
  // that can be converted to a grid location but also
  // be used for figuring out how far to go straight each time


  // Maze(); //initialize maze to have no walls except border
  void addWalls(int row, int col, int direction);
  void removeWalls(int row, int col, int direction);

  unsigned char getWalls(int x, int y);
  void setWalls(int x, int y, int val);

  void initialize();
  Cell nextPos();
  void incrementPos();
  void decrementPos();

  int checkWalls(boolean next);
  void detectWalls(); //Look ahead and add walls if necessary
  int decide();

  vector<Cell> getNeighbors(Cell cell);

  void floodGraph();

  boolean fullyExplored();
  void clear();
  boolean checkSolved();
  void save();
  void load();

  void createTest();
  void setupTest();

  /*debug functions*/
  void printDir();
  void printWalls();
  void printDistance();
  void showWalls();
  void printWalls();
}

#endif
