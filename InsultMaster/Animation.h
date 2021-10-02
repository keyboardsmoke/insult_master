#pragma once

enum class eJoeAnims
{
	STANDING = 0,
	SHOW_WATCH,
	SHOW_WATCH_TALKING,
	LAUGHING_1,
	LAUGHING_2,
	STAND_TALKING,
	LAUGHING_3,
	LAUGHING_4
};

enum class eRoastieAnims
{
	STANDING = 0,
	STANDING_TALKING,
	POINTING_ANGRY_1,
	POINTING_ANGRY_2,
	STANDING_ANGRY,
	BURNED_1,
	BURNED_2,
	CHARRED_1,
	CHARRED_2,
	CHARRED_3,
};

#include "Sprite.h"
#include <vector>

class Animation
{
	struct Stage
	{
		Stage() = delete;
		Stage(unsigned int frame, unsigned int ticks) : frame(frame), ticks(ticks) {}

		unsigned int frame;
		unsigned int ticks;
	};

public:
	Animation() : m_stageTickCounter(-1), m_stages(), m_initialized(false), m_currentStage(0U) {}

	void AddStage(unsigned int frame, unsigned int ticks);
	void Progress(Sprite& sprite, int x, int y, unsigned int currentTick);

private:
	unsigned int m_stageTickCounter;

	std::vector<Stage> m_stages;

	bool m_initialized;
	unsigned int m_currentStage;
};