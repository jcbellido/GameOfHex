#include <LevelInfo.h>

namespace KingsTest
{
	LevelInfo & LevelInfo::GetInstance()
	{
		static LevelInfo instance;
		return instance;
	}

	void LevelInfo::Initialize(json info)
	{
		grid_size = info["grid_size"];
		timer_limit = info["timer_limit"];
		grid_start_x = info["grid_start_x"];
		grid_start_y = info["grid_start_y"];

		score_pos_x = info["score_pos_x"];
		score_pos_y = info["score_pos_y"];
		timer_pos_x = info["timer_pos_x"];
		timer_pos_y = info["timer_pos_y"];

		padding_factor = info["padding_factor"];

		number_of_colours = info["number_of_colors"];
		lower_color_bound = info["lower_color_bound"];
		upper_color_bound = info["upper_color_bound"];

		generator_type = info["generator_type"];

		wait_limit_ms = info["wait_limit_ms"];
	}

	int LevelInfo::GetGridSize() const
	{
		return grid_size;
	}

	float LevelInfo::GetGridStartX() const
	{
		return grid_start_x;
	}

	float LevelInfo::GetGridStartY() const
	{
		return grid_start_y;
	}

	float LevelInfo::GetScorePosX() const
	{
		return score_pos_x;
	}

	float LevelInfo::GetScorePosY() const
	{
		return score_pos_y;
	}

	float LevelInfo::GetTimerPosX() const
	{
		return timer_pos_x;
	}

	float LevelInfo::GetTimerPosY() const
	{
		return timer_pos_y;
	}

	int LevelInfo::GetNumberOfColors() const
	{
		return number_of_colours;
	}

	int LevelInfo::GetLowerColorBound() const
	{
		return lower_color_bound;
	}

	int LevelInfo::GetUpperColorBound() const
	{
		return upper_color_bound;
	}

	int LevelInfo::GetTimerLimit() const
	{
		return timer_limit;
	}

	int LevelInfo::GetWaitLimitMs() const
	{
		return wait_limit_ms;
	}

	int LevelInfo::GetGeneratorType() const
	{
		return generator_type;
	}

	LevelInfo::LevelInfo()
	{
	}


	LevelInfo::~LevelInfo()
	{
	}
}

