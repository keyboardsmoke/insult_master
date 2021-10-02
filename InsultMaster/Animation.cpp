#include "Game.h"
#include "Animation.h"

void Animation::AddStage(unsigned int frame, unsigned int ticks)
{
	m_stages.emplace_back(frame, ticks);
}

void Animation::Progress(Sprite& sprite, int x, int y, unsigned int currentTick)
{
	// Loop
	if (!m_initialized || m_currentStage == m_stages.size())
	{
		m_initialized = true;
		m_currentStage = 0U;
		m_stageTickCounter = currentTick;
	}

	Stage& stage = m_stages[m_currentStage];

	// Play what we've got
	const unsigned int frameNumber = stage.frame;

	sprite.RenderFrame(x, y, frameNumber);

	// We're not doing anything
	if ((m_stageTickCounter + stage.ticks) > currentTick)
		return;

	// We're switching to a new animation
	m_stageTickCounter = currentTick;

	++m_currentStage;
}