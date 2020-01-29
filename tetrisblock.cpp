#include "tetrisblock.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h> 
#include <iterator>
#include <algorithm> 

using namespace std;

#define COLS 10
#define ROWS 20
#define SCALE 35

TetrisBlock::TetrisBlock(){

}
void TetrisBlock::init(){

	srand (time(NULL));
	int shapeChance = rand()%6+1;
	int randomRotates = rand()%12;
	int m = 0;
	int i = 0;

	//Each if is a random check to create a random drop
	//Within it i check if the 0,0 is at he edge and some pieces
	//have to be moved
	if(shapeChance == 1){
		copyToInstance(0);
		while(m < randomRotates){
			
			rotateTick();
			m++;
		}

	}
	else if(shapeChance == 2){
		copyToInstance(1);

		while(m < randomRotates){
			
			rotateTick();
			m++;
		}
	}
	else if(shapeChance == 3){
		copyToInstance(2);

		while(m < randomRotates){
			
			rotateTick();
			m++;
		}
	}
	else if(shapeChance == 4){
		copyToInstance(3);
		while(m < randomRotates){
			
			rotateTick();
			m++;
		}
	}
	else if(shapeChance == 5){
		copyToInstance(4);
		while(m < randomRotates){
			
			rotateTick();
			m++;
		}
	}
	else if(shapeChance == 6){
		copyToInstance(5);
		while(m < randomRotates){
			
			rotateTick();
			m++;
		}
	}
}
void TetrisBlock::copyToInstance(int instance){
	for(int i = 0; i < 4; i++){
		TetrisInstance[i] = TILE_SHAPES[instance][i];
		printf("%u,%u\n", TetrisInstance[i].x, TetrisInstance[i].y);
	}
}
void TetrisBlock::AddLocation(int x, int y){
	TetrisLocation = vec2(x,y);
}
void TetrisBlock::rotateTick(){
	vec2 temp[4];
	for(int i=0; i<4; i++){
		temp[i]=vec2(-TetrisInstance[i].y,TetrisInstance[i].x);
	}
	for(int i=0; i<4; i++){
		TetrisInstance[i] = temp[i];
	}
}

void TetrisBlock::downTick(){
	TetrisLocation.y -= 1;
}


