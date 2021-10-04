#pragma once

#include "SDLIncludes.h"

typedef void (*TimerFinishedCb)();

class Timer
{
public:
	Timer() : m_startTick(0u), m_finishedTick(0u), m_cb(nullptr), m_finished(true) {}

	void Start(unsigned int ms)
	{
		m_finished = false;
		m_startTick = SDL_GetTicks();
		m_finishedTick = m_startTick + ms;
	}

	void Run()
	{
		if (!m_finished)
		{
			const bool countFinished = (m_finishedTick < SDL_GetTicks());

			if (countFinished)
			{
				if (m_cb)
					m_cb();

				m_finished = true;
			}
		}
	}

	bool IsFinished()
	{
		return m_finished;
	}

	void SetFinishedCallback(TimerFinishedCb cb)
	{
		m_cb = cb;
	}

private:
	unsigned int m_startTick;
	unsigned int m_finishedTick;
	TimerFinishedCb m_cb;
	bool m_finished;
};