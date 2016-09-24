#pragma once

#include <algorithm>
#include <bitset>
#include <random>
#include <set>
#include <vector>

#include "gamestates.h"
#include "HexMonteCarlo.h"
#include "math_utils.h"

#include "ctpl_stl.h"

using namespace std;

enum class BoardStates : unsigned int 
{
	Idling = 0,
	Computing = 1,						// I'm expected to add moves to the compute pool
	WaitingForPoolToEnd = 2,			// The processing pool is churning numbers
	MoveReadyWaitingForViewUpdate = 3,	// I'm done, the movement is done and I'm just waiting
};


class BoardModel
{
public:
	BoardModel(unsigned int size);
	~BoardModel();

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

	unsigned int GetSize() const { return size; }

	unsigned int GetTestsPerPosition() const { return tests_per_position; }
	void SetTestsPerPosition(unsigned int new_tests_per_position)  { tests_per_position = new_tests_per_position; }
	
	// Interfaceable
	void UpdateBoardModel();

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
	unsigned int tests_per_position = 100;
	unsigned int size = 0;
	vector<vector<unsigned int>> edges;
	vector<CellState> stones;
	BoardStates board_state = BoardStates::Idling;
	
	ctpl::thread_pool *thread_pool = nullptr;
};

