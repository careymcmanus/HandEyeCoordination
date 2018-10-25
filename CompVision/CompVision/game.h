#include "port.h"
#include "Stack.h"

enum Move {
	NOMOVE = 0,
	READY_POSITION = 32,
	/*Stack1 -> Stack2*/
	MOVE_1_TOP_2_BOTTOM = 1,
	MOVE_1_MIDDLE_2_BOTTOM= 3,
	MOVE_1_MIDDLE_2_MIDDLE = 4,

	/*Stack1 -> Stack3*/
	MOVE_1_TOP_3_BOTTOM = 2,
	MOVE_1_MIDDLE_3_BOTTOM = 5,
	MOVE_1_BOTTOM_3_BOTTOM = 13,

	MOVE_1_MIDDLE_3_MIDDLE = 6,
	MOVE_1_BOTTOM_3_MIDDLE = 7,

	MOVE_1_BOTTOM_3_TOP = 8,

	/*Stack2 -> Stack3*/
	MOVE_2_MIDDLE_3_MIDDLE = 10,
	MOVE_2_BOTTOM_3_MIDDLE = 11,

	MOVE_2_BOTTOM_3_TOP = 12,

	/*Stack2 -> stack1*/
	MOVE_2_MIDDLE_1_BOTTOM = 9

};

class game
{
	Colour Colour1 = BLUE;
	Colour Colour2 = ORANGE;
	Colour Colour3 = PURPLE;

public:
	void changeOrder(Colour first, Colour second, Colour third);
	

	int getMove(Stack* stack1, Stack* stack2, Stack* stack3);
	
};