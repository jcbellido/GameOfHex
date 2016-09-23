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
};


class HexMonteCarlo
{
public: 
	HexMonteCarlo(	unsigned size, 
					vector<vector<unsigned int>> &edges,
					vector<CellState> &starting_stones,
					unsigned int iterations, 
					unsigned int position_to_test,
					CellState color_to_test);
	
	double ComputeWinning();

	double GetChancesPlayerOneWinning() const { return chances_of_victory_of_player_one; }

protected: 
	Player ComputeWinningPlayer(const vector<CellState> &stones);

protected:
	unsigned int size;
	const vector<vector<unsigned int>> &edges;	// No need for recreate this one
	const vector<CellState> &original_stone_placement;

	vector<CellState> stones_shuffle_base;

	unsigned int iterations; 
	unsigned int position_to_test;
	CellState color_to_test = CellState::Empty;
	double chances_of_victory_of_player_one = -1;

private:
	inline unsigned int GetPositionFromCoordinate(unsigned int row, unsigned int column) const
	{
		return row * size + column;
	}

	inline void ConstructRandomEndBoard(vector<CellState> &stones)
	{
		random_device rd;
		mt19937 g(rd());
		shuffle(stones_shuffle_base.begin(), stones_shuffle_base.end(), g);

		// STD has to have a vector combination for sure
		unsigned int position = 0;
		for (auto stone = stones.begin(); stone < stones.end(); stone++)
		{
			if (original_stone_placement[position] != CellState::Empty)
				*stone = original_stone_placement[position];
			else
				*stone = stones_shuffle_base[position];
			position++;
		}
		
		// The play we're examining
		stones[position_to_test] = color_to_test;
	} 
};
