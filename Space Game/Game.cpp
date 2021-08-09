#include "Game.h"
#include "Graphics.h"
#include "TitleScreen.h"
#include "Space.h"
#include <stdint.h>
#include <string.h>
#include <queue>
#include <mutex>
#include <atomic>
#include <thread>
#include <memory>

int32_t nScreenWidth = 1280;
int32_t nScreenHeight = 720;

int32_t nRenderTargetWidth;
int32_t nRenderTargetHeight;

float fScaleH = (float)nScreenWidth / 1280;
float fScaleV = (float)nScreenHeight / 720;

bool DoEvents();

namespace Game {
	std::shared_ptr<Level> lCurrentLevel;
	std::shared_ptr<SpaceGame> Game::sgSpaceGame;
	static bool resize = false;

	bool bHighDetail = true;

	int nClicks;

	float fSecondsSinceLastFrame = 0.0;

	const uint8_t* pKeyStates;
	int pKeyStatesLength;

	void GameMain()
	{
		SpaceGame::LoadResources();
		LoadLevel(std::make_shared<TitleScreen>());

		uint64_t nPerformanceFrequency = SDL_GetPerformanceFrequency();
		float dSecondsPerCount = 1.0f / nPerformanceFrequency;

		uint64_t nStartTime = SDL_GetPerformanceCounter();

		while (1)
		{
			if (DoEvents() == false)
				break;

			uint64_t nCurrentTime = SDL_GetPerformanceCounter();
			float dDeltaTime = (nCurrentTime - nStartTime) * dSecondsPerCount;
			nStartTime = nCurrentTime;

			pKeyStates = SDL_GetKeyboardState(&pKeyStatesLength);

			dDeltaTime /= 4.0;
			if (dDeltaTime < 0.25) //Avoid large time delta
				for (int i = 0; i < 4; i++)
					lCurrentLevel->Update(dDeltaTime);

			Graphics::BeginDraw();
			lCurrentLevel->Render();
			Graphics::EndDraw();
		}

		SpaceGame::UnloadResources();
		Quit();
	}

	void LoadLevel(const std::shared_ptr<Level>& lNewLevel)
	{
		lCurrentLevel = lNewLevel;
	}

	void LeftClick()
	{
		lCurrentLevel->LeftClick();
	}

	void KeyDown(int key)
	{
		lCurrentLevel->KeyDown(key);
	}

	void KeyPress(const char* keys)
	{
		lCurrentLevel->KeyPress(keys);
	}

	void Resize()
	{
		
	}
	void Quit()
	{
		exit(EXIT_SUCCESS);
	}
}