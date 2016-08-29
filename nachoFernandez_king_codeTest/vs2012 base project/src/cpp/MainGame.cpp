#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>

#include <glm/glm.hpp>

#include <MainGame.h>
#include <Constants.h>
#include <RandomGridGenerator.h>
#include <Timer.h>
#include <LevelInfo.h>
#include <json.h>

using json = nlohmann::json;

namespace KingsTest {
	
	MainGame::MainGame() : mEngine("./assets")
	{
		unsigned int width = mEngine.GetTextureWidth(King::Engine::TEXTURE_BACKGROUND);
		unsigned int height = mEngine.GetTextureHeight(King::Engine::TEXTURE_BACKGROUND);
		mEngine.ResizeWindow(width, height);

		json levelInfo;
		std::ifstream file("levels\\level1.json", std::fstream::in | std::fstream::out);
		if (!file)
			throw std::runtime_error("Level file not found");

		file >> levelInfo;
		file.close();

		LevelInfo::GetInstance().Initialize(levelInfo);

		mBoard = new Board(static_cast<GENERATION_TYPES>(LevelInfo::GetInstance().GetGeneratorType()));
	}
	
	MainGame::~MainGame()
	{
		delete mBoard;
	}

	void MainGame::Start()
	{
		mTimer.Start();
	    mEngine.Start(*this);
}

	void MainGame::Update()
	{
		if (mPlaying)
		{
			HandleMouse();

			mTotalScore += mBoard->Update(mFirstSelectedTile, mSecondSelectedTile);
			if ((mFirstSelectedTile != nullptr) && (mSecondSelectedTile != nullptr))
			{
				mFirstSelectedTile = nullptr;
				mSecondSelectedTile = nullptr;
			}
		}

		Render();
		// check if we're out of time
		if (LevelInfo::GetInstance().GetTimerLimit() - mTimer.GetElapsedTime() == 0)
			mPlaying = false;
	}

	void MainGame::HandleMouse()
	{
		float mouseX = -1;
		float mouseY = -1;
		if (mEngine.GetMouseButtonDown()) {
			mouseX = mEngine.GetMouseX();
			mouseY = mEngine.GetMouseY();
			mCoordX = PixelPositionToBoardCoord(mouseX, LevelInfo::GetInstance().GetGridStartX());
			mCoordY = PixelPositionToBoardCoord(mouseY, LevelInfo::GetInstance().GetGridStartY());

			if (IsWithinBoard(mCoordX,mCoordY))
			{
				// inside the board
				if (mFirstSelectedTile == nullptr)
					mFirstSelectedTile = mBoard->GetTile(mCoordX, mCoordY);
			}
		}

		// IMPROVEMENT When dragging from outside the board to the inside, don't do anything

		if (!mEngine.GetMouseButtonDown())
		{
			// are the coordinates the same as the first selected tile?
			// And we have already selected one tile
			if (mFirstSelectedTile != nullptr)
			{
				if (!((mCoordX == mFirstSelectedTile->GetX()) &&
					(mCoordY == mFirstSelectedTile->GetY())))
				{
					// not in the same coordinate as the first selected tile
					// select / re-select second tile
					mSecondSelectedTile = mBoard->GetTile(mCoordX, mCoordY);
				}
				if (!IsWithinBoard(mCoordX, mCoordY))
				{
					mFirstSelectedTile = nullptr;
					mSecondSelectedTile = nullptr;
				}
			}
		}
	}

	void MainGame::Render()
	{
		if (mPlaying)
		{
			mEngine.Render(King::Engine::TEXTURE_BACKGROUND, 0.0f, 0.0f);

			for (int x = 0; x < LevelInfo::GetInstance().GetGridSize(); x++)
			{
				for (int y = 0; y < LevelInfo::GetInstance().GetGridSize(); y++)
				{
					int toRender = mBoard->GetColorOfTile(x, y);
					if (toRender == -1)
						continue;

					float xPos = BoardCoordToPixelPosition(x, LevelInfo::GetInstance().GetGridStartX());
					float yPos = BoardCoordToPixelPosition(y, LevelInfo::GetInstance().GetGridStartY());
					glm::mat4 transform = glm::mat4(1, 0, 0, 0,
						0, 1, 0, 0,
						0, 0, 0, 0,
						xPos, yPos, 0, 1);

					mEngine.Render(static_cast<King::Engine::Texture>(toRender), transform);
				}
			}

			std::stringstream score;
			score << "Score: " << mTotalScore;
			mEngine.Write(score.str().c_str(), LevelInfo::GetInstance().GetScorePosX(), LevelInfo::GetInstance().GetScorePosX());

			score.str(std::string());
			score.clear();
			score << "Timer: " << (LevelInfo::GetInstance().GetTimerLimit() - mTimer.GetElapsedTime());
			mEngine.Write(score.str().c_str(), LevelInfo::GetInstance().GetTimerPosX(), LevelInfo::GetInstance().GetTimerPosY());

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
		else
		{
			// render the background with some grey effect or something?
			// show a simple texture on top with the final score
			// plus maybe some stats? max combo, better play, etc.
			// press a key and restart
		}
	}

	void MainGame::Quit()
	{
		mTimer.Stop();
	}
}
