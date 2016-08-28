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
		mState = STATE::IDLE;
		mMatched = std::vector<Tile>();
		mAffected = std::map<int, AffectedTilesBarrier>();

		switch (gen)
		{
		case GENERATION_TYPES::RANDOM_GENERATION:
			RandomGridGenerator(mGrid);
			break;
		case GENERATION_TYPES::CURATED_RANDOM_GENERATION:
			CuratedRandomGridGenerator(mGrid);
			break;
		default:
			// raise exception
			break;
		}
	}

	Board::~Board()
	{
	}

	int Board::GetGridSizeX()
	{
		return mGridSizeX;
	}

	int Board::GetGridSizeY()
	{
		return mGridSizeY;
	}

	int Board::GetColorOfTile(int x, int y)
	{
		if ((!IsWithinBoard(x, y)) || (mGrid[x][y] == nullptr))
			return -1;
		return mGrid[x][y]->GetColor();
	}

	Tile *Board::GetTile(int x, int y)
	{
		return mGrid[x][y];
	}

	int Board::GenerateTile(int x, int y)
	{
		return 0;
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

	int Board::Update(Tile* src, Tile* dest)
	{
		int total = 0;

		std::vector<Tile> sMatchedTiles = std::vector<Tile>();

		switch (mState)
		{
		case STATE::IDLE:
			mSX = -1; mSY = -1; mDX = -1; mDY = -1;
			if ((src != nullptr) && (dest != nullptr))
			{
				mSX = src->GetX(); mSY = src->GetY(); mDX = dest->GetX(); mDY = dest->GetY();

				// check distance
				if (Dist(mSX, mSY, mDX, mDY) > 1)
					return 0;

				TileSwap(mSX, mSY, mDX, mDY);
				mState = STATE::MOVED;
			}	

			break;

		case STATE::MOVED:

			mSColor = mGrid[mSX][mSY]->GetColor();
			mDColor = mGrid[mDX][mDY]->GetColor();

			mMatched = MatchTile(mDX, mDY, mDColor);

			// source
			sMatchedTiles = MatchTile(mSX, mSY, mSColor);

			mMatched.insert(mMatched.end(), sMatchedTiles.begin(), sMatchedTiles.end());

			if (mMatched.empty())
			{
				TileSwap(mSX, mSY, mDX, mDY);
				mState = STATE::IDLE;
				break;
			}

			mState = STATE::MATCHED;

			break;

		case STATE::MATCHED:
			if (mMatched.empty())
			{
				mState = STATE::IDLE;
				break;
			}
				
			ProcessMatchedTiles(mMatched, mAffected, total);
			mMatched.clear();
			mState = STATE::AFFECTED;

			break;

		case STATE::AFFECTED:
			if (mAffected.empty())
			{
				mState = STATE::IDLE;
				break;
			}

			ProcessAffectedTiles(mAffected);
			mAffected.clear();
			mState = STATE::FALLOUT;

			break;

		case STATE::FALLOUT:
			sMatchedTiles.clear();

			// board wide check
			for (int x = 0; x < GRID_SIZE; x++)
			{
				for (int y = 0; y < GRID_SIZE; y++)
				{
					if (mGrid[x][y] == nullptr)
						continue;

					sMatchedTiles = MatchTile(x, y, mGrid[x][y]->GetColor());
					if (sMatchedTiles.empty())
						continue;

					mMatched.insert(mMatched.end(), sMatchedTiles.begin(), sMatchedTiles.end());
				}
			}

			mState = STATE::MATCHED;

			break;
		default:
			break;
		}

		return total;
	}

	std::vector<Tile> Board::MatchTile(int x, int y, King::Engine::Texture color)
	{
		std::vector<Tile> matched = std::vector<Tile>();
		std::vector<Tile> tmp = std::vector<Tile>();
		
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
			matched.push_back(*mGrid[x][y]);

		for (auto t : matched)
		{
			int x = t.GetX(); int y = t.GetY();
			if (mGrid[x][y] != nullptr)
			{
				delete mGrid[x][y];
				mGrid[x][y] = nullptr;
			}
		}

		return matched;
	}

	int Board::MatchTileRecur(int x, int y, int dx, int dy, King::Engine::Texture color, std::vector<Tile> &matched)
	{
		if (!IsWithinBoard(x, y))
			return 0;

		if (mGrid[x][y] == nullptr)
			return 0;

		if (mGrid[x][y]->GetColor() == color)
		{
			matched.push_back(*mGrid[x][y]);
			return 1 + MatchTileRecur(x + dx, y + dy, dx, dy, color, matched);
		}
		return 0;
	}
	
	int Board::ProcessMatchedTiles(std::vector<Tile> matched, std::map<int, AffectedTilesBarrier> &affected, int &total)
	{
		for (auto t : matched)
		{
			int x = t.GetX(); int y = t.GetY();

			affected.insert(std::pair<int, AffectedTilesBarrier>(x, AffectedTilesBarrier()));
			affected.at(x).distY += 1;
			if (y > affected.at(x).maxY)
				affected.at(x).maxY = y;

			total += 10;
		}

		return 0;
	}

	int Board::ProcessAffectedTiles(std::map<int, AffectedTilesBarrier> affected)
	{
		for (auto barrier : affected)
		{
			int x = barrier.first;
			int maxY = barrier.second.maxY;
			int distY = barrier.second.distY;
			for (int y = maxY; y >= 0; y--)
			{
				int i = 0;
				while (mGrid[x][y] == nullptr)
				{
					i++;
					if (!IsWithinBoard(x, y - i))
						break;
					if (mGrid[x][y - i] == nullptr)
						continue;
					mGrid[x][y] = mGrid[x][y - i];
					mGrid[x][y]->SetCoords(x, y);
					mGrid[x][y - i] = nullptr;				
				}
			}
			
			// handle elements close to the edge
			for (int y = 0; y <= distY; y++)
			{
				if (mGrid[x][y] == nullptr)
					// IMPROVEMENT look at the neighbors to reduce chance of chain reactions
					mGrid[x][y] = new Tile(RandomTileGenerator(LOWER_COLOR_BOUND, UPPER_COLOR_BOUND), x, y);
			}
		}
		
		return 0;
	}
	
}