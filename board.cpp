#include "include/Angel.h"
#include "board.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

#define COLS 10
#define ROWS 20
#define SCALE 35
#define TRIANGLE_PER_FACE 2
#define FACES_PER_CUBE 6


Board::Board(){
}
void Board::init(){

	setToEmpty();
	setUpGrid();
}
void Board::setUpGrid(){

	for (int i=0; i < ((ROWS+1)*(COLS+1))*2+(COLS+ROWS+2)*2*2; i++){
		gridColour[i] = vec4(0.0, 0.0, 0.0, 1.0);
	}

	printf("Size of grid: %d\n", ((ROWS+1)*(COLS+1))*2+(COLS+ROWS+2)*2*2);
	for(int i = 0; i < COLS; i++){
		for (int j = 0; j < ROWS; j++)
		{
			grid[j][i] = 0;	
		}
	}

	for (int i=0; i <= COLS; i++)
	{
		//Draw the Vertical Lines
		//Front Side Vertical Lines
		gridColourLocation[2*i]    				  = vec4((SCALE*(1+i)),SCALE,  SCALE/2,1);
		gridColourLocation[2*i+1]  				  = vec4((SCALE*(1+i)),SCALE*(ROWS+1), SCALE/2,1);
		//Back Side Vertical Lines
		gridColourLocation[2*i+(ROWS+COLS+2)*2]   = vec4((SCALE*(1+i)),SCALE, -SCALE/2,1);
		gridColourLocation[2*i+(ROWS+COLS+2)*2+1] = vec4((SCALE*(1+i)),SCALE*(ROWS+1),-SCALE/2,1);
	}
	for (int i = 0; i <= ROWS; i++)
	{
		//Horizontal Lines for the front face
		gridColourLocation[(COLS+1)*2 + 2*i]    				= vec4(SCALE, 		(SCALE*(1+i)), SCALE/2,1);
		gridColourLocation[(COLS+1)*2 + 2*i+1]  			    = vec4(SCALE*(COLS+1),(SCALE*(1+i)), SCALE/2,1);
		//Horizontal lines for the back face
		gridColourLocation[(COLS+1)*2 + 2*i+(ROWS+COLS+2)*2] 	= vec4(SCALE, 		(SCALE*(1+i)),-SCALE/2,1);
		gridColourLocation[(COLS+1)*2 + 2*i+(ROWS+COLS+2)*2+1]  = vec4(SCALE*(COLS+1),(SCALE*(1+i)),-SCALE/2,1);

		//Draw the lines connecting the two faces, short lines = 1 cube length
		for (int j=0; j <= COLS; j++) 
		{
			gridColourLocation[(ROWS+COLS+2)*2*2+(COLS+1)*2*i+2*j]   = vec4(SCALE*(j+1), SCALE*(i+1),  SCALE/2, 1); 
			gridColourLocation[(ROWS+COLS+2)*2*2+(COLS+1)*2*i+2*j+1] = vec4(SCALE*(j+1), SCALE*(i+1), -SCALE/2, 1); 
		}	
	}
}

void Board::setToEmpty(){


	for(int k = 0; k < COLS*ROWS*FACES_PER_CUBE*TRIANGLE_PER_FACE*3; k++){
		boardColour[k] =  vec4(0.0, 0.0, 0.0, 0.0);
	}



	printf("Size of Board: %d\n", COLS*ROWS*FACES_PER_CUBE*TRIANGLE_PER_FACE*3);
	for (int m=0; m<ROWS; m++){
		for (int n=0; n<COLS; n++)
		{
			//36 vertexs per cube creation, 8 faces, triangles per face, 3vertex
			vec4 trianglesPerCube[FACES_PER_CUBE*TRIANGLE_PER_FACE*3] = { 

			vec4(SCALE*(n+1), SCALE*(m+1),  SCALE/2, 1), vec4(SCALE*(n+1), SCALE*(m+2),  SCALE/2, 1), vec4(SCALE*(n+2), SCALE*(m+1),  SCALE/2, 1),
			vec4(SCALE*(n+1), SCALE*(m+2),  SCALE/2, 1), vec4(SCALE*(n+2), SCALE*(m+1),  SCALE/2, 1), vec4(SCALE*(n+2), SCALE*(m+2),  SCALE/2, 1),
			vec4(SCALE*(n+1), SCALE*(m+1), -SCALE/2, 1), vec4(SCALE*(n+1), SCALE*(m+2), -SCALE/2, 1), vec4(SCALE*(n+2), SCALE*(m+1), -SCALE/2, 1),
			vec4(SCALE*(n+1), SCALE*(m+2), -SCALE/2, 1), vec4(SCALE*(n+2), SCALE*(m+1), -SCALE/2, 1), vec4(SCALE*(n+2), SCALE*(m+2), -SCALE/2, 1),
			vec4(SCALE*(n+1), SCALE*(m+1),  SCALE/2, 1), vec4(SCALE*(n+1), SCALE*(m+2),  SCALE/2, 1), vec4(SCALE*(n+1), SCALE*(m+1), -SCALE/2, 1),
			vec4(SCALE*(n+1), SCALE*(m+2),  SCALE/2, 1), vec4(SCALE*(n+1), SCALE*(m+1), -SCALE/2, 1), vec4(SCALE*(n+1), SCALE*(m+2), -SCALE/2, 1),						  
			vec4(SCALE*(n+2), SCALE*(m+1),  SCALE/2, 1), vec4(SCALE*(n+2), SCALE*(m+2),  SCALE/2, 1), vec4(SCALE*(n+2), SCALE*(m+1), -SCALE/2, 1), 
			vec4(SCALE*(n+2), SCALE*(m+2),  SCALE/2, 1), vec4(SCALE*(n+2), SCALE*(m+1), -SCALE/2, 1), vec4(SCALE*(n+2), SCALE*(m+2), -SCALE/2, 1),
			vec4(SCALE*(n+1), SCALE*(m+2),  SCALE/2, 1), vec4(SCALE*(n+2), SCALE*(m+2),  SCALE/2, 1), vec4(SCALE*(n+1), SCALE*(m+2), -SCALE/2, 1), 
			vec4(SCALE*(n+2), SCALE*(m+2),  SCALE/2, 1), vec4(SCALE*(n+1), SCALE*(m+2), -SCALE/2, 1), vec4(SCALE*(n+2), SCALE*(m+2), -SCALE/2, 1),
			vec4(SCALE*(n+1), SCALE*(m+1),  SCALE/2, 1), vec4(SCALE*(n+2), SCALE*(m+1),  SCALE/2, 1), vec4(SCALE*(n+1), SCALE*(m+1), -SCALE/2, 1), 
			vec4(SCALE*(n+2), SCALE*(m+1),  SCALE/2, 1), vec4(SCALE*(n+1), SCALE*(m+1), -SCALE/2, 1), vec4(SCALE*(n+2), SCALE*(m+1), -SCALE/2, 1) 
			};

			for(int o=0;o<3*TRIANGLE_PER_FACE*FACES_PER_CUBE;o++){
				boardColourLocation[3*TRIANGLE_PER_FACE*FACES_PER_CUBE*(COLS*m+n)+o]=trianglesPerCube[o];
			}
		}
	}
}
bool Board::checkStackCollision(vec2 TetrisDrop[4], int x, int y){
	int i = 0;
	int yOffset = 2;
	while(i < 4){
		if((TetrisDrop[i].y+y) <= 0 || (TetrisDrop[i].y+y) >= ROWS){
			return true;
		}
		else if(((int)TetrisDrop[i].y+y)<ROWS && ((int)TetrisDrop[i].x+x) >= 0 && (TetrisDrop[i].x+x) < COLS){
			if(grid[(int)TetrisDrop[i].y+y-1][(int)TetrisDrop[i].x+x] != 0){
				return true;
			}
		}
		i++;
	}
	return false;
}
bool Board::checkBottomCollision(vec2 TetrisDrop[4], int x, int y){
	int i = 0;
	int yOffset = 2;
	while(i < 4){
		if((TetrisDrop[i].y+y) < 0 || (TetrisDrop[i].y+y) >= ROWS){
			return true;
		}
		i++;
	}
	return false;
}

bool Board::checkRightSideCollison(vec2 TetrisDrop[4], int x, int y){
	for(int i = 0; i < 4; i++){
		if(grid[(int)TetrisDrop[i].y+y][(int)TetrisDrop[i].x+x] != 0 ||  TetrisDrop[i].x+x > 9 || TetrisDrop[i].y+y >= ROWS){
			return true;
		}
	}
	return false;
}
bool Board::checkLeftSideCollison(vec2 TetrisDrop[4], int x, int y){
	for(int i = 0; i < 4; i++){
		if(grid[(int)TetrisDrop[i].y+y][(int)TetrisDrop[i].x+x] != 0  || TetrisDrop[i].x+x < 0){
			return true;
		}
	}
	return false;
}
bool Board::checkRotationCollison(vec2 TetrisDrop[4], int x, int y){
	int yOffset = 2;
		
	vec2 temp[4];
	for(int i=0; i < 4; i++){
		temp[i]=vec2(-TetrisDrop[i].y,TetrisDrop[i].x);
	}
	for(int m = 0; m < 4; m++){
		if(temp[m].x+x < 0 || temp[m].x+x > COLS 
			|| temp[m].y+y < 0 || temp[m].y+y > ROWS){
			return true;
		}
		else if(grid[(int)temp[m].y+y-1][(int)temp[m].x+x-1] == 1){
			return true;
		}
	}

	return false;
}
bool Board::checkTopCollison(){
	for(int i = 0; i < COLS; i++){
		if(grid[ROWS-1][i] != 0){
			return true;
		}
	}
	return false;
}
void Board::addToStack(int x, int y){

	grid[y][x] = 1;
}
void Board::clearLine(bool fullLine, int x[10],int y[20]){
	int lineDeletion;
	for(int i = 0; i < 20; i++){
		if(y[i] != 0){
			lineDeletion = i;
			break;
		}
	}
	for(int r = lineDeletion; r < ROWS; r++){
		for(int k = 0; k < COLS; k++){
			if(r == ROWS-1){
				grid[r][k] = 0;
			}
			else{
				grid[r][k] = grid[r+1][k];
			}
		}
	}
	
}
//Runs line checker by going row by row and toggling a flag
int Board::checkLine(){
	bool lineExists;
	int xPos[10] = {0,0,0,0,0,0,0,0,0,0};
	int yPos[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	for(int r = 0; r < 20; r++){
 		lineExists = true;
 		for(int k = 0; k < 10; k++){
 			if(grid[r][k] == 0){
 				lineExists = false;
 			}
 		}
 		if(lineExists == true){;
 			yPos[r] = 1;
 			clearLine(true, xPos, yPos);
 			yPos[r] = 0;
 			return r;
 		}
 	}

 	return -1;
}
