#pragma once
#include "Colours.h"
#include <Windows.h>
#include <d2d1.h>
#include <d2d1_1.h>
#include <cassert>
#include <dwrite.h>
#include <wrl/client.h>
#include <mutex>

struct TextSize
{
	int width, height;
};

namespace Graphics {	
	bool Initialise(HWND window);
	void Close();
	void BeginDraw();
	void EndDraw();
	void Clear(D2D1_COLOR_F& colour);
	void WriteText(const wchar_t* text, int x, int y, float size, const D2D1::ColorF& = clrWhite, float fOpacity = 1.0f, const wchar_t* font = nullptr);
	void TextMetrics(const wchar_t* text, float size, TextSize& ts, const wchar_t* font = nullptr);
	void DrawRectangle(float fX, float fY, float fW, float fH, D2D1::ColorF& cColour, float fOpacity = 1.0f, float fThickness = 1.0f);
	void FillRectangle(float fX, float fY, float fW, float fH, D2D1::ColorF& cColour, float fOpacity = 1.0f);
	void Resize();
	void DrawLighting();

	extern Microsoft::WRL::ComPtr<ID2D1DeviceContext> m_d2dContext;
	extern HWND hWindow;	
	extern std::mutex mGraphics;

	extern ID2D1DeviceContext* iLightingDeviceContext;
	extern ID2D1Bitmap1* iLightingBitmap;
}