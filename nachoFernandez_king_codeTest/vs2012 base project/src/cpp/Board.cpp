#include <vector>
#include <cmath>
#include <map>
#include <exception>

#include <Constants.h>
#include <Board.h>
#include <RandomGridGenerator.h>
#include <CuratedRandomGridGenerator.h>

namespace KingsTest 
{
	static inline double Dist(int sX, int sY, int dX, int dY)
	{
		return sqrt( pow(dX - sX, 2) + pow(dY - sY, 2) );
	}

	Board::Board(GENERATION_TYPES gen)
	{
		// IMPROVEMENT: MGRID_AS_UNIQUE_PTR
		//mGrid = std::unique_ptr<Cell**[]>(new Cell**[GRID_SIZE]);
		/*mGrid = new Tile**[GRID_SIZE];
		for (int x = 0; x < GRID_SIZE; x++)
		{
		mGrid[x] = new Tile*[GRID_SIZE];
		for (int y = 0; y < GRID_SIZE; y++)
		mGrid[x][y] = (x == 3 && y == 5) ? new Tile(King::Engine::TEXTURE_RED) : new Tile(King::Engine::TEXTURE_GREEN);
		}*/

		switch (gen)
		{
		case GENERATION_TYPES::RANDOM_GENERATION:
			mGrid = RandomGridGenerator();
			break;
		case GENERATION_TYPES::CURATED_RANDOM_GENERATION:
			mGrid = CuratedRandomGridGenerator();
			break;
		default:
			// raise exception
			break;
		}
	}

	Board::~Board()
	{
	}

	King::Engine::Texture Board::GetColorOfTile(int x, int y)
	{
		return mGrid[x][y]->GetColor();
	}

	Tile *Board::GetTile(int x, int y)
	{
		return mGrid[x][y];
	}

	int Board::TileSwap(int sX, int sY, int dX, int dY)
	{
		Tile *temp = mGrid[dX][dY];
		mGrid[dX][dY] = mGrid[sX][sY];
		mGrid[sX][sY] = temp;

		mGrid[dX][dY]->SetCoords(dX, dY);
		mGrid[sX][sY]->SetCoords(sX, sY);

		return 0;
	}

	int Board::HandleTileMove(Tile* src, Tile* dest)
	{
		int sX = src->GetX(), sY = src->GetY(), dX = dest->GetX(), dY = dest->GetY();
		int dirX = dX - sX;
		int dirY = dY - sY;

		// check distance
		if (Dist(sX, sY, dX, dY) > 1)
			return -1;

		TileSwap(sX, sY, dX, dY);

		// IMPROVEMENT: use a set, make comparisons more elegant
		
		int total = 0;
		std::vector<Tile*> matchedTiles = std::vector<Tile*>();
		
		std::vector<Tile*> sMatchedTiles = std::vector<Tile*>();
		std::vector<Tile*> dMatchedTiles = std::vector<Tile*>();
		King::Engine::Texture sColor = mGrid[sX][sY]->GetColor();
		King::Engine::Texture dColor = mGrid[dX][dY]->GetColor();

		// destination
		dMatchedTiles = MatchTile(dX, dY, dColor);

		// source
		sMatchedTiles = MatchTile(sX, sY, sColor);

		if ((dMatchedTiles.empty()) && (sMatchedTiles.empty()))
		{
			TileSwap(sX, sY, dX, dY);
		}

		std::map<int, AffectedTilesBarrier> affectedTiles;

		ProcessMatchedTiles(sMatchedTiles, affectedTiles, total);
		ProcessMatchedTiles(dMatchedTiles, affectedTiles, total);

		ProcessAffectedTiles(affectedTiles);
		
		return total;
	}

	std::vector<Tile*> Board::MatchTile(int x, int y, King::Engine::Texture color)
	{
		std::vector<Tile*> matched = std::vector<Tile*>();
		std::vector<Tile*> tmp = std::vector<Tile*>();
		
		// vertical
		int tileCount = MatchTileRecur(x, y + 1, 0, 1, color, tmp) +
			        MatchTileRecur(x, y - 1, 0, -1, color, tmp);

		if (tileCount >= MINIMUM_MATCHED_TILES) 
			for (auto t : tmp)
				matched.push_back(t);

		tmp.clear();

		// horizontal
		tileCount = MatchTileRecur(x + 1, y, 1, 0, color, tmp) +
			        MatchTileRecur(x - 1, y, -1, 0, color, tmp);

		if (tileCount >= MINIMUM_MATCHED_TILES)
			for (auto t : tmp)
				matched.push_back(t);

		if (!matched.empty())
			matched.push_back(mGrid[x][y]);

		return matched;
	}

	int Board::MatchTileRecur(int x, int y, int dx, int dy, King::Engine::Texture color, std::vector<Tile*>& matched)
	{
		if ((!IsWithinBoard(x)) || (!IsWithinBoard(y)))
			return 0;

		if (mGrid[x][y]->GetColor() == color)
		{
			matched.push_back(mGrid[x][y]);
			return 1 + MatchTileRecur(x + dx, y + dy, dx, dy, color, matched);
		}
		return 0;
	}
	
	int Board::ProcessMatchedTiles(std::vector<Tile*> matched, std::map<int, AffectedTilesBarrier> &affected, int &total)
	{
		for (auto t : matched)
		{
			int x = t->GetX(); int y = t->GetY();
			if (mGrid[x][y] != nullptr)
			{
				/*
				The single element versions (1) return a pair, with its member pair::first set to an iterator 
				pointing to either the newly inserted element or to the element with an equivalent key in the map. 
				The pair::second element in the pair is set to true if a new element was inserted or false if an 
				equivalent key already existed.
				*/

				affected.insert(std::pair<int, AffectedTilesBarrier>(x, AffectedTilesBarrier()));
				affected.at(x).distY += 1;
				if (y < affected.at(x).minY)
					affected.at(x).minY = y;

				total += 10;
				delete mGrid[x][y];
				mGrid[x][y] = nullptr;
			}
		}

		return 0;
	}

	int Board::ProcessAffectedTiles(std::map<int, AffectedTilesBarrier> affected)
	{
		for (auto barrier : affected)
		{
			int x = barrier.first;
			int maxY = barrier.second.minY;
			int distY = barrier.second.distY;
			for (int y = maxY - 1; y >= 0; y--)
			{
				mGrid[x][y + distY] = mGrid[x][y];
				mGrid[x][y + distY]->SetCoords(x, y + distY);

				mGrid[x][y] = nullptr;
				mGrid[x][y] = new Tile(RandomTileGenerator(LOWER_COLOR_BOUND, UPPER_COLOR_BOUND), x, y);
			}
			
			// handle out of board
			for (int y = 0; y <= distY; y++)
			{
				if (mGrid[x][y] == nullptr)
					mGrid[x][y] = new Tile(RandomTileGenerator(LOWER_COLOR_BOUND, UPPER_COLOR_BOUND), x, y);
			}
		}
		
		return 0;
	}
}

/*
MANUAL MATCHING VERSION
*/

/*
if (dirX != 0)
{
// horizontally first - double check dir is abs(1)
if (mGrid[dX + dirX][dY]->GetColor() == mGrid[dX][dY]->GetColor())
{
if (mGrid[dX + 2 * dirX][dY]->GetColor() == mGrid[dX][dY]->GetColor())
{
// we have a match, destroy
matchedTiles.push_back(mGrid[dX][dY]);
matchedTiles.push_back(mGrid[dX + dirX][dY]);
matchedTiles.push_back(mGrid[dX + 2 * dirX][dY]);
}
}

// vertically
if (mGrid[dX][dY + 1]->GetColor() == mGrid[dX][dY]->GetColor())
{
if (mGrid[dX][dY + 2]->GetColor() == mGrid[dX][dY]->GetColor())
{
// we have a match, destroy
matchedTiles.push_back(mGrid[dX][dY]);
matchedTiles.push_back(mGrid[dX][dY + 1]);
matchedTiles.push_back(mGrid[dX][dY + 2]);
}
}
if (mGrid[dX][dY - 1]->GetColor() == mGrid[dX][dY]->GetColor())
{
if (mGrid[dX][dY - 2]->GetColor() == mGrid[dX][dY]->GetColor())
{
// we have a match, destroy
matchedTiles.push_back(mGrid[dX][dY]);
matchedTiles.push_back(mGrid[dX][dY - 1]);
matchedTiles.push_back(mGrid[dX][dY - 2]);
}
}

}
else if (dirY != 0)
{
// vertical it is then
if (mGrid[dX][dY + dirY]->GetColor() == mGrid[dX][dY]->GetColor())
{
if (mGrid[dX][dY + 2 * dirY]->GetColor() == mGrid[dX][dY]->GetColor())
{
// we have a match, destroy
matchedTiles.push_back(mGrid[dX][dY]);
matchedTiles.push_back(mGrid[dX][dY + dirY]);
matchedTiles.push_back(mGrid[dX][dY + 2 * dirY]);
}
}
}

for (auto t : matchedTiles)
{
int x = t->GetX(); int y = t->GetY();
if (mGrid[x][y] != nullptr)
{
delete mGrid[x][y];
mGrid[x][y] = new Tile(RandomTileGenerator(LOWER_COLOR_BOUND, UPPER_COLOR_BOUND), x, y);
}
}
*/

/* for (auto t : sMatchedTiles)
{
int x = t->GetX(); int y = t->GetY();
if (mGrid[x][y] != nullptr)
{
total += 10;
delete mGrid[x][y];
mGrid[x][y] = new Tile(RandomTileGenerator(LOWER_COLOR_BOUND, UPPER_COLOR_BOUND), x, y);
}
}

for (auto t : dMatchedTiles)
{
int x = t->GetX(); int y = t->GetY();
if (mGrid[x][y] != nullptr)
{
total += 10;
delete mGrid[x][y];
mGrid[x][y] = new Tile(RandomTileGenerator(LOWER_COLOR_BOUND, UPPER_COLOR_BOUND), x, y);
}
} */