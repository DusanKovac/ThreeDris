#include "include/Angel.h"
#include "board.h"
#include "tetrisblock.h"
#include <time.h>  
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>


using namespace std;

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

#define COLS 10
#define ROWS 20
#define SCALE 35
#define TRIANGLE_PER_FACE 2
#define FACES_PER_CUBE 6

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800


static TetrisBlock* tetrisblock = new TetrisBlock();
static Board* board = new Board();
clock_t begin_time = clock();
clock_t timer = 0;
bool lostGame = false;
bool blockInPlay = false;

				     		//  R    G    B    A
const vec4 RED    		= vec4(1.0, 0.0, 0.0, 1.0);//Fruit Colours
const vec4 YELLOW 		= vec4(1.0, 1.0, 0.0, 1.0);//
const vec4 PURPLE 		= vec4(0.5, 0.0, 1.0, 1.0);//
const vec4 GREEN  		= vec4(0.5, 1.0, 0.0, 1.0);//
const vec4 ORANGE 		= vec4(1.0, 0.3, 0.0, 1.0);//
const vec4 WHITE  		= vec4(1.0, 1.0, 1.0, 1.0);// Background and Empty Cell 
const vec4 BLACK        = vec4(0.0, 0.0, 0.0, 1.0);// Grid Lines Colour
const vec4 GREY         = vec4(0.8, 0.8, 0.8, 1.0);// Grey Cube Colour
const vec4 TRANSPARENT  = vec4(0.0, 0.0, 0.0, 0.0);// Transparency for the tetris blocks

//Create all the required matrices needed to draw the image and transform the models 
mat4  View_Matrix, Model_Matrix, Projection_Matrix ,RobotArm_ModelView, ModelViewProjection;

/*=============== ROBOT ARM IMPLEMENTATION. Some of the code is sourced from: Angel.h && example1.cpp ================ */

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

//Create variables from example1.cpp
GLuint VAO_robotArm, VBO_robotArm;
GLuint vPosition, vColor, uniformMatrixLocation;
vec3 RobotArm_Pos;

const int NumVertices = 36;
point4 points[NumVertices];
color4 colors[NumVertices];
/******************************************/

point4 vertices[8] = {
    point4( -0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5, -0.5, -0.5, 1.0 ),
    point4( -0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5, -0.5, -0.5, 1.0 )
};

// RGBA olors
color4 vertex_colors[8] = {
    color4( 0.0, 0.0, 0.0, 1.0 ),  // black
    color4( 1.0, 0.0, 0.0, 1.0 ),  // red
    color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
    color4( 0.0, 1.0, 0.0, 1.0 ),  // green
    color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
    color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
    color4( 0.0, 0.0, 0.0, 1.0 ),  // black
    color4( 0.0, 1.0, 1.0, 1.0 )   // cyan
};


// Parameters controlling the size of the Robot's arm
const GLfloat BASE_HEIGHT      = 2.0;
const GLfloat BASE_WIDTH       = 5.0;
const GLfloat LOWER_ARM_HEIGHT = 10.0;
const GLfloat LOWER_ARM_WIDTH  = 0.8;
const GLfloat UPPER_ARM_HEIGHT = 10.0;
const GLfloat UPPER_ARM_WIDTH  = 0.8;

enum
{
	Base,
	LowerArm,
	UpperArm,
	NumAngles
};
int     Axis = Base;
GLfloat Theta[NumAngles] = { 0.0 };

int Index = 0;

void quad( int a, int b, int c, int d )
{
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[a]; points[Index] = vertices[b]; Index++;
	colors[Index] = vertex_colors[a]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[a]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[a]; points[Index] = vertices[d]; Index++;
}

void colorcube()
{
	quad( 1, 0, 3, 2 );
	quad( 2, 3, 7, 6 );
	quad( 3, 0, 4, 7 );
	quad( 6, 5, 1, 2 );
	quad( 4, 5, 6, 7 );
	quad( 5, 4, 0, 1 );
}

void base(const mat4 &Robot_Perspective_View)
{
	mat4 instance = ( Translate( 0.0, 0.5 * BASE_HEIGHT, 0.0 ) *
			  Scale( BASE_WIDTH,
				 BASE_HEIGHT,
				 BASE_WIDTH ) );
	
	glUniformMatrix4fv( uniformMatrixLocation, 1, GL_TRUE, Robot_Perspective_View * RobotArm_ModelView * instance );
	glDrawArrays( GL_TRIANGLES, 0, NumVertices );
}

void upper_arm(const mat4 &Robot_Perspective_View)
{
	mat4 instance = ( Translate( 0.0, 0.5 * UPPER_ARM_HEIGHT, 0.0 ) *
			  Scale( UPPER_ARM_WIDTH,
				 UPPER_ARM_HEIGHT,
				 UPPER_ARM_WIDTH ) );
	
	glUniformMatrix4fv( uniformMatrixLocation, 1, GL_TRUE, Robot_Perspective_View * RobotArm_ModelView * instance );
	glDrawArrays( GL_TRIANGLES, 0, NumVertices );
}

void lower_arm(const mat4 &Robot_Perspective_View)
{
	mat4 instance = ( Translate( 0.0, 0.5 * LOWER_ARM_HEIGHT, 0.0 ) *
			  Scale( LOWER_ARM_WIDTH,
				 LOWER_ARM_HEIGHT,
				 LOWER_ARM_WIDTH ) );
	
	glUniformMatrix4fv( uniformMatrixLocation, 1, GL_TRUE, Robot_Perspective_View * RobotArm_ModelView * instance );
	glDrawArrays( GL_TRIANGLES, 0, NumVertices );
}

void initRobot( void )
{
	Index = 0;
	RobotArm_Pos = vec3(-10, 0, 0);
	colorcube();
	
	// Create a vertex array object
	glGenVertexArrays( 1, &VAO_robotArm );
	glBindVertexArray( VAO_robotArm );
	
	// Create and initialize a buffer object
	glGenBuffers( 1, &VBO_robotArm );
	glBindBuffer( GL_ARRAY_BUFFER, VBO_robotArm );
	glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_DYNAMIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );
	
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	
	glEnableVertexAttribArray( vColor );
	glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)) );
}

/*
Following Code Implementation ID came from my reading of Red Book.
Example and Idea sourced from:
https://github.com/openglredbook/examples/blob/master/src/01-triangles/01-triangles.cpp
*/
enum VAO_IDs
{
	VAO_Grid,
	VAO_Board,
	VAO_FruitTetris
};
GLuint VAO_ID[3];

enum VBO_IDs
{
	VBO_GridFrame,
	VBO_GridColour,
	VBO_BoardFrame,
	VBO_BoardColour,
	VBO_FruitTetrisLocation,
	VBO_FruitTetrisColour
};
GLuint VBO_ID[6];

/*=============================================Board Construction====================================================== */

void Board_init(){	
	//This is from first assignment, it creates the 20x10 grid that is used for checking pos
	board->init();

	//Bind the Grid VAO
	glBindVertexArray(VAO_ID[VAO_Grid]); 
	glGenBuffers(2, VBO_ID); 

	//Set up the geometric buffer for grids
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID[VBO_GridFrame]); 
	glBufferData(GL_ARRAY_BUFFER, (((ROWS+1)*(COLS+1))*2+(COLS+ROWS+2)*2*2)*sizeof(vec4), board->gridColourLocation, GL_DYNAMIC_DRAW); 
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition); 
	
	//Set up the colour buffer for grids
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID[VBO_GridColour]); 
	glBufferData(GL_ARRAY_BUFFER, (((ROWS+1)*(COLS+1))*2+(COLS+ROWS+2)*2*2)*sizeof(vec4), board->gridColour, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor); 

	glBindVertexArray(VAO_ID[VAO_Board]);
	glGenBuffers(2, &VBO_ID[VBO_BoardFrame]);

	// Setting up Vertex Positions for the Board (Tetris Blocks that Have Fallen)
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID[VBO_BoardFrame]);
	//Buffered Statically because it will not change as it is only for the locations where we are colouring
	glBufferData(GL_ARRAY_BUFFER, (COLS*ROWS*FACES_PER_CUBE*TRIANGLE_PER_FACE*3)*sizeof(vec4), board->boardColourLocation, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// Setting up Vertex Positions Colours, And will by dynamic as we are changing it constantly
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID[VBO_BoardColour]);
	glBufferData(GL_ARRAY_BUFFER, (COLS*ROWS*FACES_PER_CUBE*TRIANGLE_PER_FACE*3)*sizeof(vec4), board->boardColour, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);
		

}

/**********************************************Fruit Tetris Area*********************************************/
vec4 fruitColours[4];
bool blockFalling = false;
void runFruitTetris()
{
	vec4 buffer_FruitTetris_Colour[4*FACES_PER_CUBE*TRIANGLE_PER_FACE*3];
	if(blockFalling == false)
	{
		int Current_Arm_Location_X = round(RobotArm_Pos.x/2 + LOWER_ARM_HEIGHT * -sin(DegreesToRadians * Theta[LowerArm]) 
						+ (UPPER_ARM_HEIGHT-0.5) * -cos(DegreesToRadians * (90 - Theta[LowerArm] - Theta[UpperArm])));
		int Current_Arm_Location_Y = round(RobotArm_Pos.y + BASE_HEIGHT + LOWER_ARM_HEIGHT * cos(-DegreesToRadians* Theta[LowerArm]) 
					   	+ (UPPER_ARM_HEIGHT-0.5) * sin(DegreesToRadians * (90 - Theta[LowerArm] - Theta[UpperArm])));

		tetrisblock->AddLocation(Current_Arm_Location_X, Current_Arm_Location_Y);
	}
	

	bool stackAndBotCollison = board->checkBottomCollision(tetrisblock->TetrisInstance, tetrisblock->TetrisLocation.x, tetrisblock->TetrisLocation.y);
	bool rightCollision = board->checkRightSideCollison(tetrisblock->TetrisInstance, tetrisblock->TetrisLocation.x, tetrisblock->TetrisLocation.y);
	bool leftCollision = board->checkLeftSideCollison(tetrisblock->TetrisInstance, tetrisblock->TetrisLocation.x, tetrisblock->TetrisLocation.y);

	if((leftCollision == false && rightCollision == false && stackAndBotCollison == false) || blockFalling)
	{
		for (int i=0; i<4*FACES_PER_CUBE*TRIANGLE_PER_FACE*3; i++){
			buffer_FruitTetris_Colour[i] = fruitColours[i/(FACES_PER_CUBE*TRIANGLE_PER_FACE*3)];
		}
	}
	else if(blockFalling != true)
	{
		for (int i=0; i<4*FACES_PER_CUBE*TRIANGLE_PER_FACE*3; i++){
			buffer_FruitTetris_Colour[i] = GREY;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID[VBO_FruitTetrisColour]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(buffer_FruitTetris_Colour), buffer_FruitTetris_Colour); 
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID[VBO_FruitTetrisLocation]); 

	for (int i=0; i<4; i++) 
	{	
		GLfloat x =  tetrisblock->TetrisLocation.x  + tetrisblock->TetrisInstance[i].x; 
		GLfloat y = tetrisblock->TetrisLocation.y  + tetrisblock->TetrisInstance[i].y;

		// Two points are used by two triangles each
		vec4 trianglesPerCube[FACES_PER_CUBE*TRIANGLE_PER_FACE*3] = { 
			vec4(SCALE*(x+1), (y+1)*SCALE,  SCALE/2, 1), vec4(SCALE*(x+1), (y+2)*SCALE,  SCALE/2, 1), vec4(SCALE*(x+2), (y+1)*SCALE,  SCALE/2, 1), 
			vec4(SCALE*(x+1), (y+2)*SCALE,  SCALE/2, 1), vec4(SCALE*(x+2), (y+1)*SCALE,  SCALE/2, 1), vec4(SCALE*(x+2), (y+2)*SCALE,  SCALE/2, 1),
			vec4(SCALE*(x+1), (y+1)*SCALE, -SCALE/2, 1), vec4(SCALE*(x+1), (y+2)*SCALE, -SCALE/2, 1), vec4(SCALE*(x+2), (y+1)*SCALE, -SCALE/2, 1),
			vec4(SCALE*(x+1), (y+2)*SCALE, -SCALE/2, 1), vec4(SCALE*(x+2), (y+1)*SCALE, -SCALE/2, 1), vec4(SCALE*(x+2), (y+2)*SCALE, -SCALE/2, 1),
			vec4(SCALE*(x+1), (y+1)*SCALE,  SCALE/2, 1), vec4(SCALE*(x+1), (y+2)*SCALE,  SCALE/2, 1), vec4(SCALE*(x+1), (y+1)*SCALE, -SCALE/2, 1), 
			vec4(SCALE*(x+1), (y+2)*SCALE,  SCALE/2, 1), vec4(SCALE*(x+1), (y+1)*SCALE, -SCALE/2, 1), vec4(SCALE*(x+1), (y+2)*SCALE, -SCALE/2, 1),
			vec4(SCALE*(x+2), (y+1)*SCALE,  SCALE/2, 1), vec4(SCALE*(x+2), (y+2)*SCALE,  SCALE/2, 1), vec4(SCALE*(x+2), (y+1)*SCALE, -SCALE/2, 1), 
			vec4(SCALE*(x+2), (y+2)*SCALE,  SCALE/2, 1), vec4(SCALE*(x+2), (y+1)*SCALE, -SCALE/2, 1), vec4(SCALE*(x+2), (y+2)*SCALE, -SCALE/2, 1),
			vec4(SCALE*(x+1), (y+2)*SCALE,  SCALE/2, 1), vec4(SCALE*(x+2), (y+2)*SCALE,  SCALE/2, 1), vec4(SCALE*(x+1), (y+2)*SCALE, -SCALE/2, 1), 
			vec4(SCALE*(x+2), (y+2)*SCALE,  SCALE/2, 1), vec4(SCALE*(x+1), (y+2)*SCALE, -SCALE/2, 1), vec4(SCALE*(x+2), (y+2)*SCALE, -SCALE/2, 1),
			vec4(SCALE*(x+1), (y+1)*SCALE,  SCALE/2, 1), vec4(SCALE*(x+2), (y+1)*SCALE,  SCALE/2, 1), vec4(SCALE*(x+1), (y+1)*SCALE, -SCALE/2, 1), 
			vec4(SCALE*(x+2), (y+1)*SCALE,  SCALE/2, 1), vec4(SCALE*(x+1), (y+1)*SCALE, -SCALE/2, 1), vec4(SCALE*(x+2), (y+1)*SCALE, -SCALE/2, 1) 
		};

		glBufferSubData(GL_ARRAY_BUFFER, i*sizeof(trianglesPerCube), sizeof(trianglesPerCube), trianglesPerCube);  
	}
}
void shiftDown(int m)
{
	//shift everything 
	for(int b = m; b < (ROWS-1); b++){
		for(int a = 0; a < COLS; a++)
		{
			for(int i=0; i<TRIANGLE_PER_FACE*FACES_PER_CUBE*3; i++){
				board->boardColour[TRIANGLE_PER_FACE*FACES_PER_CUBE*3*(COLS*b+a)+i] = board->boardColour[TRIANGLE_PER_FACE*FACES_PER_CUBE*3*(COLS*(b+1)+a)];
			}
		}
	}
	for(int a=0; a<COLS; a++)
	{
		for(int i=0; i<TRIANGLE_PER_FACE*FACES_PER_CUBE*3; i++){
			board->boardColour[TRIANGLE_PER_FACE*FACES_PER_CUBE*3*(COLS*(ROWS-1)+a)+i] = TRANSPARENT;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID[VBO_BoardColour]); 
	glBufferData(GL_ARRAY_BUFFER, COLS*ROWS*FACES_PER_CUBE*TRIANGLE_PER_FACE*3*sizeof(vec4), board->boardColour, GL_DYNAMIC_DRAW);
}
void createFruitTetris()
{
	blockInPlay = true;
	blockFalling = false;


	int Current_Arm_Location_X = round(RobotArm_Pos.x/2 + LOWER_ARM_HEIGHT * -sin(DegreesToRadians * Theta[LowerArm]) 
					+ (UPPER_ARM_HEIGHT-0.5) * -cos(DegreesToRadians * (90 - Theta[LowerArm] - Theta[UpperArm])));
	int Current_Arm_Location_Y = round(RobotArm_Pos.y + BASE_HEIGHT + LOWER_ARM_HEIGHT * cos(-DegreesToRadians* Theta[LowerArm]) 
				   	+ (UPPER_ARM_HEIGHT-0.5) * sin(DegreesToRadians * (90 - Theta[LowerArm] - Theta[UpperArm])));

	tetrisblock->init();

	tetrisblock->AddLocation(Current_Arm_Location_X, Current_Arm_Location_Y);

	for(int i=0; i<4; i++)
	{
		int random_colour = rand()%5+1;

		if(random_colour == 1){
			fruitColours[i] = RED;
		}
		else if(random_colour == 2){
			fruitColours[i] = YELLOW;
			
		}
		else if(random_colour == 3){
			fruitColours[i] = PURPLE;
			
		}
		else if(random_colour == 4){
			fruitColours[i] = GREEN;
			
		}
		else if(random_colour == 5){
			fruitColours[i] = ORANGE;	
		}
	}
	runFruitTetris();

	glBindVertexArray(0);
}


void fruitTetris_init(){

	//Set up the VAO and VBO's
	glBindVertexArray(VAO_ID[VAO_FruitTetris]);
	glGenBuffers(2, &VBO_ID[VBO_FruitTetrisLocation]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID[VBO_FruitTetrisLocation]);
	glBufferData(GL_ARRAY_BUFFER, 4*FACES_PER_CUBE*TRIANGLE_PER_FACE*3*sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID[VBO_FruitTetrisColour]);
	glBufferData(GL_ARRAY_BUFFER, 4*FACES_PER_CUBE*TRIANGLE_PER_FACE*3*sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);
}
void addToBoard()
{
	for(int i=0; i<4; i++)
	{
		int x = tetrisblock->TetrisInstance[i].x + tetrisblock->TetrisLocation.x;
		int y = tetrisblock->TetrisInstance[i].y + tetrisblock->TetrisLocation.y;
		board->addToStack(x, y);	
		board->printList();
		for(int m=0; m<TRIANGLE_PER_FACE*FACES_PER_CUBE*3; m++)
		{
			board->boardColour[TRIANGLE_PER_FACE*FACES_PER_CUBE*3*(COLS*y+x)+m] = fruitColours[i];
		}
	}
	timer = clock();
	createFruitTetris();
}
/********************************************************************************************************************/

void init (void) {
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	vPosition = glGetAttribLocation(program, "vPosition");
	vColor = glGetAttribLocation(program, "vColor");

	glGenVertexArrays(3, &VAO_ID[0]);
	Board_init();
	fruitTetris_init();
	initRobot();

	lostGame = false;
	timer = clock();
	
	//Set up Initial Robot Arm Location
	Theta[LowerArm] = 0;
	Theta[UpperArm] = -90;

	uniformMatrixLocation = glGetUniformLocation(program, "MVPmat");


	View_Matrix = LookAt(vec3(0, ROWS+10, SCALE), vec3(0, ROWS/2, 0), vec3(0, 1, 0));
   	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glClearColor(1.0, 1.0,1.0, 1.0);
	glClearDepth(1.0);
}
//1,57 rads
void drawBoard(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Projection_Matrix = Perspective(45,1.0*WINDOW_WIDTH/WINDOW_HEIGHT,10,200);

	if(lostGame == false){

		if(blockFalling == true && clock()-begin_time > 600000){
	    	bool addToStackCheck = board->checkStackCollision(tetrisblock->TetrisInstance, tetrisblock->TetrisLocation.x, tetrisblock->TetrisLocation.y);
	    	bool checkBottom = board->checkBottomCollision(tetrisblock->TetrisInstance, tetrisblock->TetrisLocation.x, tetrisblock->TetrisLocation.y);
			if(addToStackCheck == 1 || checkBottom == 1){
				addToBoard();
				int m = board->checkLine();
				while(m != -1){
					shiftDown(m);
					m = board->checkLine();
				}
				createFruitTetris();
			}
			else{
		    	tetrisblock->downTick();
			}
			begin_time = clock();
		}

		//Timer function
		if((clock()-timer)/10 >= 1){
			blockFalling == true;
		}

		if(blockInPlay == false){
			createFruitTetris();
			timer = clock();
		}

		runFruitTetris();

			
		if(blockFalling == false){
			string str = to_string(5-(clock()-timer)/1000000);
			for(int i = 0; i < str.length(); i++){
				glutStrokeCharacter(GLUT_STROKE_ROMAN, str[0]);
			}

			if(5-(clock()-timer)/1000000 == 0){
				bool addToStackCheck = board->checkStackCollision(tetrisblock->TetrisInstance, tetrisblock->TetrisLocation.x, tetrisblock->TetrisLocation.y);
				bool stackAndBotCollison = board->checkBottomCollision(tetrisblock->TetrisInstance, tetrisblock->TetrisLocation.x, tetrisblock->TetrisLocation.y);
				bool rightCollision = board->checkRightSideCollison(tetrisblock->TetrisInstance, tetrisblock->TetrisLocation.x, tetrisblock->TetrisLocation.y);
				bool leftCollision = board->checkLeftSideCollison(tetrisblock->TetrisInstance, tetrisblock->TetrisLocation.x, tetrisblock->TetrisLocation.y);
				if(stackAndBotCollison || rightCollision || leftCollision || addToStackCheck){
					lostGame = true;
				}
				else{
					timer = clock();
					blockFalling = true;
				}
			}
		}
	}		
	else{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, 'R');
	}

	glBindVertexArray(VAO_robotArm);
	mat4 M = Projection_Matrix * View_Matrix * Translate(RobotArm_Pos);
	RobotArm_ModelView = RotateY(Theta[Base] );
	base(M);
	RobotArm_ModelView *= Translate(0.0, BASE_HEIGHT, 0.0);
	RobotArm_ModelView *= RotateZ(Theta[LowerArm]);
	lower_arm(M);
	RobotArm_ModelView *= Translate(0.0, LOWER_ARM_HEIGHT, 0.0);
	RobotArm_ModelView *= RotateZ(Theta[UpperArm]);
	upper_arm(M);

	RobotArm_ModelView *= Translate(0.0, UPPER_ARM_HEIGHT, 0.0);


	//Set up the Model Matrix
	Model_Matrix = mat4();
	//Translate over to put the picture into the center via scale, then scale it down and translate in terms of the overall picture
	Model_Matrix *= Translate(0,COLS,0) * Scale(1.0/SCALE,1.0/SCALE,1.0/SCALE)* Translate(-ROWS*COLS,-(COLS+1)*SCALE,0);

	//Set up the overall matrix, calculated by calculating all previous matrices
	mat4 ModelViewProjection = Projection_Matrix * View_Matrix * Model_Matrix;
	glUniformMatrix4fv(uniformMatrixLocation,1,GL_TRUE,ModelViewProjection);

	//Draw the board.
	glBindVertexArray(VAO_ID[VAO_Board]); 
	glDrawArrays(GL_TRIANGLES,0,COLS*ROWS*FACES_PER_CUBE*TRIANGLE_PER_FACE*3); 

	//Draw the fruits falling
	glBindVertexArray(VAO_ID[VAO_FruitTetris]); 
	glDrawArrays(GL_TRIANGLES,0,4*FACES_PER_CUBE*TRIANGLE_PER_FACE*3);
	
	//Draw the grid over top, so that it is not over written
	glBindVertexArray(VAO_ID[VAO_Grid]);
	glDrawArrays(GL_LINES,0,((ROWS+1)*(COLS+1))*2+(COLS+ROWS+2)*2*2);

	//Draw the colour to the board vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID[VBO_BoardColour]); 
	glBufferData(GL_ARRAY_BUFFER, COLS*ROWS*FACES_PER_CUBE*TRIANGLE_PER_FACE*3*sizeof(vec4), board->boardColour, GL_DYNAMIC_DRAW);


	glutSwapBuffers();
}	
void Options(unsigned char input, int x, int y){
    switch (input)
    {
    case 'q':
        exit(1); // quit the game
        break;
    case 'r':
    	init();
    	createFruitTetris();
        break;
	case 'w':
		Theta[UpperArm]+=2;
		break;
	case 'a':
		Theta[LowerArm]+=2;
		break;
	case 's':
		Theta[UpperArm]-=2;
		break;
	case 'd':
		Theta[LowerArm]-=2;
		break;
	case ' ':
		bool rightCollision = board->checkRightSideCollison(tetrisblock->TetrisInstance,tetrisblock->TetrisLocation.x, tetrisblock->TetrisLocation.y);
		bool leftCollision = board->checkLeftSideCollison(tetrisblock->TetrisInstance,tetrisblock->TetrisLocation.x, tetrisblock->TetrisLocation.y);
		if(leftCollision == false && rightCollision == false)
			blockFalling = true;
		break;
	}
    glutPostRedisplay();
}
void special(int key, int x, int y)
{
	switch(key)
	{
	    case GLUT_KEY_UP:{
			printf("Up Arrow\n");
			bool rotateMoveCheck = board->checkRotationCollison(tetrisblock->TetrisInstance, tetrisblock->TetrisLocation.x, tetrisblock->TetrisLocation.y);
			if(rotateMoveCheck == false || blockFalling == false){
				printf("rotateMoveCheck: false\n");
				tetrisblock->rotateTick();
    		}
    		break;
    	}
		case GLUT_KEY_DOWN:{
			if(blockFalling == true){
		    	printf("Down Arrow\n");
		    	//Runs checks and adds to stack if it cant fall anymore
		    	bool addToStackCheck = board->checkStackCollision(tetrisblock->TetrisInstance, tetrisblock->TetrisLocation.x, tetrisblock->TetrisLocation.y);
				bool checkBottom 	 = board->checkBottomCollision(tetrisblock->TetrisInstance, tetrisblock->TetrisLocation.x, tetrisblock->TetrisLocation.y);
				if(addToStackCheck || checkBottom){
					addToBoard();
					int m = board->checkLine();
					while(m != -1){
						shiftDown(m);
						m = board->checkLine();
					}
					createFruitTetris();
				}
				else{
			    	tetrisblock->downTick();
				}
			}
	    	break;
	    }	
	    case GLUT_KEY_RIGHT:
			if(glutGetModifiers() == GLUT_ACTIVE_CTRL)
				View_Matrix *= RotateY(3);
			break;
		case GLUT_KEY_LEFT:
			if(glutGetModifiers() == GLUT_ACTIVE_CTRL)
				View_Matrix *= RotateY(-3);
			break;
    }
}
int main(int argc,char** argv) {
	printf("==================================\n");
	printf("Dusan's Tetris/Fruit Game Debuging\n");
	printf("Tool and Screen, Will output debug\n");
	printf("==================================\n");

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_MULTISAMPLE | GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("FunTris");
    glewInit();
	init();

	glutDisplayFunc(drawBoard);
	glutKeyboardFunc(Options);
	glutSpecialFunc(special);
	glutIdleFunc(drawBoard);
	glutMainLoop();
}
