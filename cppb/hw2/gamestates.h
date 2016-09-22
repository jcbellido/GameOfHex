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


enum class GameStates : unsigned int
{
	Not_Started = 0,	// The runtime just started
	Waiting_Human = 1,	// Human does the next move
	Waiting_CPU = 2,	// CPU does the next move
	Finished = 3,		// Someone has won the match
};
