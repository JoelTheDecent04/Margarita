#include <Windows.h>
#include <d2d1.h>
#include <d2d1_1.h>
#include <cassert>
#include <dwrite.h>
#include "Game.h"

namespace Graphics {

	static ID2D1Factory1* iFactory;
	ID2D1HwndRenderTarget* iRenderTarget;
	static IDWriteFactory* iWriteFactory;
	static ID2D1SolidColorBrush* iSolidColourBrush;
	ID2D1DeviceContext* iDeviceContext;
	ID2D1Bitmap1* iTargetBitmap;

	//static IDWriteTextFormat* iTextFormat;
	HWND hWindow;

	D2D1::ColorF clrWhite = D2D1::ColorF(1.0f, 1.0f, 1.0f);

	bool Initialise(HWND window)
	{
		hWindow = window;
		HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, __uuidof(ID2D1Factory1), nullptr, (void**)&iFactory); //D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &iFactory); //Single threaded direct2d factory
		assert(SUCCEEDED(hr));
		if (FAILED(hr)) return false;

		RECT rClientRect;
		GetClientRect(window, &rClientRect); //Get window area

		hr = iFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(), //Default properties
			D2D1::HwndRenderTargetProperties(window, D2D1::SizeU(rClientRect.right, rClientRect.bottom)),
			&iRenderTarget);
		assert(SUCCEEDED(hr));
		if (FAILED(hr)) return 0;

		hr = iRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f), &iSolidColourBrush);
		assert(SUCCEEDED(hr));

		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&iWriteFactory);
		assert(SUCCEEDED(hr));
		if (FAILED(hr)) return 0;

		D2D1_ANTIALIAS_MODE mode = iRenderTarget->GetAntialiasMode();

		nRenderTargetWidth = rClientRect.right;
		nRenderTargetHeight = rClientRect.bottom;

		return true;
	}

	void Close()
	{
		if (iFactory) iFactory->Release();
		if (iRenderTarget) iRenderTarget->Release();
		if (iWriteFactory) iWriteFactory->Release();
	}

	void BeginDraw()
	{
		iRenderTarget->BeginDraw();
	}

	void EndDraw()
	{
		iRenderTarget->EndDraw();
	}

	void Clear(D2D1_COLOR_F& colour)
	{
		iRenderTarget->Clear(colour);
	}

	void WriteText(const wchar_t* text, int x, int y, float size, const D2D1::ColorF& clrColour, float fOpacity)
	{
		if (size == 0.0f) return;
		IDWriteTextFormat* iTextFormat;
		HRESULT hr = iWriteFactory->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,  size, L"en-US", &iTextFormat);
		assert(SUCCEEDED(hr));

		iSolidColourBrush->SetColor(clrColour);
		iSolidColourBrush->SetOpacity(fOpacity);

		RECT rClientRect;
		GetClientRect(hWindow, &rClientRect);

		iRenderTarget->DrawTextW(text, lstrlenW(text), iTextFormat, D2D1::RectF( x,  y,  rClientRect.right,  rClientRect.bottom), iSolidColourBrush);
		iSolidColourBrush->SetOpacity(fOpacity);
		iTextFormat->Release();
	}

	void TextMetrics(const wchar_t* text, float size, DWRITE_TEXT_METRICS& textmetrics)
	{
		if (size == 0.0f) return;
		IDWriteTextFormat* iTextFormat;
		HRESULT hr = iWriteFactory->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL, size, L"en-US", &iTextFormat);
		assert(SUCCEEDED(hr));

		RECT rClientRect;
		bool b = GetClientRect(hWindow, &rClientRect);
		assert(b);

		IDWriteTextLayout* iTextLayout;
		hr = iWriteFactory->CreateTextLayout(text, lstrlen(text), iTextFormat, rClientRect.right, rClientRect.bottom, &iTextLayout);
		assert(SUCCEEDED(hr));

		hr = iTextLayout->GetMetrics(&textmetrics);
		assert(SUCCEEDED(hr));

		iTextLayout->Release();
		iTextFormat->Release();
	}
	
	void DrawRectangle(float fX, float fY, float fW, float fH, D2D1::ColorF& cColour, float fOpacity)
	{
		iSolidColourBrush->SetColor(cColour);
		iSolidColourBrush->SetOpacity(fOpacity);
		D2D1_RECT_F rRect = D2D1::RectF( fX,  fY,  (fX + fW),  (fY + fH));
		iRenderTarget->DrawRectangle(rRect, iSolidColourBrush);
		iSolidColourBrush->SetOpacity(1.0f);
	}

	void FillRectangle(float fX, float fY, float fW, float fH, D2D1::ColorF& cColour, float fOpacity)
	{
		iSolidColourBrush->SetColor(cColour);
		iSolidColourBrush->SetOpacity(fOpacity);
		D2D1_RECT_F rRect = D2D1::RectF( fX,  fY,  (fX + fW),  (fY + fH));
		iRenderTarget->FillRectangle(rRect, iSolidColourBrush);
		iSolidColourBrush->SetOpacity(1.0f);
	}

	void Resize()
	{
		if (iRenderTarget)
		{
			RECT rcClientRect;
			GetClientRect(hWindow, &rcClientRect);
			D2D1_SIZE_U size = D2D1::SizeU(rcClientRect.right, rcClientRect.bottom);
			HRESULT hr = iRenderTarget->Resize(size);
			nScreenWidth = rcClientRect.right;
			nScreenHeight = rcClientRect.bottom;
			nRenderTargetWidth = rcClientRect.right;
			nRenderTargetHeight = rcClientRect.bottom;
			fScaleV = (float)nScreenHeight / 720.0f;
			fScaleH = (float)nScreenWidth / 1280.0f;
		}
	}

}