#include "GUI.h"
#include "Graphics.h"
#include "Game.h"

static D2D1::ColorF clrDarkGrey = D2D1::ColorF::DarkGray;
static D2D1::ColorF clrDarkerGrey = D2D1::ColorF(0.15f, 0.15f, 0.15f);
static D2D1::ColorF clrWhite = D2D1::ColorF::White;
static D2D1::ColorF clrBlack = D2D1::ColorF(0.0f, 0.0f, 0.0f);
static D2D1::ColorF clrRed = D2D1::ColorF::Red;

Bar::Bar(int left, int top, int right, int bottom, D2D1::ColorF& clrColour, float* fValue, float* fMaxValue)
	: clrColour(clrColour)
{
	this->rect = { left, top, right, bottom }; this->fValue = fValue; this->fMaxValue = fMaxValue;
}

void Bar::Draw()
{
	Graphics::FillRectangle(fScaleH * rect.left, fScaleV * rect.top, fScaleH * ((*fValue / *fMaxValue) * (rect.right - rect.left)), fScaleV * (rect.bottom - rect.top), clrColour);
	Graphics::DrawRectangle(fScaleH * rect.left, fScaleV * rect.top, fScaleH * (rect.right - rect.left), fScaleV * (rect.bottom - rect.top), clrDarkGrey);
}

Button::Button(int left, int top, int right, int bottom, void (*function)(void*), const wchar_t* text, float fSize)
{
	this->rect = { left, top, right, bottom };
	this->function = function;
	this->text = text;
	this->fSize = fSize;
	bClickable = true;
};

void Button::Click(void* p)
{
	function(p);
}

void Button::Draw(bool bHover)
{
	Graphics::FillRectangle(fScaleH * rect.left, fScaleV * rect.top, fScaleH * (rect.right - rect.left), fScaleV * (rect.bottom - rect.top), bClickable ? (bHover ? clrDarkGrey : clrBlack) : clrDarkerGrey);
	Graphics::DrawRectangle(fScaleH * rect.left, fScaleV * rect.top, fScaleH * (rect.right - rect.left), fScaleV * (rect.bottom - rect.top), clrDarkGrey);
	DWRITE_TEXT_METRICS tmTextMetrics;
	Graphics::TextMetrics(text, fScaleV * fSize, tmTextMetrics);
	Graphics::WriteText(text,
		fScaleH * ((rect.right + rect.left) / 2) - tmTextMetrics.width / 2,
		fScaleV * ((rect.bottom + rect.top) / 2) - tmTextMetrics.height / 2,
		fScaleV * fSize);
}