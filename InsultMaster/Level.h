#pragma once

#include "Render.h"

class Level
{
public:
	static void SetTextColor(const SDL_Color& color);
	static void SetTextLine1(const std::string text);
	static void SetTextLine2(const std::string text);
	static void ClearText();
	static bool Load();
	static void RenderLevel();
	static void RenderMessage();
};