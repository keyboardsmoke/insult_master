#include "Game.h"
#include "Track.h"

Track::~Track()
{
	if (m_music != nullptr)
	{
		Mix_FreeMusic(m_music);

		m_music = nullptr;
	}
}

bool Track::Load()
{
	m_music = Mix_LoadMUS(m_filename.c_str());

	if (m_music != nullptr)
	{
		return true;
	}

	return false;
}

void Track::Play(double seconds)
{
	if (seconds == 0.0)
		Mix_PlayMusic(m_music, 1);
	else
		Mix_FadeInMusicPos(m_music, 1, 0, seconds);
}

void Track::Rewind()
{
	Mix_RewindMusic();
}

void Track::SetPosition(double seconds)
{
	Mix_SetMusicPosition(seconds);
}

void Track::Pause()
{
	Mix_PauseMusic();
}

void Track::Resume()
{
	Mix_ResumeMusic();
}

bool Track::IsDonePlaying()
{
	return (Mix_PlayingMusic() == 0);
}