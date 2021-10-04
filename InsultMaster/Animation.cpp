#include "Game.h"
#include "Animation.h"

void Animation::AddStage(unsigned int frame, unsigned int ticks)
{
	m_stages.emplace_back(frame, ticks);
}

void Animation::AddFinishCallback(AnimationFinishedCb cb)
{
	m_callbacks.emplace_back(cb);
}

void Animation::Progress(Sprite& sprite, int x, int y, unsigned int currentTick)
{
	bool finished = m_currentStage == m_stages.size();

	// Loop
	if (!m_initialized || finished)
	{
		m_initialized = true;
		m_currentStage = 0U;
		m_stageTickCounter = currentTick;
	}

	Stage& stage = m_stages[m_currentStage];

	// Play what we've got
	const unsigned int frameNumber = stage.frame;

	sprite.RenderFrame(x, y, frameNumber);

	// We rendered the final frame of the animation
	if (finished)
	{
		for (auto& cb : m_callbacks)
		{
			cb(this);
		}
	}

	// We're not doing anything
	if ((m_stageTickCounter + stage.ticks) > currentTick)
		return;

	// We're switching to a new animation
	m_stageTickCounter = currentTick;

	++m_currentStage;
}