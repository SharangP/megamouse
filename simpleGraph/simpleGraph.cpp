/**
 * Simple Graph implementation
 */

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#define NORTH 1
#define SOUTH 2
#define WEST  4
#define EAST  8
#define MAZE_SIZE 16

using namespace std;

class Cell {
	public:
		Cell();
		Cell(int x, int y);
		int value;
		int x;
		int y;
};


Cell::Cell(int x, int y){
	this->x = x;
	this->y = y;
}

Cell::Cell(){}

Cell nodes[MAZE_SIZE][MAZE_SIZE];
int distanceValue[MAZE_SIZE][MAZE_SIZE] = {{0}};
int walls[MAZE_SIZE][MAZE_SIZE];


/* Prints the distance from cell to center */
void printDistance(){

	for (int i = 0; i<MAZE_SIZE; i++ ) {
		for (int j = 0; j<MAZE_SIZE; j++){
			cout.width(3);
			cout<<distanceValue[i][j]<<"  ";
		}
		cout<<endl;
	}
	cout<<endl;
}

/* Visual Representation of maze*/
void showWalls(){
	for ( int m = 0; m < MAZE_SIZE; m++){
			cout<< " _";
		}
	cout<<endl;
	for (int i = 0; i<MAZE_SIZE; i++ ) {
		for (int j = 0; j<MAZE_SIZE; j++){
			if ( (walls[i][j] & WEST ) != 0 )
				cout<<"|";
			else
				cout<<" ";
			if ( (walls[i][j] & SOUTH ) != 0 ) 
				cout<<"_";
			else
				cout<<" ";
			// if ( (walls[i][j] & EAST) != 0)
			// 	cout<<"|";
			// else
			// 	cout<<" ";
		}
		cout<<"|"<<endl;
	}
	cout<<endl;
}

/* Displays the numeric representation of maze*/
void printWalls(){
	for (int i = 0; i<MAZE_SIZE; i++ ) {
		for (int j = 0; j<MAZE_SIZE; j++){
			cout.width(2);
			cout<<walls[i][j]<<"  ";
		}
		cout<<endl;
	}
	cout<<endl;
}



/*Adds wall at (row,col) in direction*/
void addWalls(int row, int col, int direction){

	if ( (walls[row][col] & direction ) !=0) {
		return;
	}

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
void removeWalls(int row, int col, int direction){
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
void initializeWalls(){
	for (int j = 0; j < MAZE_SIZE; j++) {
		walls[0][j] += NORTH;
		walls[j][0] += WEST;
		walls[MAZE_SIZE-1][MAZE_SIZE-1-j]+= SOUTH;
		walls[MAZE_SIZE-1-j][MAZE_SIZE-1] +=EAST;
	}
	
	// for(int i = 0; i < MAZE_SIZE; i++){
	// 	for (int j = 0; j <MAZE_SIZE; j++){
	// 		walls[i][j] = MAZE_SIZE-1;
	// 	}
	// }
	addWalls(7,7, NORTH);
	addWalls(7,7, WEST);
	addWalls(8,7, WEST);
	addWalls(8,7, SOUTH);
	addWalls(8,8,SOUTH);
	addWalls(8,8,EAST);
	addWalls(7,8, EAST);



}

/*Fills node list with tuples (Probably not needed)*/
void initializeGraph(){
	for (int i = 0; i<MAZE_SIZE; i++){
		for (int j =0 ; j<MAZE_SIZE; j++){
			 nodes[i][j] = Cell(i, j);
		}
	}
}


/* Returns vector of neighbors not blocked by walls*/
vector<Cell*> getNeighbors(Cell* cell){
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
void recursiveFlood(vector<Cell *> currentLevel, int level){


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
void floodGraph(){
	for(int i = 0; i < MAZE_SIZE; i++){
		for (int j = 0; j <MAZE_SIZE; j++){
			distanceValue[i][j] = 255;
		}
	}
	vector<Cell*> currentLevel;

	currentLevel.push_back(&nodes[7][7]);
	currentLevel.push_back(&nodes[7][8]);
	currentLevel.push_back(&nodes[8][8]);
	currentLevel.push_back(&nodes[8][7]);
	int level = 0;

	recursiveFlood(currentLevel, level);
}

bool isBorder(int direction){
	switch(direction){
		case NORTH:
			break;
		case SOUTH:
			break;
		case EAST:
			break;
		case WEST:
			break;
	}
	return false;
}

/*randomly generates a maze*/
void genWalls(){
	// vector<Cell*> drillers;
	// drillers.push_back(&nodes[7][7]);

	// while(!drillers.empty()){
	// 	int dir = pow(2, rand() % 4 );
	// 	Cell *drill = drillers.back();
	// 	drillers.pop_back();	

	// 	if (drill->x == 0 || drill->x == MAZE_SIZE-1 || drill->y == 0 || drill->y == MAZE_SIZE-1){
	// 		continue;
	// 	}
	// 	if( walls[drill->x][drill->y] != 0){
	// 		removeWalls(drill->x, drill->y,dir);
	// 	}
	// 	switch(dir){
	// 		case NORTH:
	// 			drillers.push_back(&nodes[drill->x-1][drill->y]);
	// 			break;
	// 		case SOUTH:
	// 			drillers.push_back(&nodes[drill->x+1][drill->y]);
	// 			break;
	// 		case EAST:
	// 			drillers.push_back(&nodes[drill->x][drill->y+1]);
	// 			break;
	// 		case WEST:
	// 			drillers.push_back(&nodes[drill->x][drill->y-1]);
	// 			break;
	// 	}
	// }
	for (int n =0; n<MAZE_SIZE; n++){
		for(int m = 0; m <MAZE_SIZE; m++){
			for(int t = 0; t <rand() % 2 +1; t++){
				int wallDir = pow(2, (rand() % 4)) * ( (rand() % 100)>40	 ); // Pow exists in arduino
				if ( (walls[n][m] & wallDir ) == 0 ){
					addWalls(n,m,wallDir);
				} 
			}
		}
	}

	removeWalls(7,7, EAST);
	removeWalls(7,7, SOUTH);
	removeWalls(7,8, NORTH);
	removeWalls(7,8, SOUTH);
	removeWalls(8,8, WEST);

}

int main(){
	initializeWalls();
	initializeGraph();
	genWalls();

	floodGraph();
	printDistance();
	printWalls();
	showWalls();
}

