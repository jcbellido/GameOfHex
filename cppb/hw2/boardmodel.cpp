#include "boardmodel.h"


BoardModel::BoardModel(unsigned int size): size(size)
{
	edges.resize(size * size);
	stones.resize(size * size);
	ResetBoard();
	CreateEdges();
}


bool BoardModel::CheckColorWins(CellState color_to_check) const
{
	if (color_to_check == CellState::Empty)
		return false;

	vector<unsigned int> open_positions;
	set<unsigned int> ending_positions;
	set<unsigned int> reachable;
	if (color_to_check == CellState::Red)
	{
		for (unsigned int row = 0; row < size; row++)
		{
			if (stones[GetPositionFromCoordinate(row, 0)] == color_to_check)
				open_positions.push_back(GetPositionFromCoordinate(row, 0));
			if (stones[GetPositionFromCoordinate(row, size - 1)] == color_to_check)
				ending_positions.insert(GetPositionFromCoordinate(row, size - 1));
		}
	}
	else 
	{
		for (unsigned int column = 0; column < size; column++)
		{
			if (stones[GetPositionFromCoordinate(0, column)] == color_to_check)
				open_positions.push_back(GetPositionFromCoordinate(0, column));
			if (stones[GetPositionFromCoordinate(size - 1, column)] == color_to_check)
				ending_positions.insert(GetPositionFromCoordinate(size - 1, column));
		}
	}

	// Early exit: checked it works
	if (open_positions.empty() || ending_positions.empty())
		return false;

	while (!open_positions.empty())
	{
		unsigned int examining = open_positions.back();
		open_positions.pop_back();
		reachable.insert(examining);
		// Weird cases on which the board is tiny, like size = 1, 2
		if (ending_positions.find(examining) != ending_positions.end())
			return true;

		for (auto connected : edges[examining])
		{
			if (stones[connected] == color_to_check)
			{
				if (reachable.find(connected) == reachable.end())	// newly found
				{
					if (ending_positions.find(connected) != ending_positions.end())
						return true;	// This stone in particular is connected to an existing destination
					open_positions.push_back(connected);
				}
			}
		}
	}

	return false;	// This color has not a connection
}


void BoardModel::PopulateABoardAtRandom()
{
	unsigned int position = 0;
	for (auto stone = stones.begin(); stone < stones.end(); stone++)
	{
		if (position % 2 == 0)
			*stone = CellState::Blue;
		else 
			*stone = CellState::Red;
		position++;
	}
	
	random_device rd;
	mt19937 g(rd());

	shuffle(stones.begin(), stones.end(), g);
}


void BoardModel::ResetBoard()
{
	for (auto stone = stones.begin(); stone < stones.end(); stone++)
	{
		*stone = CellState::Empty;
	}
}


const CellState BoardModel::GetStoneColor(unsigned int row, unsigned int column) const
{
	unsigned int position = column + row * size;
	return GetStoneColor(position);
}


const CellState BoardModel::GetStoneColor(unsigned int position) const
{
	return stones[position];	// not safe and in dire need of assertions
}


void BoardModel::CreateEdges()
{
	unsigned int position = 0;
	vector<unsigned int> node_edges;
	for (unsigned int row = 0; row < size; row++)
	{
		for (unsigned int column = 0; column < size; column++)
		{
			node_edges.clear();
			if (IsCentralNode(row, column))
			{
				node_edges.push_back(GetPositionFromCoordinate(row - 1, column + 0));
				node_edges.push_back(GetPositionFromCoordinate(row - 1, column + 1));
				node_edges.push_back(GetPositionFromCoordinate(row + 0, column + 1));
				node_edges.push_back(GetPositionFromCoordinate(row + 1, column + 0));
				node_edges.push_back(GetPositionFromCoordinate(row + 1, column - 1));
				node_edges.push_back(GetPositionFromCoordinate(row + 0, column - 1));
			}
			else if (IsA2ConnectionCorner(row, column))
			{
				if (row == 0)
				{
					node_edges.push_back(GetPositionFromCoordinate(row + 0, column + 1));
					node_edges.push_back(GetPositionFromCoordinate(row + 1, column + 0));
				}
				else
				{
					node_edges.push_back(GetPositionFromCoordinate(row - 1, column + 0));
					node_edges.push_back(GetPositionFromCoordinate(row + 0, column - 1));
				}
			}
			else if (IsA3ConnectionCorner(row, column))
			{
				if (row == 0)
				{
					node_edges.push_back(GetPositionFromCoordinate(row + 0, column - 1));
					node_edges.push_back(GetPositionFromCoordinate(row + 1, column - 1));
					node_edges.push_back(GetPositionFromCoordinate(row + 1, column + 0));
				}
				else
				{
					node_edges.push_back(GetPositionFromCoordinate(row - 1, column + 0));
					node_edges.push_back(GetPositionFromCoordinate(row - 1, column + 1));
					node_edges.push_back(GetPositionFromCoordinate(row + 0, column + 1));
				}
			} 
			else if (IsABoardBorder(row, column))
			{
				if (row == 0)
				{
					node_edges.push_back(GetPositionFromCoordinate(row + 0, column - 1));
					node_edges.push_back(GetPositionFromCoordinate(row + 0, column + 1));
					node_edges.push_back(GetPositionFromCoordinate(row + 1, column - 1));
					node_edges.push_back(GetPositionFromCoordinate(row + 1, column + 0));
				}
				else if (row == (size - 1))
				{
					node_edges.push_back(GetPositionFromCoordinate(row + 0, column - 1));
					node_edges.push_back(GetPositionFromCoordinate(row + 0, column + 1));
					node_edges.push_back(GetPositionFromCoordinate(row - 1, column - 0));
					node_edges.push_back(GetPositionFromCoordinate(row - 1, column + 1));
				}
				else if (column == 0)
				{
					node_edges.push_back(GetPositionFromCoordinate(row - 1, column + 0));
					node_edges.push_back(GetPositionFromCoordinate(row - 1, column + 1));
					node_edges.push_back(GetPositionFromCoordinate(row + 0, column + 1));
					node_edges.push_back(GetPositionFromCoordinate(row + 1, column + 0));
				} 
				else if (column == (size - 1))
				{
					node_edges.push_back(GetPositionFromCoordinate(row - 1, column + 0));
					node_edges.push_back(GetPositionFromCoordinate(row + 1, column + 0));
					node_edges.push_back(GetPositionFromCoordinate(row + 1, column - 1));
					node_edges.push_back(GetPositionFromCoordinate(row + 0, column - 1));
				}
			}
			edges[position] = node_edges;
			position++;
		}
	}
}


bool BoardModel::ClickOnStone(unsigned int position, CellState to_color)
{
	if (stones[position] == CellState::Empty)
	{
		stones[position] = to_color;
		return true;
	}
	return false;
}


void BoardModel::SetBoardToIdling()
{
	// assert board state is in Computing
	if (board_state == BoardStates::MoveReadyWaitingForViewUpdate)
	{
		board_state = BoardStates::Idling;
	}
}


void BoardModel::ComputeMove(CellState cpu_color)
{
	// assert board state is in Idling
	board_state = BoardStates::Computing;
	unsigned int selected_position = 0;
	double my_best_winning_chance = 0;
	for (unsigned int position = 0; position < (size * size); position++)
	{
		if (stones[position] == CellState::Empty)
		{
			auto monte_carlo = HexMonteCarlo(size, edges, stones, tests_per_position, position, cpu_color);
			auto my_winning_chance = monte_carlo.ComputeWinning();
			
			// Invert the chances if I am player two
			if (cpu_color != CellState::Red)
				my_winning_chance = 1.0 - my_winning_chance;

			// Check the position quality
			if (my_winning_chance > my_best_winning_chance)
			{
				selected_position = position;
				my_best_winning_chance = my_winning_chance;
			}
		}
	}

	stones[selected_position] = cpu_color;
	board_state = BoardStates::MoveReadyWaitingForViewUpdate;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

HexMonteCarlo::HexMonteCarlo(unsigned size,
	vector<vector<unsigned int>> &edges,
	vector<CellState> &starting_stones,
	unsigned int iterations,
	unsigned int position_to_test,
	CellState color_to_test) :
			size(size), 
			edges(edges), 
			iterations(iterations), 
			position_to_test(position_to_test), 
			original_stone_placement(starting_stones), 
			color_to_test(color_to_test)
{
	stones_shuffle_base.resize(size * size);
	unsigned int position = 0;
	for (auto stone = stones_shuffle_base.begin(); stone < stones_shuffle_base.end(); stone++)
	{
		if (position % 2 == 0)
			*stone = CellState::Blue;
		else
			*stone = CellState::Red;
		position++;
	}
}

double HexMonteCarlo::ComputeWinning()
{
	unsigned int player_one_victories = 0;
	vector<CellState> randomized_stones(size * size);

	for (unsigned int i = 0; i < iterations; i++)
	{
		ConstructRandomEndBoard(randomized_stones); 
		auto winner = ComputeWinningPlayer(randomized_stones);
		if (winner == Player::Player_One)
			player_one_victories ++;
	}
	
	chances_of_victory_of_player_one = static_cast<double>(player_one_victories) / static_cast<double>(iterations);
	return chances_of_victory_of_player_one;
}


Player HexMonteCarlo::ComputeWinningPlayer(const vector<CellState> &stones)
{
	// Testing only the red stones (player ONE)
	//  Red player wins in horizontal connection
	vector<unsigned int> open_positions;
	set<unsigned int> ending_positions;
	set<unsigned int> reachable;
	for (unsigned int row = 0; row < size; row++)
	{
		if (stones[GetPositionFromCoordinate(row, 0)] == CellState::Red)
			open_positions.push_back(GetPositionFromCoordinate(row, 0));
		if (stones[GetPositionFromCoordinate(row, size - 1)] == CellState::Red)
			ending_positions.insert(GetPositionFromCoordinate(row, size - 1));
	}

	// Early exit: checked it works
	if (open_positions.empty() || ending_positions.empty())
		return Player::Player_Two;		// Blue stones are covering any of the sides

	while (!open_positions.empty())
	{
		unsigned int examining = open_positions.back();
		open_positions.pop_back();
		reachable.insert(examining);
		// Weird cases on which the board is tiny, like size = 1, 2
		if (ending_positions.find(examining) != ending_positions.end())
			return Player::Player_One;

		for (auto connected : edges[examining])
		{
			if (stones[connected] == CellState::Red)
			{
				if (reachable.find(connected) == reachable.end())	// newly found
				{
					if (ending_positions.find(connected) != ending_positions.end())
						return Player::Player_One;	// This stone in particular is connected to an existing destination
					open_positions.push_back(connected);
				}
			}
		}
	}

	return Player::Player_Two;
}
