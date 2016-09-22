#pragma once

#include <algorithm>
#include <bitset>
#include <random>
#include <set>
#include <vector>

#include "gamestates.h"
#include "math_utils.h"

using namespace std;

enum class BoardStates : unsigned int 
{
	Idling = 0,
	Computing = 1, 
	MoveReadyWaitingForViewUpdate = 2,
};

class BoardModel
{
public:
	BoardModel(unsigned int size);
	Player ComputeWinningPlayer();

public:
	void PopulateABoardAtRandom();
	void ResetBoard();
	
	const CellState GetStoneColor(unsigned int row, unsigned int column) const;
	const CellState GetStoneColor(unsigned int position) const;

	bool ClickOnStone(unsigned int position, CellState to_color);

	bool CheckColorWins(CellState color_to_check) const;

	void ComputeMove(CellState cpu_color);

	BoardStates GetBoardState() const { return board_state; }

	void SetBoardToIdling();

private:
	void CreateEdges();
	inline bool IsCentralNode(unsigned int row, unsigned int column) const
	{
		if ((row == 0) || (row == (size - 1)))
			return false;
		if ((column == 0) || (column == (size - 1)))
			return false;
		return true;
	}

	inline bool IsA2ConnectionCorner(unsigned int row, unsigned int column) const
	{
		if ((row == 0) && (column == 0))
			return true;
		if ((row == (size -1)) && (column == (size - 1)))
			return true;
		return false;
	}

	inline bool IsA3ConnectionCorner(unsigned int row, unsigned int column) const
	{
		if ((row == 0) && (column == (size - 1)))
			return true;
		if ((row == (size - 1)) && (column == 0))
			return true;
		return false;
	}

	inline bool IsABoardBorder(unsigned int row, unsigned int column) const
	{
		if ((row == 0) || (row == (size - 1)))
			return true;
		if ((column == 0) || (column == (size - 1)))
			return true;
		return false;
	}

private:
	inline unsigned int GetPositionFromCoordinate(unsigned int row, unsigned int column) const
	{
		return row * size + column;
	}

protected: 
	unsigned int size = 0;
	vector<vector<unsigned int>> edges;
	vector<CellState> stones;
	BoardStates board_state = BoardStates::Idling;
};