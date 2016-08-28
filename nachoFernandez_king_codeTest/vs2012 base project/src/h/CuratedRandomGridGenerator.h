// IMPROVEMENT: C++11_RANDOM_GENERATOR
#include <random>
#include <intrin.h>
#include <ctime>

#include <Constants.h>
#include <Tile.h>

namespace KingsTest
{
	static King::Engine::Texture CuratedRandomTileGenerator(int lower_bound, int upper_bound)
	{
		std::uniform_int_distribution<int> gen(lower_bound, upper_bound);
		std::mt19937 eng;
		eng.seed(static_cast<unsigned int>( __rdtsc()));

		int randColor = gen(eng);
		return static_cast<King::Engine::Texture>(randColor);
	}

	static void CuratedRandomGridGenerator(Grid &grid, int size = GRID_SIZE)
	{
		// sadly hardcoded values for the backwards check
		int min_backwards_check = 1;
		int max_backwards_check = 2;

		std::uniform_int_distribution<int> boardGen(LOWER_COLOR_BOUND, UPPER_COLOR_BOUND);
		std::uniform_int_distribution<int> matchCheck(min_backwards_check, max_backwards_check);
		std::mt19937 eng;

#ifdef _DEBUG_
		eng.seed();
#endif // _DEBUG
#ifndef _DEBUG_
		eng.seed(static_cast<unsigned int>(__rdtsc()));
#endif // !_DEBUG

		// IMPROVEMENT: Make statistics! time taken to complete, # of swaps, etc

		int randColor = 0;
		int back = 0;

		for (int x = 0; x < size; x++)
		{
			for (int y = 0; y < size; y++)
			{
				randColor = boardGen(eng);
				King::Engine::Texture selectedColor = static_cast<King::Engine::Texture>(randColor);

				// check neighbors
				back = matchCheck(eng); // yields 1 or 2

				// IMPROVEMENT rework this. A lot of code that looks exactly the same, there has to be a way!

				if (back == 1)
				{
					// check just one neighbor behind (on both axis)
					if ((y > 1) && (grid[x][y - 1]->GetColor() == selectedColor))
						selectedColor = static_cast<King::Engine::Texture>(((randColor + 1) % NUMBER_OF_COLORS) + 1);
					if ((x > 1) && (grid[x - 1][y]->GetColor() == selectedColor))
						selectedColor = static_cast<King::Engine::Texture>(((randColor + 1) % NUMBER_OF_COLORS) + 1);
				}
				else
				{
					// check two neighbors back
					if ((y > 1) && (grid[x][y - 1]->GetColor() == selectedColor))
						if ((y > 2) && (grid[x][y - 2]->GetColor() == selectedColor))
							selectedColor = static_cast<King::Engine::Texture>(((randColor + 1) % NUMBER_OF_COLORS) + 1);
					if ((x > 1) && (grid[x - 1][y]->GetColor() == selectedColor))
						if ((x > 2) && (grid[x - 2][y]->GetColor() == selectedColor))
							selectedColor = static_cast<King::Engine::Texture>(((randColor + 1) % NUMBER_OF_COLORS) + 1);
				}

				grid[x][y] = new Tile(selectedColor, x, y);
			}
		}
	}
}

