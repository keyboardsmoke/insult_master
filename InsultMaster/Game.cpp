#include "Game.h"

SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;
SDL_Surface* g_windowSurface = nullptr;
SDL_Texture* g_backBuffer = nullptr;
SDL_Rect g_windowRect{};
SDL_Rect g_windowScaledRect{};
uint8_t* g_bbPixels = nullptr;
int g_bbPitch = 0;
float g_renderScale = 0.0f;

std::vector<Game::RenderCallback> g_renderCallbacks;
std::vector<Game::ButtonDownCallback> g_buttonDownCallbacks;
std::vector<Game::ButtonUpCallback> g_buttonUpCallbacks;

SDL_Window* Game::GetWindow()
{
	return g_window;
}

SDL_Surface* Game::GetWindowSurface()
{
	return g_windowSurface;
}

SDL_Texture* Game::GetBackBuffer()
{
	return g_backBuffer;
}

SDL_Renderer* Game::GetRenderer()
{
	return g_renderer;
}

const SDL_Rect& Game::GetWindowRect()
{
	return g_windowRect;
}

const SDL_Rect& Game::GetWindowScaledRect()
{
	return g_windowScaledRect;
}

const float Game::GetWindowScale()
{
	return g_renderScale;
}

bool Game::Initialize(const std::string& windowName, const SDL_Rect& windowRect, const char** errorMsg, float renderScale)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		*errorMsg = SDL_GetError();

		return false;
	}

	g_window = SDL_CreateWindow(windowName.c_str(), // creates a window 
		windowRect.x, windowRect.y, windowRect.w, windowRect.h, 0);

	if (g_window == nullptr)
	{
		*errorMsg = SDL_GetError();

		return false;
	}

	Uint32 render_flags = SDL_RENDERER_ACCELERATED;

	g_renderer = SDL_CreateRenderer(g_window, -1, render_flags);

	if (g_renderer == nullptr)
	{
		*errorMsg = SDL_GetError();

		return false;
	}

	g_windowSurface = SDL_GetWindowSurface(g_window);

	if (g_windowSurface == nullptr)
	{
		*errorMsg = SDL_GetError();

		return false;
	}

	// 64 x 48 now lol
	SDL_RenderSetScale(g_renderer, renderScale, renderScale);

	g_renderScale = renderScale;

	memcpy(&g_windowRect, &windowRect, sizeof(SDL_Rect));

	g_windowScaledRect.x = g_windowRect.x;
	g_windowScaledRect.y = g_windowRect.y;
	g_windowScaledRect.w = g_windowRect.w / static_cast<int>(renderScale);
	g_windowScaledRect.h = g_windowRect.h / static_cast<int>(renderScale);

	return true;
}

void Game::RegisterRenderCallback(RenderCallback cb)
{
	g_renderCallbacks.emplace_back(cb);
}

void Game::RegisterButtonDownCallback(ButtonDownCallback cb)
{
	g_buttonDownCallbacks.emplace_back(cb);
}

void Game::RegisterButtonUpCallback(ButtonUpCallback cb)
{
	g_buttonUpCallbacks.emplace_back(cb);
}

void Game::Render()
{
	int close = 0;

	while (!close)
	{
		SDL_Event event;

		// Events mangement 
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				close = 1;
				break;
			case SDL_KEYDOWN:
			case SDL_JOYBUTTONDOWN:
			case SDL_CONTROLLERBUTTONDOWN:
			case SDL_FINGERDOWN:
				for (auto& cb : g_buttonDownCallbacks)
				{
					cb(&event);
				}

				break;
			case SDL_KEYUP:
			case SDL_JOYBUTTONUP:
			case SDL_CONTROLLERBUTTONUP:
			case SDL_FINGERUP:
				for (auto& cb : g_buttonUpCallbacks)
				{
					cb(&event);
				}

				break;
			}
		}

		SDL_LockTexture(g_backBuffer, nullptr, (void**)&g_bbPixels, &g_bbPitch);

		SDL_UnlockTexture(g_backBuffer);

		SDL_RenderClear(g_renderer);

		for (auto& r : g_renderCallbacks)
		{
			r();
		}

		SDL_RenderPresent(g_renderer);

		// Force 60fps
		SDL_Delay(1000 / 60);
	}
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(g_renderer);

	g_renderer = nullptr;

	SDL_DestroyWindow(g_window);

	g_window = nullptr;
}