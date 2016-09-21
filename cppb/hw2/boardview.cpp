#include "boardview.h"

BoardView::BoardView(	scene::ISceneManager* smgr, 
						const core::vector3d<f32>& first_hexagon_place, 
						u32 board_dimension,
						f32 hexagon_size):
	first_hexagon_place(first_hexagon_place), board_dimension(board_dimension), hexagon_size(hexagon_size)
{
	core::vector3d<f32> current_center = first_hexagon_place;
	s32 current_id = 0;
	// Separation calculations
	hexagon_height = 2 * hexagon_size;
	vertical_separation = hexagon_height * (3.0 / 4.0);
	hexagon_width = hexagon_height * (sqrt(3) / 2.0);

	for (u32 row = 0; row < board_dimension; ++row)
	{
		for (u32 column = 0; column < board_dimension; ++column) 
		{
			HexagonNode *myNode = new HexagonNode(smgr->getRootSceneNode(), smgr, current_id, current_center, hexagon_size);

			hexagons.push_back(myNode);

			myNode->drop();
			myNode = nullptr;
			
			current_id++;

			current_center.X += hexagon_width;
		}
		current_center.X = first_hexagon_place.X + (row + 1) * (hexagon_width / 2.0);
		current_center.Y -= vertical_separation;
	}
}

const core::vector3d<f32> BoardView::GetBoardCenter() const
{
	core::vector3d<f32> board_center = first_hexagon_place;
	board_center.X += hexagon_width * board_dimension / 1.5;
	board_center.Y -= vertical_separation * board_dimension / 2.0;

	return board_center;
}

f32 BoardView::GetBoardWidth() const
{
	return hexagon_width * board_dimension;
}


void BoardView::RandomizeBoard() const
{
	for (auto h : hexagons)
	{
		meter aqui un randomize entre el 0 y el maximo de colores ... 
	}
}
