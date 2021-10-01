#include "Game.h"
#include "Texture.h"

Texture::~Texture()
{
	if (m_surface != nullptr)
	{
		SDL_FreeSurface(m_surface);

		m_surface = nullptr;
	}
}

bool Texture::Load()
{
	m_surface = IMG_Load(m_filename.c_str());

	if (m_surface == nullptr)
	{
		return false;
	}

	const Uint32 transKeyColor = SDL_MapRGB(m_surface->format, m_transparencyKey.r, m_transparencyKey.g, m_transparencyKey.b);

	SDL_SetColorKey(m_surface, SDL_TRUE, transKeyColor);

	m_texture = SDL_CreateTextureFromSurface(Game::GetRenderer(), m_surface);

	if (m_texture == nullptr)
	{
		return false;
	}

	if (SDL_QueryTexture(m_texture, nullptr, nullptr, &m_width, &m_height) != 0)
	{
		return false;
	}

	return false;
}

void Texture::Render(int x, int y)
{
	SDL_Rect rect = { x, y, m_width, m_height };
	SDL_RenderCopy(Game::GetRenderer(), m_texture, nullptr, &rect);
}