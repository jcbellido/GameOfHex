#include "boardmodel.h"


BoardModel::BoardModel(unsigned int size): size(size)
{
	edges.resize(size * size);
	stones.resize(size * size);
	ResetBoard();
	CreateEdges();
}


Player BoardModel::ComputeWinningPlayer()
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
	
	// Chuffling the board
	random_device rd;
	mt19937 g(rd());

	shuffle(stones.begin(), stones.end(), g);
}


void BoardModel::ResetBoard()
{
	for (auto s : stones)
	{
		s = CellState::Empty;
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

