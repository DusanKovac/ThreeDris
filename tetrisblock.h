#include "include/Angel.h"
class TetrisBlock {

	//size of the board and its grids
	
	// 1 == LshapeLeft
	// 2 == LshapeRight
	// 3 == Ishape
	// 4 == Tshape
	// 5 == SshapeRight
	// 6 == SshapeLeft
	//
	const vec2 TILE_SHAPES[6][4] =
	{
		/* LshapeLeft[4][2] */
		{vec2(-1,-1),
		vec2(0,-1),
		vec2(0,0),
		vec2(0,1)},
		/* LshapeRight[4][2] */
		{vec2(1,-1),
		vec2(0,-1),
		vec2(0,0),
		vec2(0,1)},
		/* Ishape[4][2] */
		{vec2(0,-2),
		vec2(0,-1),
		vec2(0,0),
		vec2(0,1)},
		/* Tshape[4][2] */
		{vec2(0,-1),
		vec2(-1,0),
		vec2(0,0),
		vec2(0,1)},
		/* SshapeRight[4][2] */
		{vec2(1,-1),
		vec2(1,0),
		vec2(0,0),
		vec2(0,1)},
		/* SshapeLeft[4][2] */
		{vec2(-1,-1),
		vec2(-1,0),
		vec2(0,0),
		vec2(0,1)}
	};

	//Stack = previous blocks stacked up
	public:

		TetrisBlock();

		void init();

		// vec2 CURRENT_SHAPE[4] = {
		// 	vec2(0,0),
		// 	vec2(0,0),
		// 	vec2(0,0),
		// 	vec2(0,0)
		// };
		vec2 TetrisInstance[4];
		vec2 TetrisLocation;

		int tetrisColour[4] = {-1,-1,-1,-1};

		//copies specific shape to temp local instance
		void copyToInstance(int instance);	
		void AddLocation(int x, int y);	
		void rotateTick();		
		//Moves relative tetris pos down
		void downTick();
};