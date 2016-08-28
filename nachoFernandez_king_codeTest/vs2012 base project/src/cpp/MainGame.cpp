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

	// IMPROVEMENT Data driven
	// get sizes and calculate padding factor from textures themselves
	// define positions in data input format (json for instance)

	void MainGame::Start()
	{
		mBoard = new Board(CURATED_RANDOM_GENERATION);

	    mEngine.Start(*this);
	}

	void MainGame::Update()
	{
		HandleMouse();

		Render();

		//if ((mFirstSelectedTile != nullptr) && (mouseX > 0) && (mouseY > 0))
			//mEngine.Render(mFirstSelectedTile->GetColor(), mouseX, mouseY);

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

	void MainGame::HandleMouse()
	{
		float mouseX = -1;
		float mouseY = -1;
		if (mEngine.GetMouseButtonDown()) {
			mouseX = mEngine.GetMouseX();
			mouseY = mEngine.GetMouseY();
			mCoordX = PixelPositionToBoardCoord(mouseX, GRID_START_X);
			mCoordY = PixelPositionToBoardCoord(mouseY, GRID_START_Y);

			if (IsWithinBoard(mCoordX,mCoordY))
			{
				// inside the board
				if (mFirstSelectedTile != nullptr)
				{
					// And we have already selected one tile
					if (!((mCoordX == mFirstSelectedTile->GetX()) &&
						(mCoordY == mFirstSelectedTile->GetY())))
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
			if (IsWithinBoard(mCoordX, mCoordY))
			{
				if ((mFirstSelectedTile != nullptr) && (mSecondSelectedTile != nullptr))
				{
					// we have both tiles
					mTotalScore += mBoard->HandleTileMove(mFirstSelectedTile, mSecondSelectedTile);
					mFirstSelectedTile = nullptr;
					mSecondSelectedTile = nullptr;
				}
			}
			else
			{
				mFirstSelectedTile = nullptr;
				mSecondSelectedTile = nullptr;
			}
		}
	}

	void MainGame::Render()
	{
		mEngine.Render(King::Engine::TEXTURE_BACKGROUND, 0.0f, 0.0f);

		for (int y = 0; y < GRID_SIZE; y++)
		{
			for (int x = 0; x < GRID_SIZE; x++)
			{
				float xPos = BoardCoordToPixelPosition(x, GRID_START_X);
				float yPos = BoardCoordToPixelPosition(y, GRID_START_Y);
				glm::mat4 transform = glm::mat4(SCALING_FACTOR, 0, 0, 0,
					0, SCALING_FACTOR, 0, 0,
					0, 0, 0, 0,
					xPos, yPos, 0, 1);
				mEngine.Render(mBoard->GetColorOfTile(x, y), transform);
			}
		}

		std::stringstream score;
		score << "Score: " << mTotalScore;
		mEngine.Write(score.str().c_str(), SCORE_POS_X, SCORE_POS_Y);
	}

	void MainGame::Quit()
	{
		delete mBoard;
	}
}
