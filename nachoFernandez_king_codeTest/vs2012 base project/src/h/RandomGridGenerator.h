// IMPROVEMENT: C++11_RANDOM_GENERATOR
//#include <random>

#include <ctime>
#include <cstdlib>
#include <intrin.h>

#include <Constants.h>
#include <Tile.h>

namespace KingsTest
{
	static King::Engine::Texture RandomTileGenerator(int lower_bound, int upper_bound)
	{
		srand(static_cast <unsigned> (__rdtsc()));
		int r = RandInt(lower_bound, upper_bound);
		return static_cast<King::Engine::Texture>(r);
	}

	static Grid RandomGridGenerator(int size = GRID_SIZE)
	{
		// IMPROVEMENT: Make statistics! time taken to complete, # of swaps, etc
#ifdef _DEBUG_
		srand(static_cast <unsigned> (time(0)));
#endif // _DEBUG_
#ifndef _DEBUG_
		srand(static_cast <unsigned> (__rdtsc()));
#endif // !_DEBUG_

		Grid res;

		for (int x = 0; x < size; x++)
		{
			for (int y = 0; y < size; y++)
			{
				King::Engine::Texture selectedColor = RandomTileGenerator(LOWER_COLOR_BOUND, UPPER_COLOR_BOUND);
				res[x][y] = new Tile(selectedColor, x, y);
			}
				
		}

		return res;
	}
}

