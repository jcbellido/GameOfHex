#include <iostream>
#include <sstream>
#include <string>

#include <glm/glm.hpp>

#include <MainGame.h>
#include <Constants.h>
#include <RandomGridGenerator.h>

namespace KingsTest {
	
	MainGame::MainGame() : mEngine("./assets") { }
	
	MainGame::~MainGame()
	{
	}

	void MainGame::Start()
	{
		

		mBoard = new Board(CURATED_RANDOM_GENERATION);

	    mEngine.Start(*this);
	}

	void MainGame::Update()
	{
		if (mEngine.GetMouseButtonDown()) {
			// IMPROVEMENT (of sorts) have the pixel coordinates for the board on const to do the comprobation 
			// before calling any functions

			// we're clicking
			mCoordX = XPixelPositionToBoardCoord(mEngine.GetMouseX());
			mCoordY = YPixelPositionToBoardCoord(mEngine.GetMouseY());

			if ((IsWithinBoard(mCoordX)) &&
				(IsWithinBoard(mCoordY)))
			{
				// inside the board
				if (mFirstSelectedTile != NULL)
				{
					// And we have already selected one tile
					if (! ( ( mCoordX == mFirstSelectedTile->GetX() ) && 
						    ( mCoordY == mFirstSelectedTile->GetY() ) ) )
					{
						// not in the same coordinate as the first selected tile
						// select / re-select second tile
						mSecondSelectedTile = mBoard->GetTile(mCoordX, mCoordY);
					}
				}
				else
					// we select the first tile
					mFirstSelectedTile = mBoard->GetTile(mCoordX, mCoordY);
			}
		}

		// IMPROVEMENT When dragging from outside the board to the inside, don't do anything

		if (!mEngine.GetMouseButtonDown())
		{
			if ((IsWithinBoard(mCoordX)) &&
				(IsWithinBoard(mCoordY)))
			{
				if ((mFirstSelectedTile != NULL) && (mSecondSelectedTile != NULL))
				{
					// we have both tiles
					std::cout << TSwap();
					mTotalScore += mBoard->HandleTileMove(mFirstSelectedTile, mSecondSelectedTile);
					mFirstSelectedTile = NULL;
					mSecondSelectedTile = NULL;
				}
			}
			else
			{
				mFirstSelectedTile = NULL;
				mSecondSelectedTile = NULL;
			}
		}


		// check for matches


		mEngine.Render(King::Engine::TEXTURE_BACKGROUND, 0.0f, 0.0f);

		for (int y = 0; y < GRID_SIZE; y++)
		{
			for (int x = 0; x < GRID_SIZE; x++)
			{
				float xPos = XBoardCoordToPixelPosition(x);
				float yPos = YBoardCoordToPixelPosition(y);
				glm::mat4 transform = glm::mat4(SCALING_FACTOR, 0, 0, 0,
					                            0, SCALING_FACTOR, 0, 0,
					                            0, 0,                                   0, 0,
					                            xPos, yPos,                             0, 1);
				mEngine.Render(mBoard->GetColorOfTile(x, y), transform);
			}
		}

		std::stringstream score;
		score << "Score: " << mTotalScore;
		mEngine.Write(score.str().c_str(), SCORE_POS_X, SCORE_POS_Y);
		
		// DEBUG BITS AND PIECES
#ifdef _DEBUG_

		std::stringstream debugTextStream;
		debugTextStream << "[" << mCoordX << "," << mCoordY << "]";
		mEngine.Write(debugTextStream.str().c_str(), DEBUG_TEXT_START_X, DEBUG_TEXT_START_Y);

		// info of selected tile
		debugTextStream.str(std::string());
		debugTextStream.clear();
		if (mFirstSelectedTile != NULL)
		{
			debugTextStream << "First tile [" << mFirstSelectedTile->GetX() << "," << mFirstSelectedTile->GetY() << "], with color " << mFirstSelectedTile->GetColor();
			mEngine.Write(debugTextStream.str().c_str(), DEBUG_TEXT_START_X, DEBUG_TEXT_START_Y + 50.0f);
		}
			
		debugTextStream.str(std::string());
		debugTextStream.clear();
		if (mSecondSelectedTile != NULL)
		{
			debugTextStream << "Second tile [" << mSecondSelectedTile->GetX() << "," << mSecondSelectedTile->GetY() << "], with color " << mSecondSelectedTile->GetColor();
			mEngine.Write(debugTextStream.str().c_str(), DEBUG_TEXT_START_X, DEBUG_TEXT_START_Y + 100.0f);
		}
#endif
		
	}

	void MainGame::Quit()
	{
		delete mBoard;
	}

	std::string MainGame::TSwap()
	{
		if (mFirstSelectedTile == NULL || mSecondSelectedTile == NULL)
			return std::string("empty");

		std::stringstream tStream;
		tStream << "Swapping [" << mFirstSelectedTile->GetX() << "," << mFirstSelectedTile->GetY() << "]\n";
		tStream << "with \n";
		tStream << "[" << mSecondSelectedTile->GetX() << "," << mSecondSelectedTile->GetY() << "]\n";
	
		return tStream.str();
	}

}
