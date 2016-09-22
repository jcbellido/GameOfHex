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
	return Player::Player_Unknown;
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

