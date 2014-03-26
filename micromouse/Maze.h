/***************
* Maze Class Declaration
*
* Sharang Phadke
* Sameer Chauhan
***************/

#ifndef MAZE_H
#define MAZE_H

#include "Arduino.h"
#include "const.h"
#include <StandardCplusplus.h>
#include <vector>
#include "SensorController.h"

using namespace std;
namespace Maze{
  class Cell {
    public:
        Cell();
        Cell(int x, int y);
        int x;
        int y;
};


  extern Cell nodes[MAZE_SIZE][MAZE_SIZE];
  extern int distanceValue[MAZE_SIZE][MAZE_SIZE];
  extern int walls[MAZE_SIZE][MAZE_SIZE];
  extern int curDir;
  extern Cell curPos;


  // Maze(); //initialize maze to have no walls except border
  void addWalls(int row, int col, int direction);
  void removeWalls(int row, int col, int direction);
  void initializeWalls();
  void initializeGraph();
  vector<Cell*> getNeighbors(Cell* cell);
  void recursiveFlood(vector<Cell *> currentLevel, int level);
  void floodGraph();
  void createTest();
  void setupTest();

  void peek(); //Look ahead and add walls if necessary
  int decide(); //decide where to go next
  boolean fullyExplored();
}

#endif
