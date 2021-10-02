#include "Game.h"
#include "Sprite.h"

void Sprite::RenderFrame(int x, int y, int frameNumber)
{
	const int frameX = m_frameWidth * frameNumber;
	const int frameY = 0;
	const int frameW = m_frameWidth;
	const int frameH = m_texture.GetHeight();

	SDL_Rect src = { frameX, frameY, frameW, frameH };
	SDL_Rect dst = { x, y, frameW, frameH };

	SDL_RenderCopy(Game::GetRenderer(), m_texture.GetSDLTexture(), &src, &dst);
}