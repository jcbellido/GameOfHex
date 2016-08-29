#pragma once

#include <json.h>

using json = nlohmann::json;

namespace KingsTest
{
	class LevelInfo
	{
	private:
		static LevelInfo * mInstance;

		int grid_size;
		float grid_start_x;
		float grid_start_y;

		float score_pos_x;
		float score_pos_y;
		float timer_pos_x;
		float timer_pos_y;

		float padding_factor;

		int number_of_colours;
		int lower_color_bound;
		int upper_color_bound;

		int generator_type;

		int timer_limit;

		int wait_limit_ms;

		LevelInfo();
		LevelInfo(LevelInfo const&);
		void operator=(LevelInfo const&);

	public:
		static LevelInfo& GetInstance();
		void Initialize(json info);

		int GetGridSize() const;
		float GetGridStartX() const;
		float GetGridStartY() const;
		float GetScorePosX() const;
		float GetScorePosY() const;
		float GetTimerPosX() const;
		float GetTimerPosY() const;

		int GetNumberOfColors() const;
		int GetLowerColorBound() const;
		int GetUpperColorBound() const;

		int GetGeneratorType() const;

		int GetTimerLimit() const;

		int GetWaitLimitMs() const;

		
		~LevelInfo();
	};
}
