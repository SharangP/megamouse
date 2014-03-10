/**
 * Simple Graph implementation
 *  Skeleton (not compiled)
 */

#include <iostream>
#include <vector>
#define NORTH 1
#define SOUTH 2
#define WEST  4
#define EAST  8

using namespace std;

class Cell {
	public:
		int value;
		int x;
		int y;
};

Cell distVal[16][16];
int distanceValue[16][16] = {{0}};
int walls[16][16];




void printGraph(){
	for (int i = 0; i<16; i++ ) {
		for (int j = 0; j<16; j++){
			cout<<distVal[i][j].value<<"  ";
		}
		cout<<endl;
	}
	cout<<endl;
}

void printDistance(){
	for (int i = 0; i<16; i++ ) {
		for (int j = 0; j<16; j++){
			cout<<distanceValue[i][j]<<"  ";
		}
		cout<<endl;
	}
	cout<<endl;
	cout<<"DONE PRINTING"<<endl<<endl;
}

void printWalls(){
	for (int i = 0; i<16; i++ ) {
		for (int j = 0; j<16; j++){
			cout<<walls[i][j]<<"  ";
		}
		cout<<endl;
	}
	cout<<endl;
}

void initializeWalls(){
	for (int j = 0; j < 16; j++) {
		walls[0][j] += NORTH;
		walls[j][0] += WEST;
		walls[15][15-j]+= SOUTH;
		walls[15-j][15] +=EAST;
	}
}

void initializeGraph(){
	for (int i = 0; i<16; i++){
		for (int j =0 ; j<16; j++){
			 distVal[i][j] = (Cell) { 0, i, j};
		}
	}
}



vector<Cell*> getNeighbors(Cell* cell){
	vector<Cell*> neighbors;
	int row = cell->x;
	int col = cell->y;
	if ( walls[row][col] == 0 ){
		neighbors.push_back(&distVal[row][col-1]);
		neighbors.push_back(&distVal[row][col+1]);
		neighbors.push_back(&distVal[row+1][col]);
		neighbors.push_back(&distVal[row-1][col]);

	}
	if( (walls[row][col] & NORTH) == 0){
		neighbors.push_back(&distVal[row][col-1]);
	} 
	if( (walls[row][col] & SOUTH )== 0){
		neighbors.push_back(&distVal[row][col+1]);
	}
	if( (walls[row][col] & EAST )== 0){
		neighbors.push_back(&distVal[row+1][col]);
	}
	if( (walls[row][col] & WEST )== 0){
		neighbors.push_back(&distVal[row-1][col]);
	}

	return neighbors;
}



void recursiveFlood(vector<Cell *> currentLevel, int level){
	cout<<"Calling flood: #"<<level<<endl;;


	vector<Cell*> nextLevel;
	while(!currentLevel.empty()){
		Cell* tmp = currentLevel.back();
		
		cout<<"("<<tmp->x <<", " <<tmp->y<<")"<<endl;
		// tmp->x is a huge number... idk why...
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


	cout<<" "<<nextLevel.size()<<endl;
	if(!nextLevel.empty()){
		level++;
		currentLevel = nextLevel;
		recursiveFlood(currentLevel, level);
	}
	else{
		return;
	}
}


void floodGraph(){
	for(int i = 0; i < 16; i++){
		for (int j = 0; j <16; j++){
			distanceValue[i][j] = 255;
		}
	}
	vector<Cell*> currentLevel;


	currentLevel.push_back(&distVal[7][7]);
	currentLevel.push_back(&distVal[7][8]);
	currentLevel.push_back(&distVal[8][8]);
	currentLevel.push_back(&distVal[8][7]);
	int level = 0;

	recursiveFlood(currentLevel, level);

}



int main(){
	cout<<"Hello Penis\n";
	initializeWalls();
	initializeGraph();
	printWalls();
	printGraph();

	floodGraph();
	printDistance();
}

