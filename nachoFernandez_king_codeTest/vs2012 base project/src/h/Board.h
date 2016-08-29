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
			AffectedTilesBarrier(int max = 0, int d = 0) : maxY(max), distY(d) {}
			int maxY;
			int distY;
		};

		const enum class STATE {
			IDLE,
			MOVED,
			MATCHED,
			AFFECTED,
			FALLOUT,
			WAIT
		};

		Board(GENERATION_TYPES gen = GENERATION_TYPES::RANDOM_GENERATION);
		~Board();

		int GetGridSizeX() const;
		int GetGridSizeY() const;
		int Update(Tile* source, Tile* dest);
		int GetColorOfTile(int x, int y) const;
		Tile* GetTile(int x, int y);

	private:
		const int mGridSizeX = GRID_SIZE;
		const int mGridSizeY = GRID_SIZE;

		Grid mGrid;
		STATE mState;
		STATE mNextState; // hack for waiting and updating
		std::vector<Tile> mMatched;
		std::map<int, AffectedTilesBarrier> mAffected;

		int mSX = -1; int mSY = -1; int mDX = -1; int mDY = -1;
		King::Engine::Texture mSColor;
		King::Engine::Texture mDColor;

		int TileSwap(int sX, int sY, int dX, int dY);

		std::vector<Tile> MatchTile(int x, int y, King::Engine::Texture color);
		int MatchTileRecur(int x, int y, int dx, int dy, King::Engine::Texture color, std::vector<Tile> &matched);
		int ProcessMatchedTiles(std::vector<Tile> matched, std::map<int, AffectedTilesBarrier> &affected, int &total);
		int ProcessAffectedTiles(std::map<int, AffectedTilesBarrier> affected);
	};

}

