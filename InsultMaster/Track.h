#pragma once

#include <string>

class Track
{
public:
	Track() : m_filename(), m_music(nullptr), m_finished(false) {}
	Track(const std::string& wav) : m_filename(wav), m_music(nullptr), m_finished(false) {}
	~Track();

	void SetFilename(const std::string& filename) { m_filename = filename; }
	const std::string& GetFilename() { return m_filename; }

	bool Load();
	void Play(double seconds = 0.0);
	void Rewind();
	void SetPosition(double seconds);
	void Pause();
	void Resume();
	bool IsDonePlaying();

private:
	std::string m_filename;

	Mix_Music* m_music;

	bool m_finished;
};