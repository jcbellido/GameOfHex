#pragma once

#include <vector>
#include <set>
#include <map>

#include <Tile.h>
#include <Constants.h>

namespace KingsTest 
{
	typedef std::array<std::array<Tile*, GRID_SIZE>, GRID_SIZE> Grid;
	

	class Board
	{
	public:
		struct AffectedTilesBarrier {
			AffectedTilesBarrier(int min = GRID_SIZE, int d = 0) : minY(min), distY(d) {}
			int minY;
			int distY;
		};

		Board(GENERATION_TYPES gen = GENERATION_TYPES::RANDOM_GENERATION);
		~Board();

		int HandleTileMove(Tile* source, Tile* dest);
		King::Engine::Texture GetColorOfTile(int x, int y);
		Tile* GetTile(int x, int y);

	private:
		Grid mGrid;

		int TileSwap(int sX, int sY, int dX, int dY);

		std::vector<Tile*> MatchTile(int x, int y, King::Engine::Texture color);
		int MatchTileRecur(int x, int y, int dx, int dy, King::Engine::Texture color, std::vector<Tile*> &matched);
		int ProcessMatchedTiles(std::vector<Tile*> matched, std::map<int, AffectedTilesBarrier> &affected, int &total);
		int ProcessAffectedTiles(std::map<int, AffectedTilesBarrier> affected);
	};

}

