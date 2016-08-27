#pragma once

#include <string>

#include <king/Updater.h>
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
		// IMPROVEMENT: MGRID_AS_UNIQUE_PTR
		//std::unique_ptr<Tile**[]> mGrid;

		Tile *mFirstSelectedTile = NULL;
		Tile *mSecondSelectedTile = NULL;
		bool mClickControl = false;

		int mCoordX = 0;
		int mCoordY = 0;
		int mTotalScore = 0;


		// IMPROVEMENT: make this get GRID_START too to reduce to one function per conversion
		inline float XBoardCoordToPixelPosition(int x) {
			return GRID_START_X + (TEXTURE_SIZE * x * PADDING_FACTOR);
		}

		inline float YBoardCoordToPixelPosition(int y) {
			return GRID_START_Y + (TEXTURE_SIZE * y * PADDING_FACTOR);
		}

		inline int XPixelPositionToBoardCoord(float p) {
			return static_cast<int> ((p - GRID_START_X) / (TEXTURE_SIZE * PADDING_FACTOR));
		}

		inline int YPixelPositionToBoardCoord(float p) {
			return static_cast<int> ((p - GRID_START_Y) / (TEXTURE_SIZE * PADDING_FACTOR));
		}

		// TODO: EXTRA STUFF CLEAN THIS UP AT THE END
		std::string TSwap();
	};


}
