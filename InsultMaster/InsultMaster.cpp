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

Sprite roastie(RESOURCE_FOLDER "images/roastie.bmp", 32);
Sprite joe(RESOURCE_FOLDER "images/joe.bmp", 32);
Sprite fire(RESOURCE_FOLDER "images/fire.bmp", 32);
Sprite smoke(RESOURCE_FOLDER "images/smoke.bmp", 32);

// Smoke Animations
Animation smokeInvisible(true);
Animation smokeSmoking(true);

// Fire Animations
Animation fireInvisible(true);
Animation fireBurning(true);

// Roastie Animations
Animation roastieIdle(true);
Animation roastieAngryIdle(true);
Animation roastieDead(true);
Animation roastieBurned(true);

// Joe Animations
Animation joeIdle(true);
Animation joeLaughIdle(true);

// Script animations
Animation roastieHeyMan;
Animation roastieSawYou;

Animation joeYouDumb;
Animation joeMamaYouDid;
Animation joeMamaJoeMamaJoeMama;

Animation* currentRoastieAnim = &roastieHeyMan;
Animation* currentJoeAnim = &joeIdle;
Animation* currentFireAnim = &fireInvisible;
Animation* currentSmokeAnim = &smokeInvisible;

Timer g_screenMessageTimer;
Timer g_fireStartTimer;

bool g_waitingForInput = false;

unsigned int g_currentAct = static_cast<unsigned int>(eActs::HEY_MAN_YOU_STOLE_MY_WRISTWATCH);

static void RenderPlayers()
{
	// Render player effects (smoke, fire)
	currentFireAnim->Progress(fire, 9, 12, SDL_GetTicks());
	currentSmokeAnim->Progress(smoke, 8, 0, SDL_GetTicks());

	// Want to render players above their effects
	currentRoastieAnim->Progress(roastie, 8, 18, SDL_GetTicks());
	currentJoeAnim->Progress(joe, 40, 18, SDL_GetTicks());
}

static void SwitchToAct()
{
	switch (static_cast<eActs>(g_currentAct))
	{
	case eActs::YOU_DUMB_I_ALREADY_HAVE_A_WRISTWATCH:
		currentRoastieAnim = &roastieIdle; // Set him to idle, he gets angry when hes BURNED
		currentJoeAnim = &joeYouDumb;

		sounds[static_cast<unsigned int>(eAudioState::YOU_DUMB)].Play();

		break;
	case eActs::I_SAW_YOU_LOOKIN_AT_IT:
		currentRoastieAnim = &roastieSawYou;
		currentJoeAnim = &joeIdle;

		sounds[static_cast<unsigned int>(eAudioState::SAW_YOU_LOOKIN)].Play();
		break;
	case eActs::JOE_MAMA_YOU_DID:
		currentRoastieAnim = &roastieIdle;
		currentJoeAnim = &joeMamaYouDid;

		sounds[static_cast<unsigned int>(eAudioState::JOE_MAMA)].Play();
		break;
	case eActs::JOE_MAMA_JOE_MAMA_JOE_MAMA:
		currentRoastieAnim = &roastieBurned;
		currentJoeAnim = &joeMamaJoeMamaJoeMama;

		sounds[static_cast<unsigned int>(eAudioState::JOE_MAMA_X3_INCINERATION)].Play();

		g_fireStartTimer.Start(600);
		g_fireStartTimer.SetFinishedCallback([]() -> void
		{
			Level::SetTextLine1("INCINERATION!");

			currentFireAnim = &fireBurning;
		});

		break;
	case eActs::YOURE_THE_INSULT_MASTER:
		Level::SetTextColor({ 235, 230, 20, 255 });
		Level::SetTextLine1("YOU'RE THE");
		Level::SetTextLine2("INSULT MASTER!");

		sounds[static_cast<unsigned int>(eAudioState::INSULT_MASTER)].Play();

		currentJoeAnim = &joeLaughIdle;
		currentRoastieAnim = &roastieDead;
		currentFireAnim = &fireInvisible;
		currentSmokeAnim = &smokeSmoking;
		break;
	}
}

static void ButtonDown(SDL_Event* event)
{
	if (g_waitingForInput)
	{
		++g_currentAct;

		g_waitingForInput = false;

		SwitchToAct();
	}
}

static void Render()
{
	g_screenMessageTimer.Run();
	g_fireStartTimer.Run();
	Level::RenderLevel();
	RenderPlayers();
	Level::RenderMessage();
}

static void HeyManFinished(Animation* anim, void* userdata)
{
	// We need to wait for user input...
	g_waitingForInput = true;
}

static void YouDumbFinished(Animation* anim, void* userdata)
{
	// switch to burned, don't setup input yet
	Level::SetTextColor({ 255, 255, 255, 255 });
	Level::SetTextLine1("BUUUUUURNED!");
	sounds[static_cast<unsigned int>(eAudioState::BURNED)].Play();
	currentJoeAnim = &joeIdle;
	currentRoastieAnim = &roastieAngryIdle;

	g_screenMessageTimer.Start(2000);
	g_screenMessageTimer.SetFinishedCallback([]() -> void
	{
		Level::ClearText();

		++g_currentAct;

		SwitchToAct();
	});
}

static void SawYouFinished(Animation* anim, void* userdata)
{
	g_waitingForInput = true;
}

static void JoeMamaFinished(Animation* anim, void* userdata)
{
	Level::SetTextColor({ 255, 255, 255, 255 });
	Level::SetTextLine1("CLASSIC");
	Level::SetTextLine2("COMEBACK!");
	
	sounds[static_cast<unsigned int>(eAudioState::CLASSIC_COMEBACK)].Play();

	currentJoeAnim = &joeIdle;
	currentRoastieAnim = &roastieIdle;

	g_screenMessageTimer.Start(1500);
	g_screenMessageTimer.SetFinishedCallback([]() -> void
	{
		Level::ClearText();

		// We are actually waiting for input here
		g_waitingForInput = true;
	});
}

static void JoeMamaJoeMamaJoeMamaFinished(Animation* anim, void* userdata)
{
	g_screenMessageTimer.Start(2600);
	g_screenMessageTimer.SetFinishedCallback([]() -> void
	{
		Level::ClearText();

		++g_currentAct;

		SwitchToAct();
	});
}

/*

*/

static void InitializeAnimations()
{
	// Idle anims
	roastieIdle.AddStage(static_cast<unsigned int>(eRoastieAnims::STANDING), 1); // Roastie Idle
	roastieAngryIdle.AddStage(static_cast<unsigned int>(eRoastieAnims::STANDING_ANGRY), 1); // Angry standing
	roastieBurned.AddStage(static_cast<unsigned int>(eRoastieAnims::BURNED), 1); // Burning (start fire)
	roastieDead.AddStage(static_cast<unsigned int>(eRoastieAnims::CHARRED), 1); // Charred
	joeIdle.AddStage(static_cast<unsigned int>(eJoeAnims::STANDING), 1); // Joe Idle

	// Laughing loop for end game
	joeLaughIdle.AddStage(static_cast<unsigned int>(eJoeAnims::LAUGHING_1), 200);
	joeLaughIdle.AddStage(static_cast<unsigned int>(eJoeAnims::LAUGHING_2), 200);
	joeLaughIdle.AddStage(static_cast<unsigned int>(eJoeAnims::LAUGHING_3), 200);
	joeLaughIdle.AddStage(static_cast<unsigned int>(eJoeAnims::LAUGHING_4), 200);

	// Act 1
	roastieHeyMan.AddStage(static_cast<unsigned int>(eRoastieAnims::STANDING), 1000);
	roastieHeyMan.AddStage(static_cast<unsigned int>(eRoastieAnims::STANDING_TALKING), 100);
	roastieHeyMan.AddStage(static_cast<unsigned int>(eRoastieAnims::POINTING_ANGRY_1), 200);
	roastieHeyMan.AddStage(static_cast<unsigned int>(eRoastieAnims::POINTING_ANGRY_2), 200);
	roastieHeyMan.AddStage(static_cast<unsigned int>(eRoastieAnims::POINTING_ANGRY_1), 200);
	roastieHeyMan.AddStage(static_cast<unsigned int>(eRoastieAnims::POINTING_ANGRY_2), 200);
	roastieHeyMan.AddStage(static_cast<unsigned int>(eRoastieAnims::POINTING_ANGRY_1), 200);
	roastieHeyMan.AddStage(static_cast<unsigned int>(eRoastieAnims::POINTING_ANGRY_2), 200);
	roastieHeyMan.AddStage(static_cast<unsigned int>(eRoastieAnims::POINTING_ANGRY_1), 200);
	roastieHeyMan.AddStage(static_cast<unsigned int>(eRoastieAnims::POINTING_ANGRY_2), 200);
	roastieHeyMan.AddStage(static_cast<unsigned int>(eRoastieAnims::STANDING), 200);
	roastieHeyMan.AddFinishCallback(HeyManFinished);

	// Act 2
	joeYouDumb.AddStage(static_cast<unsigned int>(eJoeAnims::SHOW_WATCH), 400);
	joeYouDumb.AddStage(static_cast<unsigned int>(eJoeAnims::SHOW_WATCH_TALKING), 200);
	joeYouDumb.AddStage(static_cast<unsigned int>(eJoeAnims::SHOW_WATCH), 400);
	joeYouDumb.AddStage(static_cast<unsigned int>(eJoeAnims::SHOW_WATCH_TALKING), 200);
	joeYouDumb.AddStage(static_cast<unsigned int>(eJoeAnims::SHOW_WATCH), 400);
	joeYouDumb.AddStage(static_cast<unsigned int>(eJoeAnims::SHOW_WATCH_TALKING), 200);
	joeYouDumb.AddStage(static_cast<unsigned int>(eJoeAnims::SHOW_WATCH), 400);
	joeYouDumb.AddStage(static_cast<unsigned int>(eJoeAnims::SHOW_WATCH_TALKING), 200);
	joeYouDumb.AddStage(static_cast<unsigned int>(eJoeAnims::SHOW_WATCH), 400);
	joeYouDumb.AddFinishCallback(YouDumbFinished);

	// Act 3
	roastieSawYou.AddStage(static_cast<unsigned int>(eRoastieAnims::POINTING_ANGRY_1), 200);
	roastieSawYou.AddStage(static_cast<unsigned int>(eRoastieAnims::POINTING_ANGRY_2), 200);
	roastieSawYou.AddStage(static_cast<unsigned int>(eRoastieAnims::POINTING_ANGRY_1), 200);
	roastieSawYou.AddStage(static_cast<unsigned int>(eRoastieAnims::POINTING_ANGRY_2), 200);
	roastieSawYou.AddStage(static_cast<unsigned int>(eRoastieAnims::POINTING_ANGRY_1), 200);
	roastieSawYou.AddFinishCallback(SawYouFinished);

	// Act 4
	joeMamaYouDid.AddStage(static_cast<unsigned int>(eJoeAnims::SHRUGGING), 500);
	joeMamaYouDid.AddStage(static_cast<unsigned int>(eJoeAnims::STAND_TALKING), 400);
	joeMamaYouDid.AddStage(static_cast<unsigned int>(eJoeAnims::STANDING), 200);
	joeMamaYouDid.AddStage(static_cast<unsigned int>(eJoeAnims::STAND_TALKING), 400);
	joeMamaYouDid.AddStage(static_cast<unsigned int>(eJoeAnims::STANDING), 200);
	joeMamaYouDid.AddStage(static_cast<unsigned int>(eJoeAnims::STAND_TALKING), 400);
	joeMamaYouDid.AddStage(static_cast<unsigned int>(eJoeAnims::STANDING), 200);
	joeMamaYouDid.AddFinishCallback(JoeMamaFinished);

	// Act 5
	joeMamaJoeMamaJoeMama.AddStage(static_cast<unsigned int>(eJoeAnims::STANDING), 200);
	joeMamaJoeMamaJoeMama.AddStage(static_cast<unsigned int>(eJoeAnims::STAND_TALKING), 400);
	joeMamaJoeMamaJoeMama.AddStage(static_cast<unsigned int>(eJoeAnims::STANDING), 200);
	joeMamaJoeMamaJoeMama.AddStage(static_cast<unsigned int>(eJoeAnims::STAND_TALKING), 200);
	joeMamaJoeMamaJoeMama.AddStage(static_cast<unsigned int>(eJoeAnims::STANDING), 200);
	joeMamaJoeMamaJoeMama.AddFinishCallback(JoeMamaJoeMamaJoeMamaFinished);

	// Fire Invisible
	fireInvisible.AddStage(0, 1);

	// Fire Burning
	fireBurning.AddStage(1, 500);
	fireBurning.AddStage(2, 500);

	// Smoke Invisible
	smokeInvisible.AddStage(7, 1);

	// Smoke Smoking
	smokeSmoking.AddStage(0, 100);
	smokeSmoking.AddStage(1, 100);
	smokeSmoking.AddStage(2, 100);
	smokeSmoking.AddStage(3, 100);
	smokeSmoking.AddStage(4, 100);
	smokeSmoking.AddStage(5, 100);
	smokeSmoking.AddStage(6, 100);
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

	if (!smoke.Load())
	{
		printf("Unable to load smoke sprite [%s]\n", SDL_GetError());

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