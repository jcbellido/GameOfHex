#pragma once

#include <algorithm>
#include <random>
#include <set>
#include <vector>

#include "gamestates.h"

using namespace std;

class HexMonteCarlo
{
public:
	HexMonteCarlo(unsigned size,
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
