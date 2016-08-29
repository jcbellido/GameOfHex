#include <vector>
#include <cmath>
#include <map>
#include <exception>
#include <chrono>
#include <thread>

#include <Constants.h>
#include <Board.h>
#include <RandomGridGenerator.h>
#include <CuratedRandomGridGenerator.h>
#include <LevelInfo.h>

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
		for (auto x : mGrid)
		{
			for (auto y : x)
			{
				delete y;
			}
		}
	}

	int Board::GetGridSizeX() const
	{
		return mGridSizeX;
	}

	int Board::GetGridSizeY() const
	{
		return mGridSizeY;
	}

	int Board::GetColorOfTile(int x, int y) const
	{
		/*if ((!IsWithinBoard(x, y)) || (mGrid[x][y] == nullptr))
			return -1;*/
		if (mGrid[x][y]->IsDestroyed())
			return -1;
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

	int Board::Update(Tile* src, Tile* dest)
	{
		int total = 0;

		switch (mState)
		{
		case STATE::IDLE:
		{
			mSX = -1; mSY = -1; mDX = -1; mDY = -1;
			if ((src != nullptr) && (dest != nullptr))
			{
				mSX = src->GetX(); mSY = src->GetY(); mDX = dest->GetX(); mDY = dest->GetY();

				// check distance
				if (Dist(mSX, mSY, mDX, mDY) > 1)
					return 0;

				TileSwap(mSX, mSY, mDX, mDY);
				mState = STATE::WAIT;
				mNextState = STATE::MOVED;
			}
			break; 
		}	
		case STATE::MOVED:
		{
			mSColor = mGrid[mSX][mSY]->GetColor();
			mDColor = mGrid[mDX][mDY]->GetColor();

			mMatched = MatchTile(mDX, mDY, mDColor);

			// source
			std::vector<Tile> sMatchedTiles = std::vector<Tile>();
			sMatchedTiles = MatchTile(mSX, mSY, mSColor);

			mMatched.insert(mMatched.end(), sMatchedTiles.begin(), sMatchedTiles.end());

			if (mMatched.empty())
			{
				TileSwap(mSX, mSY, mDX, mDY);
				mState = STATE::WAIT;
				mNextState = STATE::IDLE;
				break;
			}

			mState = STATE::WAIT;
			mNextState = STATE::MATCHED;

			break; 
		}
		case STATE::MATCHED:
		{
			if (mMatched.empty())
			{
				mState = STATE::WAIT;
				mNextState = STATE::IDLE;
				// return and reset score
				break;
			}

			ProcessMatchedTiles(mMatched, mAffected, total);
			mMatched.clear();
			mState = STATE::WAIT;
			mNextState = STATE::AFFECTED;

			break;
		}
		case STATE::AFFECTED:
		{
			if (mAffected.empty())
			{
				mState = STATE::WAIT;
				mNextState = STATE::IDLE;
				// return and reset score
				break;
			}

			ProcessAffectedTiles(mAffected);
			mAffected.clear();

			mState = STATE::WAIT;
			mNextState = STATE::FALLOUT;

			break;
		}
		case STATE::FALLOUT:
		{
			std::vector<Tile> sMatchedTiles = std::vector<Tile>();
			sMatchedTiles.clear();

			// board wide check
			for (int x = 0; x < GetGridSizeX(); x++)
			{
				for (int y = 0; y < GetGridSizeY(); y++)
				{
					if (mGrid[x][y]->IsDestroyed())
						continue;

					sMatchedTiles = MatchTile(x, y, mGrid[x][y]->GetColor());
					if (sMatchedTiles.empty())
						continue;

					mMatched.insert(mMatched.end(), sMatchedTiles.begin(), sMatchedTiles.end());
				}
			}

			mState = STATE::WAIT;
			mNextState = STATE::MATCHED;

			break;
		}
		case STATE::WAIT:
			std::this_thread::sleep_for(std::chrono::milliseconds(LevelInfo::GetInstance().GetWaitLimitMs()));
			mState = mNextState;
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
			if (!mGrid[x][y]->IsDestroyed())
			{
				mGrid[x][y]->SetDestroyed(true);
			}
		}

		return matched;
	}

	int Board::MatchTileRecur(int x, int y, int dx, int dy, King::Engine::Texture color, std::vector<Tile> &matched)
	{
		if (!IsWithinBoard(x, y))
			return 0;

		if (mGrid[x][y]->IsDestroyed())
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
				while (mGrid[x][y]->IsDestroyed())
				{
					i++;
					if (!IsWithinBoard(x, y - i))
						break;
					if (mGrid[x][y - i]->IsDestroyed())
						continue;
					TileSwap(x, y, x, y - i);
					/*mGrid[x][y] = mGrid[x][y - i];
					mGrid[x][y]->SetCoords(x, y);
					mGrid[x][y - i]->SetDestroyed(true);*/
				}
			}
			
			// handle elements close to the edge
			for (int y = 0; y <= distY; y++)
			{
				if (mGrid[x][y]->IsDestroyed())
				{
					mGrid[x][y]->SetColor(RandomTileGenerator(LevelInfo::GetInstance().GetLowerColorBound(), LevelInfo::GetInstance().GetUpperColorBound()));
					mGrid[x][y]->SetCoords(x, y);
					mGrid[x][y]->SetDestroyed(false);
				}
			}
		}
		
		return 0;
	}
	
}