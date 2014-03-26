/***************
* Constants
*
* Sharang Phadke
* Sameer Chauhan
***************/

#ifndef CONST_H
#define CONST_H

#define LEFT     0
#define RIGHT    1
#define CENTER   2

#define LEFTTHRESH       261
#define RIGHTTHRESH      257
#define CENTERTHRESH     328
#define TOOCLOSE         600

#define LEFT_IR    3
#define CENTER_IR  4
#define RIGHT_IR   5

#define LEFT_ENCODER_1   3
#define LEFT_ENCODER_2   11
#define RIGHT_ENCODER_1  2
#define RIGHT_ENCODER_2  10

#define SAMPLE_PERIOD 32
#define ALPHA 0.5

#define DECIDE    0
#define STRAIGHT  1
#define TURN      2
#define STOP      3
#define IDLE      4

#define SQUARE_SIZE 1300 // 1200
#define SLOWEST 15

#define NORTH 1
#define EAST 2
#define SOUTH  4
#define WEST  8
#define MAZE_SIZE 5

#define ROTATE(x,y) (x << y | x >> (4-y)) & 15


#endif
