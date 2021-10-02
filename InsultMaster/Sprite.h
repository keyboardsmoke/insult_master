#pragma once

#include "SDLIncludes.h"
#include "Texture.h"

#include <string>

class Sprite
{
public:
	Sprite(const std::string& filename, int frameWidth, const SDL_Color& transparencyKey = { 255, 0, 255, 0 }) :
		m_filename(filename), m_frameWidth(frameWidth), m_texture(filename, transparencyKey) {}

	bool Load() { return m_texture.Load(); }
	void RenderFrame(int x, int y, int frameNumber);

private:
	std::string m_filename;

	int m_frameWidth;

	Texture m_texture;
};