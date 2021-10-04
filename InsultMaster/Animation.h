#pragma once

#include "Timer.h"

enum class eJoeAnims
{
	STANDING = 0,
	SHOW_WATCH,
	SHOW_WATCH_TALKING,
	LAUGHING_1,
	LAUGHING_2,
	STAND_TALKING,
	LAUGHING_3,
	LAUGHING_4,
	SHRUGGING
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
	I_SAW_YOU_LOOKIN_AT_IT,
	JOE_MAMA_YOU_DID,
	JOE_MAMA_JOE_MAMA_JOE_MAMA,
	YOURE_THE_INSULT_MASTER
};

#include "Sprite.h"
#include <vector>

class Animation;

typedef void (*AnimationFinishedCb)(Animation* anim, void* userdata);

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
	Animation(bool loop = false) : m_changeTimer(), m_stages(), m_initialized(false), m_finished(false), m_currentStage(0U), m_loop(loop) {}

	void AddStage(unsigned int frame, unsigned int ticks);
	void AddFinishCallback(AnimationFinishedCb cb, void* userdata = nullptr);
	void Progress(Sprite& sprite, int x, int y, unsigned int currentTick);

private:
	unsigned int GetLastFrame()
	{
		return m_stages.size() - 1;
	}

	bool IsFinalFrame()
	{
		return m_currentStage == GetLastFrame();
	}

	Timer m_changeTimer;

	std::vector<Stage> m_stages;

	bool m_initialized;
	bool m_finished;
	unsigned int m_currentStage;

	std::vector<std::pair<AnimationFinishedCb, void*>> m_callbacks;

	bool m_loop;
};