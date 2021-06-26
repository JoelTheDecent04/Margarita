#include <cassert>
#include <mutex>
#include <SDL.h>
#include <SDL_Image.h>
#include <SDL_ttf.h>
#include <SDL_rect.h>
#include "Game.h"
#include "Graphics.h"
#include "Colours.h"
#include "Utilities.h"

namespace Graphics {
	
	bool init;
	
	SDL_Window* pWindow;
	SDL_Renderer* pRenderer;

	TTF_Font* pFont12;
	TTF_Font* pFont14;
	TTF_Font* pFont16;
	TTF_Font* pFont24;

	TTF_Font* pFont9Relative;
	TTF_Font* pFont14Relative;
	TTF_Font* pFont16Relative;
	TTF_Font* pFont20Relative;
	TTF_Font* pFont24Relative;
	TTF_Font* pFont36Relative;
	TTF_Font* pFont44Relative;
	TTF_Font* pFont125Relative;
	
#define _LOCAL_FONT

#ifndef _LOCAL_FONT
#ifdef _WIN32 
#define __FONT_FOUND__
	static const char* strFontFile = "C:\\Windows\\Fonts\\bahnschrift.ttf";
#endif
#ifdef __gnu_linux__
#define __FONT_FOUND__
	static const char* strFontFile = "/usr/share/fonts/truetype/FreeSans.ttf";
#endif
#ifndef __FONT_FOUND__
		static const char* strFontFile = "resources/Satella.ttf";
#endif
#else
	static const char* strFontFile = "resources/TitilliumWeb-Regular.ttf";
#endif


	bool Initialise(SDL_Window* w)
	{
		pWindow = w;

		pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); //SDL_RENDERER_PRESENTVSYNC
		if (pRenderer == nullptr)
		{
			ErrorAndQuit("Failed to create renderer: %s", SDL_GetError());
			return false;
		}

		SDL_Surface* pWindowIconSurface = IMG_Load("resources/icon.png");
		if (pWindowIconSurface)
		{
			SDL_SetWindowIcon(pWindow, IMG_Load("resources/icon.png"));
			SDL_FreeSurface(pWindowIconSurface);
		}

		if (SDL_SetRenderDrawBlendMode(pRenderer, SDL_BLENDMODE_BLEND) != 0)
			SDL_Log("Failed to set render draw blend mode: %s", SDL_GetError());
		
		pFont12 = TTF_OpenFont(strFontFile, 12);
		if (pFont12 == nullptr)
			SDL_Log("Failed to open font file: %s: %s\n", strFontFile, SDL_GetError());
		pFont14 = TTF_OpenFont(strFontFile, 14);
		pFont16 = TTF_OpenFont(strFontFile, 16);
		pFont24 = TTF_OpenFont(strFontFile, 24);

		pFont9Relative = TTF_OpenFont(strFontFile, (int)(fScaleV * 9));
		pFont14Relative = TTF_OpenFont(strFontFile, (int)(fScaleV * 14));
		pFont16Relative = TTF_OpenFont(strFontFile, (int)(fScaleV * 16));
		pFont20Relative = TTF_OpenFont(strFontFile, (int)(fScaleV * 20));
		pFont24Relative = TTF_OpenFont(strFontFile, (int)(fScaleV * 24));
		pFont36Relative = TTF_OpenFont(strFontFile, (int)(fScaleV * 36));
		pFont44Relative = TTF_OpenFont(strFontFile, (int)(fScaleV * 44));
		pFont125Relative = TTF_OpenFont(strFontFile, (int)(fScaleV * 125));	

		init = true;
		return true;
	}

	void Close()
	{
		//TODO cleanup
	}

	void BeginDraw()
	{
	}

	void EndDraw()
	{
		SDL_RenderPresent(pRenderer);
	}

	void Clear(SDL_Color clrColour)
	{
		SDL_SetRenderDrawColor(pRenderer, clrColour.r, clrColour.b, clrColour.g, clrColour.a);
		SDL_RenderClear(pRenderer);
	}

	void WriteText(const char* text, int x, int y, TTF_Font* pFont, SDL_Color clrColour, float fOpacity)
	{
		if (pFont == nullptr) return;
		SDL_Surface* pTextSurface = TTF_RenderText_Blended_Wrapped(pFont, text, clrColour, nScreenWidth);
		if (pTextSurface == nullptr) return;
		SDL_Texture* pTextTexture = SDL_CreateTextureFromSurface(pRenderer, pTextSurface);
		if (pTextTexture == nullptr) return;
		int nWidth, nHeight;
		SDL_QueryTexture(pTextTexture, nullptr, nullptr, &nWidth, &nHeight);
		SDL_Rect rDest = { x, y, nWidth, nHeight}; //TODO fix
		SDL_RenderCopy(pRenderer, pTextTexture, nullptr, &rDest);

		SDL_FreeSurface(pTextSurface);
		SDL_DestroyTexture(pTextTexture);
	}

	void TextMetrics(const char* text, TTF_Font* pFont, TextSize& ts)
	{
		if (pFont == nullptr)
		{
			ts.height = 0;
			ts.width = 0;
			return;
		}

		TTF_SizeText(pFont, text, &ts.width, &ts.height);
	}
	
	void DrawRectangle(float fX, float fY, float fW, float fH, SDL_Color cColour, float fOpacity, float thickness)
	{
		uint8_t nOpacity = fOpacity * 255.0f;
		SDL_SetRenderDrawColor(pRenderer, cColour.r, cColour.g, cColour.b, nOpacity);
		SDL_FRect rRect = { fX, fY, fW, fH }; //TODO float to int conversion, thickness
		SDL_RenderDrawRectF(pRenderer, &rRect);
	}

	void FillRectangle(float fX, float fY, float fW, float fH, SDL_Color cColour, float fOpacity)
	{
		uint8_t nOpacity = fOpacity * 255.0f;
		SDL_SetRenderDrawColor(pRenderer, cColour.r, cColour.g, cColour.b, nOpacity);
		SDL_FRect rRect = { fX, fY, fW, fH }; //TODO float to int conversion, opacity
		SDL_RenderFillRectF(pRenderer, &rRect);
	}

	void Resize()
	{
		TTF_CloseFont(pFont9Relative);
		TTF_CloseFont(pFont14Relative);
		TTF_CloseFont(pFont16Relative); 
		TTF_CloseFont(pFont20Relative);
		TTF_CloseFont(pFont24Relative);
		TTF_CloseFont(pFont36Relative);
		TTF_CloseFont(pFont44Relative);
		TTF_CloseFont(pFont125Relative);

		pFont9Relative = TTF_OpenFont(strFontFile, (int)(fScaleV * 9));
		pFont14Relative = TTF_OpenFont(strFontFile, (int)(fScaleV * 14));
		pFont16Relative = TTF_OpenFont(strFontFile, (int)(fScaleV * 16));
		pFont20Relative = TTF_OpenFont(strFontFile, (int)(fScaleV * 20));
		pFont24Relative = TTF_OpenFont(strFontFile, (int)(fScaleV * 24));
		pFont36Relative = TTF_OpenFont(strFontFile, (int)(fScaleV * 36));
		pFont44Relative = TTF_OpenFont(strFontFile, (int)(fScaleV * 44));
		pFont125Relative = TTF_OpenFont(strFontFile, (int)(fScaleV * 125));
	}

	void DrawLighting()
	{
		
		
	}

}