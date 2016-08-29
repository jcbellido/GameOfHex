#pragma once

#include <string>

#include <king/Updater.h>

#include <Timer.h>
#include <Tile.h>
#include <Board.h>

namespace KingsTest {
	class MainGame :
		public King::Updater
	{
	public:
		MainGame();
		~MainGame();

		void Start();

		void Update();

		void Quit();

	private:
		King::Engine mEngine;
		Board *mBoard;
		Timer mTimer;

		Tile *mFirstSelectedTile = NULL;
		Tile *mSecondSelectedTile = NULL;

		bool mPlaying = true;

		int mCoordX = 0;
		int mCoordY = 0;
		int mTotalScore = 0;

		float mMouseX = -1;
		float mMouseY = -1;

		void HandleMouse();
		void Render();

		inline float BoardCoordToPixelPosition(int c, float start/*, float tsize*/) const
		{
			return start + (TEXTURE_SIZE * c * PADDING_FACTOR);
		}

		inline int PixelPositionToBoardCoord(float p, float start/*, float tsize*/) const
		{
			return static_cast<int> ((p - start) / (TEXTURE_SIZE * PADDING_FACTOR));
		}
	};


}
