#include <time.h>
#include <cassert>
#include <thread>
#include "Game.h"
#include "Space.h"
#include "TitleScreen.h"
#include "Utilities.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

extern "C" int main(int argc, char** argv)
{
	srand(time(0));

	int ret = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	if (ret < 0) ErrorAndQuit("Couldn't initialise SDL\n");
	ret = IMG_Init(IMG_INIT_PNG);
	if (ret < 0) ErrorAndQuit("Couldn't initialise SDL Image\n");
	ret = TTF_Init();
	if (ret < 0) ErrorAndQuit("Couldn't initialise SDL TTF\n");

	

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");

	SDL_Window* pWindow = SDL_CreateWindow("Margarita 0.1.7", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, nScreenWidth, nScreenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (pWindow == nullptr)
		ErrorAndQuit("Couldn't create window: %s", SDL_GetError());

	if (Graphics::Initialise(pWindow) == false)
		ErrorAndQuit("Failed to initialise graphics: %s", SDL_GetError());

	Game::GameMain();

	return 0;
}

bool DoEvents()
{
	SDL_Event event = { 0 };

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			Game::Quit();
			SDL_Quit();
			return false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
				Game::LeftClick();
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.scancode == SDL_SCANCODE_F11)
			{
			}
			else
				Game::KeyDown(event.key.keysym.scancode);
			break;
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				nScreenWidth = event.window.data1;
				nScreenHeight = event.window.data2;
				fScaleH = (int)nScreenWidth / 1280.0f;
				fScaleV = (int)nScreenHeight / 720.0f;
				Graphics::Resize();
			}
			break;
		}
	}
	return true;
}