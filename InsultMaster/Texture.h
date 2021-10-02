#pragma once

#include "SDLIncludes.h"

#include <string>

class Texture
{
public:
	Texture(const std::string& filename, const SDL_Color& transparencyKey = { 255, 0, 255, 0 }) : 
		m_filename(filename), m_transparencyKey(transparencyKey), m_texture(nullptr), m_surface(nullptr), m_width(0), m_height(0) {}

	~Texture();

	bool Load();
	void Render(int x, int y);

	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }

	SDL_Texture* GetSDLTexture() { return m_texture; }
	SDL_Surface* GetSDLSurface() { return m_surface; }

private:
	std::string m_filename;
	SDL_Color m_transparencyKey;
	SDL_Texture* m_texture;
	SDL_Surface* m_surface;
	int m_width;
	int m_height;
};