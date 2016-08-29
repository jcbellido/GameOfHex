#pragma once

#include <array>
#include <string>

#include <json.h>

// Constants.h
#if !defined(_CONSTANTS_H)
#define _CONSTANTS_H

using json = nlohmann::json;

namespace KingsTest {
	
	const enum class GENERATION_TYPES
	{
		RANDOM_GENERATION = 0,
		CURATED_RANDOM_GENERATION
	};

	// The algorithm counts the move tiled only at the end
	// this is why this value is 2 and not 3
	const int MINIMUM_MATCHED_TILES = 2;

	const int GRID_SIZE = 8;

	// after slight asset modification
	const int TEXTURE_SIZE = 40;

	// these values have been sort of trial-and-error tested to make the grid look 
	// OK in the empty space of the background
	const float PADDING_FACTOR = 1.1f;

	static inline int RandInt(int LO = 0, int HI = 1)
	{
		return LO + static_cast <int> (rand()) / (static_cast <int> (RAND_MAX / (HI - LO)));
	}

	static inline bool IsWithinBoard(int x, int y)
	{
		return (((x >= 0) && (x < GRID_SIZE))
			&& ((y >= 0) && (y < GRID_SIZE)));
	}

#ifdef _DEBUG_
	const float DEBUG_TEXT_START_X = 100.0f;
	const float DEBUG_TEXT_START_Y = 400.0f;
#endif

}

#endif
