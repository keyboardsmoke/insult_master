#pragma once

#include "SDLIncludes.h"
#include "Game.h"

class Render
{
public:
	static void ColorPixel(int x, int y, const SDL_Color& color)
	{
		SDL_SetRenderDrawColor(Game::GetRenderer(), color.r, color.g, color.b, color.a);
		SDL_RenderDrawPoint(Game::GetRenderer(), x, y);
	}

	static void ColorLine(int x1, int y1, int x2, int y2, const SDL_Color& color)
	{
		SDL_SetRenderDrawColor(Game::GetRenderer(), color.r, color.g, color.b, color.a);
		SDL_RenderDrawLine(Game::GetRenderer(), x1, y1, x2, y2);
	}

	static void ColorRect(int x, int y, int w, int h, const SDL_Color& color)
	{
		SDL_Rect rect{ x, y, w, h };
		SDL_SetRenderDrawColor(Game::GetRenderer(), color.r, color.g, color.b, color.a);
		SDL_RenderDrawRect(Game::GetRenderer(), &rect);
	}
};