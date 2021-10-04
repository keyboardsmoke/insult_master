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
	BURNED,
	CHARRED
};

enum class eActs
{
	HEY_MAN_YOU_STOLE_MY_WRISTWATCH,
	YOU_DUMB_I_ALREADY_HAVE_A_WRISTWATCH,
	BURNED,
	I_SAW_YOU_LOOKIN_AT_IT,
	JOE_MAMA_YOU_DID,
	CLASSIC_COMEBACK,
	JOE_MAMA_JOE_MAMA_JOE_MAMA,
	INCINERATION,
	YOURE_THE_INSULT_MASTER
};

#include "Sprite.h"
#include <vector>

class Animation;

typedef void (*AnimationFinishedCb)(Animation* anim);

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
	void AddFinishCallback(AnimationFinishedCb cb);
	void Progress(Sprite& sprite, int x, int y, unsigned int currentTick);

private:
	unsigned int m_stageTickCounter;

	std::vector<Stage> m_stages;

	bool m_initialized;
	unsigned int m_currentStage;

	std::vector<AnimationFinishedCb> m_callbacks;
};