#define SDL_MAIN_HANDLED
#include "SDLIncludes.h"
#include "Game.h"
#include "Texture.h"
#include "Sprite.h"
#include "Track.h"
#include <stdio.h>
#include <Windows.h>
#include <map>
#include "Animation.h"
#include "AudioScript.h"
#include "Level.h"

#define WINDOW_W 640
#define WINDOW_H 480
#define RENDER_SCALE 8.0f

//Texture joe1("../Resources/images/joe1.bmp");
Sprite roastie("../Resources/images/roastie.bmp", 32);
Sprite joe("../Resources/images/joe.bmp", 32);
Sprite fire("../Resources/images/fire.bmp", 32);

// Fire Animations
Animation fireInvisible;
Animation fireBurning;

// Roastie Animations
Animation roastieIdle;

// Joe Animations
Animation joeIdle;
Animation joeTalkingWatch;
Animation joeTalking;
Animation joeLaughing;

Animation* currentRoastieAnim = &roastieIdle;
Animation* currentJoeAnim = &joeIdle;
Animation* currentFireAnim = &fireInvisible;

static void RenderPlayers()
{
	currentFireAnim->Progress(fire, 7, 14, SDL_GetTicks());
	currentRoastieAnim->Progress(roastie, 8, 20, SDL_GetTicks());
	currentJoeAnim->Progress(joe, 40, 20, SDL_GetTicks());
}

static void ButtonDown(SDL_Event* event)
{
	// stuff
}

static void Render()
{
	Level::RenderLevel();

	RenderPlayers();
}

static void InitializeAnimations()
{
	// Roastie Idle
	roastieIdle.AddStage(static_cast<unsigned int>(eRoastieAnims::STANDING), 1000);

	// Joe Idle
	joeIdle.AddStage(static_cast<unsigned int>(eJoeAnims::STANDING), 1000);

	// Talking showing watch
	joeTalkingWatch.AddStage(static_cast<unsigned int>(eJoeAnims::SHOW_WATCH), 400);
	joeTalkingWatch.AddStage(static_cast<unsigned int>(eJoeAnims::SHOW_WATCH_TALKING), 200);

	// Joe Talking
	joeTalking.AddStage(static_cast<unsigned int>(eJoeAnims::STANDING), 400);
	joeTalking.AddStage(static_cast<unsigned int>(eJoeAnims::STAND_TALKING), 200);

	// Laughing loop
	joeLaughing.AddStage(static_cast<unsigned int>(eJoeAnims::LAUGHING_1), 200);
	joeLaughing.AddStage(static_cast<unsigned int>(eJoeAnims::LAUGHING_2), 300);
	joeLaughing.AddStage(static_cast<unsigned int>(eJoeAnims::LAUGHING_3), 200);
	joeLaughing.AddStage(static_cast<unsigned int>(eJoeAnims::LAUGHING_4), 300);

	// Fire Invisible
	fireInvisible.AddStage(0, 1000);

	// Fire Burning
	fireBurning.AddStage(1, 500);
	fireBurning.AddStage(2, 500);
}

static bool LoadResources()
{
	const char* errorMsg = nullptr;

	if (!Game::Initialize("Insult Master", { SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H }, &errorMsg, RENDER_SCALE))
	{
		printf("Unable to initialize game [%s]\n", errorMsg);

		return false;
	}

	// Initialize font subsystem
	if (TTF_Init() != 0)
	{
		printf("Unable to initialize font library [%s]\n", TTF_GetError());

		return false;
	}

	// Initialize mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) != 0)
	{
		printf("Unable to initialize mixer [%s]\n", Mix_GetError());

		return false;
	}

	// Initialize level resources
	if (!Level::Load())
	{
		printf("Failed to load level resources.\n");

		return false;
	}

	for (auto& sound : sounds)
	{
		if (!sound.second.Load())
		{
			printf("Unable to load track [%s][%s]\n", sound.second.GetFilename().c_str(), Mix_GetError());

			return false;
		}
	}

	if (!joe.Load())
	{
		printf("Unable to load joe sprite [%s]\n", SDL_GetError());

		return false;
	}

	if (!roastie.Load())
	{
		printf("Unable to load roastie sprite [%s]\n", SDL_GetError());

		return false;
	}

	if (!fire.Load())
	{
		printf("Unable to load fire sprite [%s]\n", SDL_GetError());

		return false;
	}

	InitializeAnimations();

	return true;
}

int main()
{
	Game::RegisterRenderCallback(Render);
	Game::RegisterButtonDownCallback(ButtonDown);

	if (!LoadResources())
	{
		printf("Unable to load resources...\n");

		return 1;
	}

	// Start playing? Not sure.
	sounds[static_cast<unsigned int>(eAudioState::HEY_MAN)].Play();

	// This will loop until close
	Game::Render();

	Game::Shutdown();

	return 0;
}