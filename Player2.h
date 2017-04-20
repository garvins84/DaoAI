// player2.h: player 2 agent.
// Author:    <your name>
// Date:	  <date>
// MS Visual C++
#ifndef PLAYER2_H
#define PLAYER2_H

class Player2 : public Player
{
private:
public:
	void update_stone_pos();
	Move pick_random_validmove(Move move, board game_board);
	Move get_move(unsigned short p, board game_board);
	bool validmove(Move move, board game_board);
	bool player_horver_win(Move& move, board game_board, unsigned short p, unsigned short enemyPlayer);
	bool player_cornertrap_lose(Move& move, board game_board, unsigned short p, unsigned short enemyPlayer);
	bool enemy_fourcorner_win(Move& move, board game_board, unsigned short p, unsigned short enemyPlayer);
	bool player_boxcluster_win(Move& move, board game_board, unsigned short p, unsigned short enemyPlayer);
	int get_enemy_number(unsigned short p);
	bool check_enemy_win(Move& move, board game_board, unsigned short p, unsigned short enemyPlayer);
	bool enemy_horver_win(Move& move, board game_board, unsigned short p, unsigned short enemyPlayer);
	bool player_blockfuture_enemy_horver_win(Move& move, board game_board, unsigned short p, unsigned short enemyPlayer);
	board inverse_gameboard(board game_board);
	board assert_virtual_move(board game_board, Move move, unsigned short p, unsigned short enemyPlayer);
	board remove_virtual_move(board game_board, Move move, unsigned short p, unsigned short enemyPlayer);
	bool enemy_boxcluster_win(Move& move, board game_board, unsigned short p, unsigned short enemyPlayer);
};

bool Player2::enemy_boxcluster_win(Move& move, board game_board, unsigned short p, unsigned short enemyPlayer)
{
	Move temp_move = move;
	for (int i = 0; i < XYDIM; ++i)
	{
		for (int j = 0; j < XYDIM; ++j)
		{
			if (game_board.layout[j][i] == enemyPlayer && game_board.layout[j][i + 1] == enemyPlayer && game_board.layout[j + 1][i] == enemyPlayer && game_board.layout[j + 1][i + 1] != p)
			{
				temp_move.dst_x = i + 1;
				temp_move.dst_y = j + 1;
				if (validmove(temp_move, game_board))
				{
					move.dst_x = temp_move.dst_x;
					move.dst_y = temp_move.dst_y;
					return true;
				}
			}
			if (game_board.layout[j][i] == enemyPlayer && game_board.layout[j][i + 1] == enemyPlayer && game_board.layout[j + 1][i + 1] == enemyPlayer && game_board.layout[j + 1][i] != p)
			{
				temp_move.dst_x = i;
				temp_move.dst_y = j + 1;
				if (validmove(temp_move, game_board))
				{
					move.dst_x = temp_move.dst_x;
					move.dst_y = temp_move.dst_y;
					return true;
				}
			}
		}
	}
	return false;
}

board Player2::inverse_gameboard(board game_board)
{
	board temp_board = game_board;

	for (int row = 0; row < XYDIM; row++)
	{
		for (int col = 0; col < XYDIM; col++)
		{
			if (temp_board.layout[col][row] == 2)
				temp_board.layout[col][row] = 1;
			else if (temp_board.layout[col][row] == 1)
				temp_board.layout[col][row] = 2;
		}
	}
	return temp_board;
}

board Player2::assert_virtual_move(board game_board, Move move, unsigned short p, unsigned short enemyPlayer)
{
	if (game_board.layout[move.dst_y][move.dst_x] != enemyPlayer && game_board.layout[move.src_y][move.src_x] != enemyPlayer)
	{
		game_board.layout[move.src_y][move.src_x] = 0;
		game_board.layout[move.dst_y][move.dst_x] = p;
	}

	return game_board;
}

board Player2::remove_virtual_move(board game_board, Move move, unsigned short p, unsigned short enemyPlayer)
{
	game_board.layout[move.src_y][move.src_x] = p;
	game_board.layout[move.dst_y][move.dst_x] = 0;

	return game_board;
}

bool Player2::player_blockfuture_enemy_horver_win(Move& move, board game_board, unsigned short p, unsigned short enemyPlayer)
{
	Move potential_move = move;
	Move enemy_move = { 0 };
	Move test_move = { 0 };
	enemy_move.player = p;
	board temp_board = { 0 };

	temp_board = inverse_gameboard(game_board);

	player_horver_win(enemy_move, temp_board, p, enemyPlayer);
	potential_move.dst_x = enemy_move.src_x;
	potential_move.dst_y = enemy_move.src_y;


	for (int i = 0; i < XYDIM; ++i)
	{
		for (int j = 0; j < XYDIM; ++j)
		{
			test_move = enemy_move;
			potential_move.dst_x = i;
			potential_move.dst_y = j;
			if (validmove(potential_move, game_board))
			{
				temp_board = assert_virtual_move(temp_board, potential_move, enemyPlayer, p);
				if (!player_horver_win(test_move, temp_board, p, enemyPlayer))
				{
					move.dst_x = potential_move.dst_x;
					move.dst_y = potential_move.dst_y;
					return true;
				}
				else
					temp_board = remove_virtual_move(temp_board, potential_move, enemyPlayer, p);
			}

		}
	}
	return false;
}

bool Player2::enemy_horver_win(Move& move, board game_board, unsigned short p, unsigned short enemyPlayer)
{
	Move potential_move = move;
	int p_in_row = 0;
	int enemyp_in_row = 0;
	for (int j = 0; j < XYDIM; ++j)
	{
		p_in_row = 0;
		enemyp_in_row = 0;
		for (int i = 0; i < XYDIM; ++i)
		{
			if (game_board.layout[i][j] == p)
				++p_in_row;
			if (game_board.layout[i][j] == enemyPlayer)
				++enemyp_in_row;
			if (game_board.layout[i][j] == 0)
			{
				potential_move.dst_x = j;
				potential_move.dst_y = i;
			}
			if (enemyp_in_row == 3 && p_in_row == 1)
				return false;
			if (enemyp_in_row == 3)
			{
				if (validmove(potential_move, game_board))
				{
					move.dst_x = potential_move.dst_x;
					move.dst_y = potential_move.dst_y;
					return true;
				}
				else if (!validmove(potential_move, game_board))
				{
					potential_move.src_x = 0;
					potential_move.src_y = 0;
					for (int i = 0; i < XYDIM; ++i)
					{
						for (int j = 0; j < XYDIM; ++j)
						{
							if (game_board.layout[i][j] == p)
							{
								potential_move.src_x = j;
								potential_move.src_y = i;
							}
							if (validmove(potential_move, game_board))
							{
								move.src_x = potential_move.src_x;
								move.src_y = potential_move.src_y;
								move.dst_x = potential_move.dst_x;
								move.dst_y = potential_move.dst_y;
								return true;
							}
							if (player_blockfuture_enemy_horver_win(potential_move, game_board, p, enemyPlayer))
								return true;
						}
					}
				}
				else
					return false;
			}
		}
	}
	for (int j = 0; j < XYDIM; ++j)
	{
		p_in_row = 0;
		enemyp_in_row = 0;
		for (int i = 0; i < XYDIM; ++i)
		{
			if (game_board.layout[j][i] == p)
				++p_in_row;
			if (game_board.layout[j][i] == enemyPlayer)
				++enemyp_in_row;
			if (game_board.layout[j][i] == 0)
			{
				potential_move.dst_x = i;
				potential_move.dst_y = j;
			}
			if (enemyp_in_row == 3 && p_in_row == 1)
				return false;
			if (enemyp_in_row == 3)
			{
				if (validmove(potential_move, game_board))
					return true;
				else if (!validmove(potential_move, game_board))
				{
					potential_move.dst_x = 0;
					potential_move.dst_y = 0;
					for (int i = 0; i < XYDIM; ++i)
					{
						for (int j = 0; j < XYDIM; ++j)
						{
							if (game_board.layout[i][j] == p)
							{
								potential_move.src_x = j;
								potential_move.src_y = i;
							}
							if (validmove(potential_move, game_board))
							{
								move.src_x = potential_move.src_x;
								move.src_y = potential_move.src_y;
								move.dst_x = potential_move.dst_x;
								move.dst_y = potential_move.dst_y;
								return true;
							}
							if (player_blockfuture_enemy_horver_win(potential_move, game_board, p, enemyPlayer))
								return true;
						}
					}
				}
				else
					return false;
			}
		}
	}
	return false;
}

bool Player2::check_enemy_win(Move& move, board game_board, unsigned short p, unsigned short enemyPlayer)
{
	if (enemy_fourcorner_win(move, game_board, p, enemyPlayer))
	{
		return true;
	}
	else if (enemy_horver_win(move, game_board, p, enemyPlayer))
		return true;

	else if (enemy_boxcluster_win(move, game_board, p, enemyPlayer))
		return true;

	else
		return false;
}

int Player2::get_enemy_number(unsigned short p)
{
	if (p == PLAYER1)
		return PLAYER2;
	else
		return PLAYER1;
}

// try to form a square to win

bool Player2::player_boxcluster_win(Move& move, board game_board, unsigned short p, unsigned short enemyPlayer)
{
	Move temp_move = move;
	for (int i = 0; i < XYDIM; ++i)
	{
		for (int j = 0; j < XYDIM; ++j)
		{
			if (game_board.layout[j][i] == p && game_board.layout[j][i + 1] == p && game_board.layout[j + 1][i] == p)
			{
				temp_move.dst_x = i + 1;
				temp_move.dst_y = j + 1;
				if (validmove(temp_move, game_board))
				{
					move.dst_x = temp_move.dst_x;
					move.dst_y = temp_move.dst_y;
					return true;
				}
			}
			if (game_board.layout[j][i] == p && game_board.layout[j][i + 1] == p && game_board.layout[j + 1][i + 1] == p)
			{
				temp_move.dst_x = i;
				temp_move.dst_y = j + 1;
				if (validmove(temp_move, game_board))
				{
					move.dst_x = temp_move.dst_x;
					move.dst_y = temp_move.dst_y;
					return true;
				}
			}
		}
	}
	return false;
}

//check and prevent enemy from reaching four corner win

bool Player2::enemy_fourcorner_win(Move& move, board game_board, unsigned short p, unsigned short enemyPlayer)
{
	int i = 0, j = 0;
	Move m = move;
	for (; i < XYDIM; i++)
	{
		for (; j < XYDIM; j++)
		{
			if (game_board.layout[j][i] == p)
			{
				m.src_x = i;
				m.src_y = j;
				if (game_board.layout[0][0] == enemyPlayer && game_board.layout[3][0] == enemyPlayer && game_board.layout[3][3] == enemyPlayer && game_board.layout[0][3] != p)
				{
					m.dst_x = 0;
					m.dst_y = 3;
					if (validmove(m, game_board))
					{
						move.src_x = m.src_x;
						move.src_y = m.src_y;
						move.dst_x = m.dst_x;
						move.dst_y = m.dst_y;
						return true;
					}
				}
				if (game_board.layout[3][0] == enemyPlayer && game_board.layout[3][3] == enemyPlayer && game_board.layout[0][3] == enemyPlayer && game_board.layout[0][0] != p)
				{
					m.dst_x = 0;
					m.dst_y = 0;
					if (validmove(m, game_board))
					{
						move.src_x = m.src_x;
						move.src_y = m.src_y;
						move.dst_x = m.dst_x;
						move.dst_y = m.dst_y;
						return true;
					}
				}
				if (game_board.layout[3][3] == enemyPlayer && game_board.layout[0][3] == enemyPlayer && game_board.layout[0][0] == enemyPlayer && game_board.layout[3][0] != p)
				{
					m.dst_x = 3;
					m.dst_y = 0;
					if (validmove(m, game_board))
					{
						move.src_x = m.src_x;
						move.src_y = m.src_y;
						move.dst_x = m.dst_x;
						move.dst_y = m.dst_y;
						return true;
					}
				}
			}
		}

	}
	return false;
}

Move Player2::pick_random_validmove(Move move, board game_board)
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

Move Player2::get_move(unsigned short p, board game_board)
{
	unsigned short enemyPlayer = 0;
	Move move;
	board temp_board = game_board;
	bool enemyWin = false;
	int moveOption;
	move.player = p;

	enemyPlayer = get_enemy_number(p);

	do
	{
		do{
			move = pick_random_validmove(move, game_board);
			enemyWin = check_enemy_win(move, game_board, p, enemyPlayer);
			if (enemyWin == false)
			{
				moveOption = (rand() % 4);
				switch (moveOption)
				{
				case 0:
					player_boxcluster_win(move, game_board, p, enemyPlayer);
					break;
				case 1:
					player_horver_win(move, game_board, p, enemyPlayer);
					break;
				case 2:
					move = pick_random_validmove(move, game_board);
					break;
				case 3:
					break;
				}
			}
			temp_board = assert_virtual_move(temp_board, move, p, enemyPlayer);
			if (player_cornertrap_lose(move, temp_board, p, enemyPlayer))
				temp_board = game_board;
		} while (player_cornertrap_lose(move, temp_board, p, enemyPlayer));
	} while (!validmove(move, game_board));
	return move;
}

// check if player will lose by boxing in second player
bool Player2::player_cornertrap_lose(Move& move, board game_board, unsigned short p, unsigned short enemyPlayer)
{
	if (move.dst_x == 0 && move.dst_y == 1)
	{
		if (game_board.layout[move.dst_y - 1][move.dst_x] == enemyPlayer && game_board.layout[move.dst_y][move.dst_x + 1] == p && game_board.layout[move.dst_y - 1][move.dst_x + 1] == p)
			return true;
	}
	if (move.dst_x == 1 && move.dst_y == 0)
	{
		if (game_board.layout[move.dst_y][move.dst_x - 1] == enemyPlayer && game_board.layout[move.dst_y + 1][move.dst_x] == p && game_board.layout[move.dst_y + 1][move.dst_x - 1] == p)
			return true;
	}
	if (move.dst_x == 1 && move.dst_y == 1)
	{
		if (game_board.layout[move.dst_y - 1][move.dst_x - 1] == enemyPlayer && game_board.layout[move.dst_y - 1][move.dst_x] == p && game_board.layout[move.dst_y][move.dst_x - 1] == p)
			return true;
	}
	if (move.dst_x == 2 && move.dst_y == 0)
	{
		if (game_board.layout[move.dst_y][move.dst_x + 1] == enemyPlayer && game_board.layout[move.dst_y + 1][move.dst_x] == p && game_board.layout[move.dst_y + 1][move.dst_x + 1] == p)
			return true;
	}

	if (move.dst_x == 2 && move.dst_y == 1)
	{
		if (game_board.layout[move.dst_y - 1][move.dst_x + 1] == enemyPlayer && game_board.layout[move.dst_y - 1][move.dst_x] == p && game_board.layout[move.dst_y][move.dst_x + 1] == p)
			return true;
	}

	if (move.dst_x == 3 && move.dst_y == 1)
	{
		if (game_board.layout[move.dst_y - 1][move.dst_x] == enemyPlayer && game_board.layout[move.dst_y][move.dst_x - 1] == p && game_board.layout[move.dst_y - 1][move.dst_x - 1] == p)
			return true;
	}

	if (move.dst_x == 0 && move.dst_y == 2)
	{
		if (game_board.layout[move.dst_y + 1][move.dst_x] == enemyPlayer && game_board.layout[move.dst_y][move.dst_x + 1] == p && game_board.layout[move.dst_y + 1][move.dst_x + 1] == p)
			return true;
	}
	if (move.dst_x == 1 && move.dst_y == 2)
	{
		if (game_board.layout[move.dst_y + 1][move.dst_x - 1] == enemyPlayer && game_board.layout[move.dst_y][move.dst_x - 1] == p && game_board.layout[move.dst_y + 1][move.dst_x] == p)
			return true;
	}
	if (move.dst_x == 1 && move.dst_y == 3)
	{
		if (game_board.layout[move.dst_y][move.dst_x - 1] == enemyPlayer && game_board.layout[move.dst_y - 1][move.dst_x] == p && game_board.layout[move.dst_y - 1][move.dst_x - 1] == p)
			return true;
	}
	if (move.dst_x == 2 && move.dst_y == 2)
	{
		if (game_board.layout[move.dst_y + 1][move.dst_x + 1] == enemyPlayer && game_board.layout[move.dst_y][move.dst_x + 1] == p && game_board.layout[move.dst_y + 1][move.dst_x] == p)
			return true;
	}

	if (move.dst_x == 3 && move.dst_y == 2)
	{
		if (game_board.layout[move.dst_y + 1][move.dst_x] == enemyPlayer && game_board.layout[move.dst_y][move.dst_x - 1] == p && game_board.layout[move.dst_y + 1][move.dst_x - 1] == p)
			return true;
	}

	if (move.dst_x == 2 && move.dst_y == 3)
	{
		if (game_board.layout[move.dst_y][move.dst_x + 1] == enemyPlayer && game_board.layout[move.dst_y - 1][move.dst_x + 1] == p && game_board.layout[move.dst_y - 1][move.dst_x] == p)
			return true;
	}
	return false;
}

//try to make a horizontal or vertical row win
bool Player2::player_horver_win(Move& move, board game_board, unsigned short p, unsigned short enemyPlayer)
{
	Move potential_move = move;
	int p_in_row = 0;
	int enemyp_in_row = 0;
	for (int j = 0; j < XYDIM; ++j)
	{
		p_in_row = 0;
		enemyp_in_row = 0;
		for (int i = 0; i < XYDIM; ++i)
		{
			if (game_board.layout[i][j] == p)
				++p_in_row;
			if (game_board.layout[i][j] == enemyPlayer)
				++enemyp_in_row;
			if (game_board.layout[i][j] == 0)
			{
				potential_move.dst_x = j;
				potential_move.dst_y = i;
			}
			if (p_in_row == 3 && enemyp_in_row == 1)
				return false;
			if (p_in_row == 3)
			{
				if (validmove(potential_move, game_board) && (potential_move.src_x != potential_move.dst_x))
				{
					move.dst_x = potential_move.dst_x;
					move.dst_y = potential_move.dst_y;
					return true;
				}
				else
				{
					for (int i = 0; i < XYDIM; ++i)
					{
						for (int j = 0; j < XYDIM; ++j)
						{
							if (game_board.layout[i][j] == p)
							{
								potential_move.src_x = j;
								potential_move.src_y = i;
							}
							if (validmove(potential_move, game_board) && (potential_move.src_x != potential_move.dst_x))
							{
								move.src_x = potential_move.src_x;
								move.src_y = potential_move.src_y;
								move.dst_x = potential_move.dst_x;
								move.dst_y = potential_move.dst_y;
								return true;
							}
						}
					}
				}
			}
		}
	}

	for (int j = 0; j < XYDIM; ++j)
	{
		p_in_row = 0;
		enemyp_in_row = 0;
		for (int i = 0; i < XYDIM; ++i)
		{
			if (game_board.layout[j][i] == p)
				++p_in_row;
			if (game_board.layout[j][i] == enemyPlayer)
				++enemyp_in_row;
			if (game_board.layout[j][i] == 0)
			{
				potential_move.dst_x = i;
				potential_move.dst_y = j;
			}
			if (p_in_row == 3 && enemyp_in_row == 1)
				return false;
			if (p_in_row == 3)
			{
				if (validmove(potential_move, game_board) && (potential_move.src_x != potential_move.dst_x))
				{
					move.dst_x = potential_move.dst_x;
					move.dst_y = potential_move.dst_y;
					return true;
				}
				else if (!validmove(potential_move, game_board))
				{
					for (int i = 0; i < XYDIM; ++i)
					{
						for (int j = 0; j < XYDIM; ++j)
						{
							if (game_board.layout[i][j] == p)
							{
								potential_move.src_x = j;
								potential_move.src_y = i;
							}
							if (validmove(potential_move, game_board))
							{
								move.src_x = potential_move.src_x;
								move.src_y = potential_move.src_y;
								move.dst_x = potential_move.dst_x;
								move.dst_y = potential_move.dst_y;
								return true;
							}
						}
					}
				}
				else
					return false;
			}
		}
	}

	return false;
}


//check if move is valid
bool Player2::validmove(Move move, board game_board)
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
