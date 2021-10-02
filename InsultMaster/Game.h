#pragma once

#include <string>
#include <vector>
#include "SDLIncludes.h"

class Game
{
public:
	typedef void (*RenderCallback)();
	typedef void (*ButtonDownCallback)(SDL_Event* event);
	typedef void (*ButtonUpCallback)(SDL_Event* event);

	static SDL_Window* GetWindow();
	static SDL_Surface* GetWindowSurface();
	static SDL_Texture* GetBackBuffer();
	static SDL_Renderer* GetRenderer();
	static const SDL_Rect& GetWindowRect();
	static const SDL_Rect& GetWindowScaledRect();

	static const float GetWindowScale();

	static bool Initialize(const std::string& windowName, const SDL_Rect& windowRect, const char** errorMsg, float renderScale = 1.0f);
	static void RegisterRenderCallback(RenderCallback cb);
	static void RegisterButtonDownCallback(ButtonDownCallback cb);
	static void RegisterButtonUpCallback(ButtonUpCallback cb);
	static void Render();
	static void Shutdown();
};