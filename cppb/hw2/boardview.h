#pragma once

#pragma warning(disable: 4244) // possible loss of data

#include <irrlicht.h>
#include <vector>
#include "hexagon.h"
#include "gamestates.h"

using namespace irr;
using namespace std;

class BoardView
{
public:
	BoardView(scene::ISceneManager* smgr, 
		const core::vector3d<f32>& first_hexagon_place,
		u32 board_dimension,
		f32 hexagon_size);

	const core::vector3d<f32> GetBoardCenter() const;
	f32 GetBoardWidth() const; 

public: 
	void RandomizeBoard() const; 

protected:
	core::vector3d<f32> first_hexagon_place;
	u32 board_dimension;
	f32 hexagon_size;

	f32 hexagon_height;
	f32 vertical_separation;
	f32 hexagon_width;

	vector<HexagonNode*> hexagons;
};