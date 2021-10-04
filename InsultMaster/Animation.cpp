#include "Game.h"
#include "Animation.h"

void Animation::AddStage(unsigned int frame, unsigned int ticks)
{
	m_stages.emplace_back(frame, ticks);
}

void Animation::AddFinishCallback(AnimationFinishedCb cb, void* userdata)
{
	m_callbacks.emplace_back(std::make_pair(cb, userdata));
}

void Animation::Progress(Sprite& sprite, int x, int y, unsigned int currentTick)
{
	// Some not at all needed optimizations
	if (m_stages.empty())
		return;

	if (m_stages.size() == 1)
	{
		sprite.RenderFrame(x, y, m_stages[0].frame);

		return;
	}

	if (m_finished && !m_loop)
	{
		sprite.RenderFrame(x, y, m_stages[GetLastFrame()].frame);

		return;
	}

	// Here's the meat
	unsigned int m_nextStage = m_currentStage + 1;

	if (!m_initialized)
	{
		m_initialized = true;
		m_changeTimer.Start(m_stages[m_currentStage].ticks);
	}

	Stage& stage = m_stages[m_currentStage];

	// Play what we've got
	sprite.RenderFrame(x, y, stage.frame);

	m_changeTimer.Run();

	// We're not doing anything
	if (!m_changeTimer.IsFinished())
		return;

	// We rendered the final frame of the animation
	if (IsFinalFrame())
	{
		for (auto& cb : m_callbacks)
		{
			cb.first(this, cb.second);
		}

		if (m_loop)
			m_nextStage = 0U;
		else
			m_nextStage = m_currentStage;
			m_finished = true;
	}

	// We're switching to a new animation
	m_currentStage = m_nextStage;

	m_changeTimer.Start(m_stages[m_currentStage].ticks);
}