#include "game.h"

void game::changeOrder(Colour first, Colour second, Colour third)
{
	
		Colour1 = first;
		Colour2 = second;
		Colour3 = third;
	

}

int game::getMove(Stack* stack1, Stack* stack2, Stack* stack3)
{
	if (stack3->stackCount == 3 && stack3->topColour == Colour1)
	{
		cout << "End Game" << endl;
		return 0;
	}
	/*
	* STARTING POSITION
	* STACK = 3
	*/
	if (stack1->stackCount == 3 && stack2->topColour == NONE && stack3->topColour == NONE)
	{
		if (stack1->topColour == Colour1 || stack1->topColour == Colour2)
		{
			port::sendSignal(MOVE_1_TOP_2_BOTTOM);
			stack1->stackCount--;
			stack2->stackCount++;
			port::sendSignal(NOMOVE);
			waitKey(15000);
		}
		else if (stack1->topColour == Colour3)
		{
			port::sendSignal(MOVE_1_TOP_3_BOTTOM);
			stack1->stackCount--;
			stack3->stackCount++;
			port::sendSignal(NOMOVE);
			waitKey(15000);
		}
		else
		{
			port::sendSignal(READY_POSITION);
		}
	}
	/*
	* STACK 1 = 2
	*/
	if (stack1->stackCount == 2 && stack1->topColour != NONE)
	{
		if (stack2->stackCount == 1 && stack2->topColour != NONE)
		{
			if (stack1->topColour == Colour3)
			{
				port::sendSignal(MOVE_1_MIDDLE_3_BOTTOM);
				stack1->stackCount--;
				stack3->stackCount++;
				port::sendSignal(NOMOVE);
				waitKey(15000);
			}
			else if (stack1->topColour == Colour2 || stack1->topColour == Colour1)
			{
				port::sendSignal(MOVE_1_MIDDLE_2_MIDDLE);
				stack1->stackCount--;
				stack2->stackCount++;
				port::sendSignal(NOMOVE);
				waitKey(15000);
			}
		}
		else if (stack3->stackCount == 1 && stack3->topColour != NONE)
		{
			if (stack1->topColour == Colour2)
			{
				port::sendSignal(MOVE_1_MIDDLE_3_MIDDLE);
				stack1->stackCount--;
				stack3->stackCount++;
				port::sendSignal(NOMOVE);
				waitKey(15000);
			}
			else if (stack1->topColour == Colour1)
			{
				port::sendSignal(MOVE_1_MIDDLE_2_BOTTOM);
				stack1->stackCount--;
				stack2->stackCount++;
				port::sendSignal(NOMOVE);
				waitKey(15000);
			}
		}
		else
		{
			port::sendSignal(NOMOVE);
		}
	}
	/*
	* STACK 1 = 1
	*/
	else if (stack1->stackCount == 1 && stack1->topColour != NONE)
	{
		if (stack2->stackCount == 1 && stack2->topColour != NONE)
		{
			if (stack1->topColour == Colour2)
			{
				port::sendSignal(MOVE_1_BOTTOM_3_BOTTOM);
				stack1->stackCount--;
				stack3->stackCount++;
				port::sendSignal(NOMOVE);
				waitKey(15000);
			}
			else if (stack2->topColour == Colour2)
			{
				port::sendSignal(MOVE_2_BOTTOM_3_MIDDLE);
				stack2->stackCount--;
				stack3->stackCount++;
				port::sendSignal(NOMOVE);
				waitKey(15000);
			}
		}
		else if (stack2->stackCount == 2 && stack2->topColour != NONE)
		{
			if (stack1->topColour == Colour3)
			{
				port::sendSignal(MOVE_1_BOTTOM_3_BOTTOM);
				stack1->stackCount--;
				stack3->stackCount++;
				port::sendSignal(NOMOVE);
				waitKey(15000);
			}
			
		}
		else
		{
			port::sendSignal(0);
		}

	}
	/*
	* STACK 1 =0
	*/
	else if (stack1->stackCount == 0 && stack1->topColour == NONE)
	{
		if (stack2->stackCount == 1 && stack2->topColour != NONE)
		{
			port::sendSignal(MOVE_2_BOTTOM_3_TOP);
			stack2->stackCount--;
			stack3->stackCount++;
			port::sendSignal(NOMOVE);
			waitKey(15000);
		}
		else if (stack2->stackCount == 2 && stack2->topColour != NONE)
		{
			if (stack2->topColour == Colour2)
			{
				port::sendSignal(MOVE_2_MIDDLE_3_MIDDLE);
				stack2->stackCount--;
				stack3->stackCount++;
				port::sendSignal(NOMOVE);
				waitKey(15000);
			}
			else
			{
				port::sendSignal(MOVE_2_MIDDLE_1_BOTTOM);
				stack2->stackCount--;
				stack3->stackCount++;
				port::sendSignal(NOMOVE);
				waitKey(15000);
			}
		}
	}
	return 1;
}