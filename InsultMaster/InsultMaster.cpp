#define SDL_MAIN_HANDLED
#include "SDLIncludes.h"
#include "Game.h"
#include "Texture.h"
#include <stdio.h>

#define WINDOW_W 640
#define WINDOW_H 480
#define RENDER_SCALE 8.0f

SDL_Color g_borderColor{ 255, 103, 75, 255 };
SDL_Color g_levelColor{ 102, 143, 85, 255 };

Texture joe1("../Resources/images/joe1.bmp");
Texture sht1("../Resources/images/watch1.bmp");

TTF_Font* g_gameFont = nullptr;

void ColorPixel(int x, int y, const SDL_Color& color)
{
	SDL_SetRenderDrawColor(Game::GetRenderer(), color.r, color.g, color.b, color.a);
	SDL_RenderDrawPoint(Game::GetRenderer(), x, y);
}

void ColorLine(int x1, int y1, int x2, int y2, const SDL_Color& color)
{
	SDL_SetRenderDrawColor(Game::GetRenderer(), color.r, color.g, color.b, color.a);
	SDL_RenderDrawLine(Game::GetRenderer(), x1, y1, x2, y2);
}

void ColorRect(int x, int y, int w, int h, const SDL_Color& color)
{
	SDL_Rect rect { x, y, w, h };
	SDL_SetRenderDrawColor(Game::GetRenderer(), color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(Game::GetRenderer(), &rect);
}

void RenderBorder()
{
	const int scaleW = WINDOW_W / static_cast<int>(RENDER_SCALE);
	const int scaleH = WINDOW_H / static_cast<int>(RENDER_SCALE);

	// Rect
	ColorRect(2, 2, scaleW - 4, scaleH - 4, g_borderColor);

	// Accent topleft
	ColorPixel(1, 1, g_borderColor);
	ColorPixel(1, 2, g_borderColor);
	ColorPixel(2, 1, g_borderColor);

	// Accent topright
	ColorPixel(scaleW - 2, 1, g_borderColor);
	ColorPixel(scaleW - 2, 2, g_borderColor);
	ColorPixel(scaleW - 3, 1, g_borderColor);

	// Accent bottomleft
	ColorPixel(1, scaleH - 2, g_borderColor);
	ColorPixel(1, scaleH - 3, g_borderColor);
	ColorPixel(2, scaleH - 2, g_borderColor);

	// Accent bottomright
	ColorPixel(scaleW - 2, scaleH - 2, g_borderColor);
	ColorPixel(scaleW - 2, scaleH - 3, g_borderColor);
	ColorPixel(scaleW - 3, scaleH - 2, g_borderColor);
}

void RenderText()
{
	SDL_Rect textContainmentRect{ 4, 4, Game::GetWindowScaledRect().w - 8, 8 };
	
	// ColorRect(textContainmentRect.x, textContainmentRect.y, textContainmentRect.w, textContainmentRect.h, { 0, 0, 0 });

	SDL_Color textColor{ 255, 255, 255 };

	SDL_Surface* textSurface = TTF_RenderText_Solid(g_gameFont, "BUUUUUURNED!", textColor);

	// TTF_SizeText()

	if (textSurface)
	{
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(Game::GetRenderer(), textSurface);

		const int x = textContainmentRect.x + (textContainmentRect.w / 2);
		const int y = textContainmentRect.y + (textContainmentRect.h / 2);
		const int w = ((textSurface->w * 2) / RENDER_SCALE);
		const int h = ((textSurface->h * 2) / RENDER_SCALE) + 1;

		SDL_Rect src = { 0, 0, textSurface->w, textSurface->h };
		SDL_Rect dest = { x - (w / 2), y - (h / 2), w, h};

		SDL_RenderCopy(Game::GetRenderer(), textTexture, &src, &dest);

		SDL_FreeSurface(textSurface);
	}
	else
	{
		printf("TTF Render Error [%s]\n", TTF_GetError());
	}
}

void RenderLevel()
{
	SDL_Rect r;
	r.x = 0;
	r.y = 0;
	r.w = WINDOW_W;
	r.h = WINDOW_H;

	SDL_SetRenderDrawColor(Game::GetRenderer(), g_levelColor.r, g_levelColor.g, g_levelColor.b, g_levelColor.a);
	SDL_RenderFillRect(Game::GetRenderer(), &r);

	RenderBorder();

	RenderText();
}

void RenderPlayers()
{
	joe1.Render(44, 24);
	sht1.Render(-2, 24);
}

int main()
{
	const char* errorMsg = nullptr;

	Game::RegisterRenderCallback(RenderLevel);
	Game::RegisterRenderCallback(RenderPlayers);

	if (!Game::Initialize("Insult Master", { SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H }, &errorMsg, RENDER_SCALE))
	{
		printf("Unable to initialize game [%s]\n", errorMsg);

		return 1;
	}

	if (TTF_Init() != 0)
	{
		printf("Unable to initialize font library [%s]\n", TTF_GetError());

		return 1;
	}

	g_gameFont = TTF_OpenFont("../Resources/fonts/BarcadeBrawl.ttf", 18);

	if (g_gameFont == nullptr)
	{
		printf("Unable to initialize font [%s]\n", TTF_GetError());

		return 1;
	}

	TTF_SetFontStyle(g_gameFont, TTF_STYLE_BOLD);

	joe1.Load();
	sht1.Load();

	// This will loop until close
	Game::Render();

	Game::Shutdown();

	return 0;
}