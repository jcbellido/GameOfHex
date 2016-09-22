#pragma once

#pragma warning(disable: 4244) // possible loss of data

#include <irrlicht.h>
#include <vector>
#include "hexagon.h"
#include "gamestates.h"
#include "boardmodel.h"

using namespace irr;
using namespace std;

class BoardView
{
public:
	BoardView(scene::ISceneManager* smgr, 
		const core::vector3d<f32>& first_hexagon_place,
		u32 board_dimension,
		f32 hexagon_size,
		BoardModel *board_model);

	const core::vector3d<f32> GetBoardCenter() const;
	f32 GetBoardWidth() const; 
	u32 GetBoardDimension() const; 

public: 
	// Testing the coloring of the cells, randomizes the Cell states
	void RandomizeBoard() const; 

	// Testing the coloring of the cells, randomizes the Cell states
	void SetCellState(u32 row, u32 column, CellState new_state);

	// All the cells to the Empty state
	void ResetBoard();

	void ClickOnBoard(const core::vector3d<f32> &click_position);

public: 
	void UpdateFromModel();

protected:
	core::vector3d<f32> first_hexagon_place;
	u32 board_dimension;
	f32 hexagon_size;

	f32 hexagon_height;
	f32 vertical_separation;
	f32 hexagon_width;

	vector<HexagonNode*> hexagons;
	BoardModel *board_model;
};