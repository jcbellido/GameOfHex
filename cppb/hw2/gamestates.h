#pragma once

enum class CellState : unsigned int
{
	Empty = 0,
	Red = 1,
	Blue = 2,
};

enum class Player : unsigned int
{
	Player_One = 1,		// Red stones
	Player_Two = 2,		// Blue stones	
	Player_Unknown = 3,	// Unkown stones
};

// Victory conditions 
// Blue player wins in vertical connection
//  Red player wins in horizontal connection
