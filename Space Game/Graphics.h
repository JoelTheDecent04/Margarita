#pragma once
#include "Colours.h"
#include <cassert>
#include <mutex>
#include <SDL.h>
#include <SDL_ttf.h>

struct TextSize
{
	int width, height;
};

namespace Graphics {	
	bool Initialise(SDL_Window* window);
	void Close();
	void BeginDraw();
	void EndDraw();
	void Clear(SDL_Color colour);
	void WriteText(const char* text, int x, int y, TTF_Font* pFont, SDL_Color clrColour = clrWhite, float fOpacity = 1.0f);
	void TextMetrics(const char* text, TTF_Font* pFont, TextSize& ts);
	void DrawRectangle(float fX, float fY, float fW, float fH, SDL_Color cColour, float fOpacity = 1.0f, float fThickness = 1.0f);
	void FillRectangle(float fX, float fY, float fW, float fH, SDL_Color cColour, float fOpacity = 1.0f);
	void Resize();
	void DrawLighting();

	extern SDL_Renderer* pRenderer;
	extern SDL_Window* pWindow;

	extern TTF_Font* pFont12;
	extern TTF_Font* pFont14;
	extern TTF_Font* pFont16;
	extern TTF_Font* pFont24;

	extern TTF_Font* pFont9Relative;
	extern TTF_Font* pFont14Relative;
	extern TTF_Font* pFont16Relative;
	extern TTF_Font* pFont20Relative;
	extern TTF_Font* pFont24Relative;
	extern TTF_Font* pFont36Relative;
	extern TTF_Font* pFont44Relative;
	extern TTF_Font* pFont125Relative;
}