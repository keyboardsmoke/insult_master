#include "Game.h"
#include "Level.h"

static SDL_Color g_textColor{ 255, 255, 255 };
static SDL_Color g_borderColor{ 255, 103, 75, 255 };
static SDL_Color g_levelColor{ 102, 143, 85, 255 };

static TTF_Font* g_gameFont = nullptr;

static std::string g_textLines[2];

static void RenderBorder()
{
	const SDL_Rect& scaledRect = Game::GetWindowScaledRect();

	// Rect
	Render::ColorRect(2, 2, scaledRect.w - 4, scaledRect.h - 4, g_borderColor);

	// Accent topleft
	Render::ColorPixel(1, 1, g_borderColor);
	Render::ColorPixel(1, 2, g_borderColor);
	Render::ColorPixel(2, 1, g_borderColor);

	// Accent topright
	Render::ColorPixel(scaledRect.w - 2, 1, g_borderColor);
	Render::ColorPixel(scaledRect.w - 2, 2, g_borderColor);
	Render::ColorPixel(scaledRect.w - 3, 1, g_borderColor);

	// Accent bottomleft
	Render::ColorPixel(1, scaledRect.h - 2, g_borderColor);
	Render::ColorPixel(1, scaledRect.h - 3, g_borderColor);
	Render::ColorPixel(2, scaledRect.h - 2, g_borderColor);

	// Accent bottomright
	Render::ColorPixel(scaledRect.w - 2, scaledRect.h - 2, g_borderColor);
	Render::ColorPixel(scaledRect.w - 2, scaledRect.h - 3, g_borderColor);
	Render::ColorPixel(scaledRect.w - 3, scaledRect.h - 2, g_borderColor);
}

static void RenderTextLine(int x, int y, const std::string& str)
{
	SDL_Rect textContainmentRect{ x, y, Game::GetWindowScaledRect().w - 8, 8 };

	// ColorRect(textContainmentRect.x, textContainmentRect.y, textContainmentRect.w, textContainmentRect.h, { 0, 0, 0 });

	SDL_Surface* textSurface = TTF_RenderText_Solid(g_gameFont, str.c_str(), g_textColor);

	if (textSurface)
	{
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(Game::GetRenderer(), textSurface);

		const int x = textContainmentRect.x + (textContainmentRect.w / 2);
		const int y = textContainmentRect.y + (textContainmentRect.h / 2);
		const int w = static_cast<int>((static_cast<float>(textSurface->w * 2) / Game::GetWindowScale()));
		const int h = static_cast<int>((static_cast<float>(textSurface->h * 2) / Game::GetWindowScale())) + 1;

		SDL_Rect src = { 0, 0, textSurface->w, textSurface->h };
		SDL_Rect dest = { x - (w / 2), y - (h / 2), w, h };

		SDL_RenderCopy(Game::GetRenderer(), textTexture, &src, &dest);

		SDL_FreeSurface(textSurface);
	}
	else
	{
		printf("TTF Render Error [%s]\n", TTF_GetError());
	}
}

static void RenderText()
{
	// We never have only the second set, so this makes sense. Shush.
	if (g_textLines[0].empty())
		return;
	else
		RenderTextLine(4, 4, g_textLines[0]);

	if (!g_textLines[1].empty())
		RenderTextLine(4, 10, g_textLines[1]);
}

void Level::SetTextColor(const SDL_Color& color)
{
	g_textColor = color;
}

void Level::SetTextLine1(const std::string text)
{
	g_textLines[0] = text;
}

void Level::SetTextLine2(const std::string text)
{
	g_textLines[1] = text;
}

void Level::ClearText()
{
	g_textLines[0] = std::string();
	g_textLines[1] = std::string();
}

bool Level::Load()
{
	// We really should have a class for this..
	g_gameFont = TTF_OpenFont("../Resources/fonts/BarcadeBrawl.ttf", 18);

	if (g_gameFont == nullptr)
	{
		printf("Unable to initialize font [%s]\n", TTF_GetError());

		return false;
	}

	TTF_SetFontStyle(g_gameFont, TTF_STYLE_BOLD);

	return true;
}

void Level::RenderLevel()
{
	const SDL_Rect& windowRect = Game::GetWindowRect();

	SDL_Rect r;
	r.x = 0;
	r.y = 0;
	r.w = windowRect.w;
	r.h = windowRect.h;

	SDL_SetRenderDrawColor(Game::GetRenderer(), g_levelColor.r, g_levelColor.g, g_levelColor.b, g_levelColor.a);
	SDL_RenderFillRect(Game::GetRenderer(), &r);

	RenderBorder();
}

void Level::RenderMessage()
{
	RenderText();
}