#pragma once
#include <d2d1.h>
#include <wincodec.h>
#include <Windows.h>
#include "Graphics.h"

class Texture
{
	ID2D1Bitmap* iBitmap;
public:
	int nTextureWidth, nTextureHeight;
	float fTextureDrawnWidth, fTextureDrawnHeight;
	int nTexturesAcross;

	Texture(const wchar_t* filename);
	Texture(const wchar_t* filename, int nTextureWidth, int nTextureHeight, float fTextureDrawnWidth, float fTextureDrawnHeight);
	Texture() { iBitmap = nullptr; };
	~Texture();

	void Draw();
	void Draw(int index, float x, float y, bool bRealCoordinates = false);
	void DrawPanorama(int x);
};