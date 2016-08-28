#pragma once

#include <array>
#include <string>

// Constants.h
#if !defined(_CONSTANTS_H)
#define _CONSTANTS_H

namespace KingsTest {
	class Tile; 

	const enum GENERATION_TYPES
	{
		RANDOM_GENERATION = 0,
		CURATED_RANDOM_GENERATION
	};

	// The algorithm counts the move tiled only at the end
	// this is why this value is 2 and not 3
	const int MINIMUM_MATCHED_TILES = 2;

	static inline int RandInt(int LO = 0, int HI = 1) 
	{
		return LO + static_cast <int> (rand()) / (static_cast <int> (RAND_MAX / (HI - LO)));
	}

	const int GRID_SIZE = 8;

	static inline bool IsWithinBoard(int x, int y)
	{
		return (((x >= 0) && (x < GRID_SIZE))
			&& ((y >= 0) && (y < GRID_SIZE)));
	}

	//const string LEVELS_FOLDER = "levels";

	const float GRID_START_X = 325.0f;
	const float GRID_START_Y = 95.0f;

	const float SCORE_POS_X = 50.0f;
	const float SCORE_POS_Y = 50.0f;
	const float TIMER_POS_X = 50.0f;
	const float TIMER_POS_Y = 100.0f;

	const int NUMBER_OF_COLORS = 5;
	const int LOWER_COLOR_BOUND = 1;
	const int UPPER_COLOR_BOUND = 5;

	// after slight asset modification
	const int TEXTURE_SIZE = 40;

	// these values have been sort of trial-and-error tested to make the grid look 
	// OK in the empty space of the background
	const float PADDING_FACTOR = 1.1f;

#ifdef _DEBUG_
	const float DEBUG_TEXT_START_X = 100.0f;
	const float DEBUG_TEXT_START_Y = 400.0f;
#endif

}

#endif
