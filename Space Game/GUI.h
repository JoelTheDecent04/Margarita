#pragma once
#include "Utilities.h"
#include "Graphics.h"
#include <SDL.h>
#include <SDL_ttf.h>

class Bar //TODO add horizontal / vertical
{
public:
	Rect rect;
	SDL_Color clrColour;
	float* fValue;
	float* fMaxValue;
	Bar(int left, int top, int right, int bottom, SDL_Color clrColour, float* fValue, float* fMaxValue);
	void Draw();
};

class Button
{
public:
	Rect rect;
	void (*function)(void*);
	const char* text;
	TTF_Font** font;
	bool bClickable;
	Button(int left, int top, int right, int bottom, void (*function)(void*), const char* text, TTF_Font** font = &Graphics::pFont24Relative);
	void Click(void* p);
	void Draw(bool bHover);
};