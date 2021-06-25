#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include "Graphics.h"

class Texture
{
	SDL_Texture* pTexture;
public:
	int nTextureWidth = 0, nTextureHeight = 0;
	float fTextureDrawnWidth = 0.0f, fTextureDrawnHeight = 0.0f;
	int nTexturesAcross = 0;
	bool light = false;

	Texture(const char* filename);
	Texture(const char* filename, int nTextureWidth, int nTextureHeight, float fTextureDrawnWidth, float fTextureDrawnHeight);
	Texture() { pTexture = nullptr; };
	~Texture();

	void Draw();
	void Draw(int index, float x, float y, bool bRealCoordinates = false, float fAngle = 0.0f);
	void DrawDifferentSize(int index, float x, float y, float width, float height, bool bRealCoordinates = false, float fAngle = 0.0f);
	void DrawPanorama(float x);
};