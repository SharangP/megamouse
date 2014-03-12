void pollSensors(){
 // Updates Sensor Object
}



void updateWalls(){
	if( sensorsIndicateWall() && wallNotExist() ){
		addWallToLoc();
	}
	//Find next less step... Make the move
	return nextMove();
}

void loop(){
	pollSensors();
	int nextMove = updateWalls();


	switch(nextMove){
		case STRAIGHT:
			//adjust for shit
			goStraight();
			break;
		case TURN:
			//Turn until you are done. 
			break;
	}

}

void goStraight(){
	while (notMovedOneSquare() ){
		pollSensors();
		switch( getState() ){
			case ADJLEFT:
				break;
			case ADJRIGHT:
				break;
		}
	}
}

void turn(){
	int dir = whichWaytoTurn();
	while(notFullyTurned(dir)){
		pollSensors();
		keepTurning();
		switch(getState()){

		}
	}
}