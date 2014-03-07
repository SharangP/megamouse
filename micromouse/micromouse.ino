/***************
*  Maze Solving Micromouse
*  
*  Sharang Phadke
*  Sameer Chauhan
***************/

#include "Micromouse.h"

Micromouse mouse;
  
void setup(){
  Serial.begin(9600);
  Serial.println("Micromouse Running...");
}

void loop(){
  mouse.solveMaze();
  delay(100000);
}
