#include "HexMonteCarlo.h"

//
//// Todo el contenido transferido al .h
//
//// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
//HexMonteCarlo::HexMonteCarlo(unsigned size,
//	vector<vector<unsigned int>> &edges,
//	vector<CellState> &starting_stones,
//	unsigned int iterations,
//	unsigned int position_to_test,
//	CellState color_to_test) :
//	size(size),
//	edges(edges),
//	iterations(iterations),
//	position_to_test(position_to_test),
//	original_stone_placement(starting_stones),
//	color_to_test(color_to_test)
//{
//	stones_shuffle_base.resize(size * size);
//	unsigned int position = 0;
//	for (auto stone = stones_shuffle_base.begin(); stone < stones_shuffle_base.end(); stone++)
//	{
//		if (position % 2 == 0)
//			*stone = CellState::Blue;
//		else
//			*stone = CellState::Red;
//		position++;
//	}
//}
//
//double HexMonteCarlo::ComputeWinning()
//{
//	unsigned int player_one_victories = 0;
//	vector<CellState> randomized_stones(size * size);
//
//	for (unsigned int i = 0; i < iterations; i++)
//	{
//		ConstructRandomEndBoard(randomized_stones);
//		auto winner = ComputeWinningPlayer(randomized_stones);
//		if (winner == Player::Player_One)
//			player_one_victories++;
//	}
//
//	chances_of_victory_of_player_one = static_cast<double>(player_one_victories) / static_cast<double>(iterations);
//	return chances_of_victory_of_player_one;
//}
//
//
//Player HexMonteCarlo::ComputeWinningPlayer(const vector<CellState> &stones)
//{
//	// Testing only the red stones (player ONE)
//	//  Red player wins in horizontal connection
//	vector<unsigned int> open_positions;
//	set<unsigned int> ending_positions;
//	set<unsigned int> reachable;
//	for (unsigned int row = 0; row < size; row++)
//	{
//		if (stones[GetPositionFromCoordinate(row, 0)] == CellState::Red)
//			open_positions.push_back(GetPositionFromCoordinate(row, 0));
//		if (stones[GetPositionFromCoordinate(row, size - 1)] == CellState::Red)
//			ending_positions.insert(GetPositionFromCoordinate(row, size - 1));
//	}
//
//	// Early exit: checked it works
//	if (open_positions.empty() || ending_positions.empty())
//		return Player::Player_Two;		// Blue stones are covering any of the sides
//
//	while (!open_positions.empty())
//	{
//		unsigned int examining = open_positions.back();
//		open_positions.pop_back();
//		reachable.insert(examining);
//		// Weird cases on which the board is tiny, like size = 1, 2
//		if (ending_positions.find(examining) != ending_positions.end())
//			return Player::Player_One;
//
//		for (auto connected : edges[examining])
//		{
//			if (stones[connected] == CellState::Red)
//			{
//				if (reachable.find(connected) == reachable.end())	// newly found
//				{
//					if (ending_positions.find(connected) != ending_positions.end())
//						return Player::Player_One;	// This stone in particular is connected to an existing destination
//					open_positions.push_back(connected);
//				}
//			}
//		}
//	}
//
//	return Player::Player_Two;
//}
