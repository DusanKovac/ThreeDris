		
#define COLS 10
#define ROWS 20
#define SCALE 35
#define TRIANGLE_PER_FACE 2
#define FACES_PER_CUBE 6
class Board {
	//Stack = previous blocks stacked up
	public:

		Board();
		//Current number of blocks in the stack
		int points;
		//size of the board and its grids
		int grid[20][10];
		//The Colour of the given position, the coordinate will be determined in fruit.cpp
		//once its put into the buffer with boardColourLocation
		//Possible vertexes: #COLS*ROWS is how many cubes we have
		//FACES_PER_CUBE is how many faces on the cubes, TRIANGLES per face are the triangles required to make
		//a square (2) and 3 are the number of verticies per triangle.
		vec4 boardColour[COLS*ROWS*FACES_PER_CUBE*TRIANGLE_PER_FACE*3];
		vec4 boardColourLocation[COLS*ROWS*FACES_PER_CUBE*TRIANGLE_PER_FACE*3];
		//For lines connecting the two sides of the 
		//ROWS+1 Vertices for all rows, COLS+1 verticies for all cols per side
		//To form the connection short lines between the front and bakc face
		//To form the long vertical lines: ROWS+1 + COLS+1 then x2 for both verticies to form the line
		//and then again for both faces require the same grid
		vec4 gridColour[((ROWS+1)*(COLS+1))*2+(COLS+ROWS+2)*2*2];
		vec4 gridColourLocation[((ROWS+1)*(COLS+1))*2+(COLS+ROWS+2)*2*2];

		void setUpGrid();
		void setToEmpty();

		//Initialises/Resets board (to remove the need of creating new objects each time)
		void init();

		//check if the falling tetris block has collided with the current stack(previous blocks)
		bool checkStackCollision(vec2 TetrisDrop[4], int x, int y);
		bool checkBottomCollision(vec2 TetrisDrop[4], int x, int y);
		//Checks the validity of the tetris move on either side
		bool checkRightSideCollison(vec2 TetrisDrop[4], int x, int y);
		bool checkLeftSideCollison(vec2 TetrisDrop[4], int x, int y);
		//check if the stack has reached the top and if we have reached end condition
		bool checkTopCollison();
		//check if rotation is legal move
		bool checkRotationCollison(vec2 TetrisDrop[4], int x, int y);

		//Adds the fallign tetris block to t
		void addToStack(int x, int y);

		//checks horizontal for fill or horizontal colour equals
		int checkLine();

		void clearLine(bool fullLine, int x[10], int y[20]);
};