// player1.h: player 1 agent.
// Author:    <your name>
// Date:	  <date>
// MS Visual C++
#ifndef PLAYER1_H
#define PLAYER1_H

class Player1 : public Player
{
private:
public:
	int get_stone_pos();
	void update_stone_pos();
	Move pickvalidmove(Move move, board game_board);
	Move get_move(unsigned short p, board game_board);
	bool validmove(Move move, board game_board);
	Move strategy(Move move, board game_board);
	bool checkbox(Move& move, board game_board);
	bool enemyCheckBox(Move& move, board game_board);
	void boxwin(Move& move, board game_board);
};

// try to form a square to win

void Player1::boxwin(Move& move, board game_board)
{
	for (int i = 0; i < XYDIM; ++i)
	{
		for (int j = 0; j < XYDIM; ++j)
		{
			if (game_board.layout[i][j] == 1 && (!(game_board.layout[i][j] == 1 && game_board.layout[i + 1][j] == 1 && game_board.layout[i][j + 1] == 1)))
			{
				move.src_x = i;
				move.src_y = j;
			}
			if (game_board.layout[i][j] == 1 && game_board.layout[i + 1][j] == 1 && game_board.layout[i][j + 1] == 1)
			{
				move.dst_x = i + 1;
				move.dst_y = j + 1;
				return;
			}
		}
	}
}

//check and prevent enemy from reaching four corner win

bool Player1::enemyCheckBox(Move& move, board game_board)
{
	int i = 0, j = 0;
	bool blocked = false;
	do
	{
		move.src_x = 0;
		move.src_y = 0;
		for (; i < XYDIM; i++)
		{
			for (; j < XYDIM; j++)
			{
				if (game_board.layout[i][j] == 1)
				{
					move.src_x = i;
					move.src_y = j;
					break;
				}
			}
			if (game_board.layout[i][j] == 1)
			{
				++i;
				++j;
				break;
			}

		}
		if (game_board.layout[0][0] == 2 && game_board.layout[3][0] == 2 && game_board.layout[3][3] == 2 && game_board.layout[0][3] != 1)
		{
			move.dst_x = 0;
			move.dst_y = 3;
			if (validmove(move, game_board))
				blocked = true;
		}
		else if (game_board.layout[3][0] == 2 && game_board.layout[3][3] == 2 && game_board.layout[0][3] == 2 && game_board.layout[0][0] != 1)
		{
			move.dst_x = 0;
			move.dst_y = 0;
			if (validmove(move, game_board))
				blocked = true;
		}
		else if (game_board.layout[3][3] == 2 && game_board.layout[0][3] == 2 && game_board.layout[0][0] == 2 && game_board.layout[3][0] != 1)
		{
			move.dst_x = 3;
			move.dst_y = 0;  
			if(validmove(move, game_board))
				blocked = true;
		}
		else
			blocked = false;


	} while (!blocked && (i < XYDIM && j < XYDIM));
	
	return blocked;
}     

Move Player1::pickvalidmove(Move move, board game_board)
{
	do
	{
		move.src_x = (rand() % 4);
		move.src_y = (rand() % 4);
		move.dst_x = (rand() % 4);
		move.dst_y = (rand() % 4);
	} while (!validmove(move, game_board));
	return move;
}

Move Player1::get_move(unsigned short p, board game_board)
{
	Move move; 
	bool enemyWin = false;
	int moveOption;
	move.player = p;

	do
	{
		move = pickvalidmove(move, game_board);
		do{
			enemyWin = enemyCheckBox(move, game_board);
			if (enemyWin == false)
			{
				moveOption = (rand() % 4);
				switch (moveOption)
				{
				case 0:
					boxwin(move, game_board);
					break;
				case 1:
					strategy(move, game_board);
					break;
				case 2:
					move = pickvalidmove(move, game_board);
					break;
				case 3:
					break;
				}
			}
		} while (checkbox(move, game_board));
	} while (!validmove(move, game_board));
	return move;
}

// check if player will lose by boxing in second player
bool Player1::checkbox(Move& move, board game_board)
{
	if (move.dst_x == 0 && move.dst_y == 1)
	{
		if (game_board.layout[move.dst_x][move.dst_y - 1] == 2 && game_board.layout[move.dst_x + 1][move.dst_y] == 1 && game_board.layout[move.dst_x + 1][move.dst_y - 1] == 1)
			return true;
	}
	 else if (move.dst_x == 1 && move.dst_y == 0)
	{
		if (game_board.layout[move.dst_x - 1][move.dst_y] == 2 && game_board.layout[move.dst_x][move.dst_y + 1] == 1 && game_board.layout[move.dst_x - 1][move.dst_y + 1] == 1)
			return true;
	}
	 else if (move.dst_x == 1 && move.dst_y == 1)
	{
		if (game_board.layout[move.dst_x - 1][move.dst_y - 1] == 2 && game_board.layout[move.dst_x][move.dst_y - 1] == 1 && game_board.layout[move.dst_x - 1][move.dst_y] == 1)
			return true;
	}
	 else if (move.dst_x == 2 && move.dst_y == 0)
	{
		if (game_board.layout[move.dst_x + 1][move.dst_y] == 2 && game_board.layout[move.dst_x][move.dst_y + 1] == 1 && game_board.layout[move.dst_x + 1][move.dst_y + 1] == 1)
			return true;
	}

	 else if (move.dst_x == 2 && move.dst_y == 1)
	{
		if (game_board.layout[move.dst_x + 1][move.dst_y - 1] == 2 && game_board.layout[move.dst_x][move.dst_y - 1] == 1 && game_board.layout[move.dst_x + 1][move.dst_y] == 1)
			return true;
	}

	 else if (move.dst_x == 3 && move.dst_y == 1)
	{
		if (game_board.layout[move.dst_x][move.dst_y - 1] == 2 && game_board.layout[move.dst_x - 1][move.dst_y] == 1 && game_board.layout[move.dst_x - 1][move.dst_y - 1] == 1)
			return true;
	}

	 else if (move.dst_x == 0 && move.dst_y == 2)
	{
		if (game_board.layout[move.dst_x][move.dst_y + 1] == 2 && game_board.layout[move.dst_x + 1][move.dst_y] == 1 && game_board.layout[move.dst_x + 1][move.dst_y + 1] == 1)
			return true;
	}
	 else if (move.dst_x == 1 && move.dst_y == 2)
	{
		if (game_board.layout[move.dst_x - 1][move.dst_y + 1] == 2 && game_board.layout[move.dst_x - 1][move.dst_y] == 1 && game_board.layout[move.dst_x][move.dst_y + 1] == 1)
			return true;
	}
	 else if (move.dst_x == 1 && move.dst_y == 3)
	{
		if (game_board.layout[move.dst_x - 1][move.dst_y] == 2 && game_board.layout[move.dst_x][move.dst_y - 1] == 1 && game_board.layout[move.dst_x - 1][move.dst_y - 1] == 1)
			return true;
	}
	 else if (move.dst_x == 2 && move.dst_y == 2)
	{
		if (game_board.layout[move.dst_x + 1][move.dst_y + 1] == 2 && game_board.layout[move.dst_x + 1][move.dst_y] == 1 && game_board.layout[move.dst_x][move.dst_y + 1] == 1)
			return true;
	}

	 else if (move.dst_x == 3 && move.dst_y == 2)
	{
		if (game_board.layout[move.dst_x][move.dst_y + 1] == 2 && game_board.layout[move.dst_x - 1][move.dst_y] == 1 && game_board.layout[move.dst_x - 1][move.dst_y + 1] == 1)
			return true;
	}

	 else if (move.dst_x == 2 && move.dst_y == 3)
	{
		if (game_board.layout[move.dst_x + 1][move.dst_y] == 2 && game_board.layout[move.dst_x + 1][move.dst_y - 1] == 1 && game_board.layout[move.dst_x][move.dst_y - 1] == 1)
			return true;
	}
		return false;
}

//try to make a horizontal or vertical row win
Move Player1::strategy(Move move, board game_board)
{
	for (int j = 0; j < XYDIM; ++j)
	{
		for (int i = 0; i < XYDIM - 1; ++i)
		{
			if (game_board.layout[j][i] == 1 && game_board.layout[j][i + 1] == 1)
			{
				if (!checkbox(move, game_board))
				{
					move.dst_x = j;
					move.dst_y = i + 2;
				}
			}
		}
	}
	for (int j = 0; j < XYDIM; ++j)
	{
		for (int i = 0; i < XYDIM - 1; ++i)
		{
			if (game_board.layout[i][j] == 1 && game_board.layout[i][j + 1] == 1)
			{
				if (!checkbox(move, game_board))
				{
					move.dst_x = i + 2;
					move.dst_y = j;
				}
			}
		}
	}
	return move;
}

//incomplete function
int Player1::get_stone_pos()
{
	for (int i = 0; i < XYDIM; ++i)
	{
		for (int j = 0; j < XYDIM; ++j)
		{

		}
	}
	return 0;
}

//incomplete function
void update_stone_pos()
{

}

//check if move is valid
bool Player1::validmove(Move move, board game_board)
{
	short x, y;
	bool allclear;

	// Get current state of the game.

	// First, verify that a player isn't trying to move another's stone.
	if (move.player != game_board.layout[move.src_y][move.src_x])
		return false;

	// Second, verify that a player actually tries to move somewhere...
	if (move.dst_y == move.src_y && move.dst_x == move.src_x)  // Wow - really?
		return false;

	// Check for invalid 'North' move.
	if (move.dst_y < move.src_y && move.dst_x == move.src_x) {

		y = move.src_y - 1;
		allclear = true;

		while (allclear && y >= 0)
		if (game_board.layout[y][move.dst_x] != EMPTY)
			allclear = false;
		else
			y--;

		y++;	// Went past the valid move cell, so back it up.

		if (move.dst_y != y)
			return false;
	}

	// Check for invalid 'South' move.
	if (move.dst_y > move.src_y && move.dst_x == move.src_x) {

		y = move.src_y + 1;
		allclear = true;

		while (allclear && y < XYDIM)
		if (game_board.layout[y][move.dst_x] != EMPTY)
			allclear = false;
		else
			y++;

		y--;	// Went past the valid move cell, so back it up.

		if (move.dst_y != y)
			return false;
	}

	// Check for invalid 'East' move.
	if (move.dst_y == move.src_y && move.dst_x > move.src_x) {

		x = move.src_x + 1;
		allclear = true;

		while (allclear && x < XYDIM)
		if (game_board.layout[move.dst_y][x] != EMPTY)
			allclear = false;
		else
			x++;

		x--;	// Went past the valid move cell, so back it up.

		if (move.dst_x != x)
			return false;
	}

	// Check for invalid 'West' move.
	if (move.dst_y == move.src_y && move.dst_x < move.src_x) {

		x = move.src_x - 1;
		allclear = true;

		while (allclear && x >= 0)
		if (game_board.layout[move.dst_y][x] != EMPTY)
			allclear = false;
		else
			x--;

		x++;	// Went past the valid move cell, so back it up.

		if (move.dst_x != x)
			return false;
	}

	// Check for invalid 'Northeast' move.
	if (move.dst_y < move.src_y && move.dst_x > move.src_x) {

		x = move.src_x + 1;
		y = move.src_y - 1;
		allclear = true;

		while (allclear && y >= 0 && x < XYDIM)
		if (game_board.layout[y][x] != EMPTY)
			allclear = false;
		else {
			x++;
			y--;
		}

		x--;	// Went past the valid move cell, so back it up.
		y++;

		if (move.dst_x != x || move.dst_y != y)
			return false;
	}

	// Check for invalid 'Northwest' move.
	if (move.dst_y < move.src_y && move.dst_x < move.src_x) {

		x = move.src_x - 1;
		y = move.src_y - 1;
		allclear = true;

		while (allclear && y >= 0 && x >= 0)	// Fix due to Kirt Guthrie.
		if (game_board.layout[y][x] != EMPTY)
			allclear = false;
		else {
			x--;
			y--;
		}

		x++;	// Went past the valid move cell, so back it up.
		y++;

		if (move.dst_x != x || move.dst_y != y)
			return false;
	}

	// Check for invalid 'Southeast' move.
	if (move.dst_y > move.src_y && move.dst_x > move.src_x) {

		x = move.src_x + 1;
		y = move.src_y + 1;
		allclear = true;

		while (allclear && y < XYDIM && x < XYDIM)
		if (game_board.layout[y][x] != EMPTY)
			allclear = false;
		else {
			x++;
			y++;
		}

		x--;	// Went past the valid move cell, so back it up.
		y--;

		if (move.dst_x != x || move.dst_y != y)
			return false;
	}

	// Check for invalid 'Southwest' move.
	if (move.dst_y > move.src_y && move.dst_x < move.src_x) {

		x = move.src_x - 1;
		y = move.src_y + 1;
		allclear = true;

		while (allclear && y < XYDIM && x >= 0)
		if (game_board.layout[y][x] != EMPTY)
			allclear = false;
		else {
			x--;
			y++;
		}

		x++;	// Went past the valid move cell, so back it up.
		y--;

		if (move.dst_x != x || move.dst_y != y)
			return false;
	}

	return true;   // Made it past all the checks - Valid move!
}

#endif
