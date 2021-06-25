#include "GUI.h"
#include "Graphics.h"
#include "Game.h"
#include "Colours.h"

Bar::Bar(int left, int top, int right, int bottom, SDL_Color clrColour, float* fValue, float* fMaxValue)
	: clrColour(clrColour)
{
	this->rect = { left, top, right, bottom }; this->fValue = fValue; this->fMaxValue = fMaxValue;
}

void Bar::Draw()
{
	Graphics::FillRectangle(fScaleH * rect.left, fScaleV * rect.top, fScaleH * ((*fValue / *fMaxValue) * (rect.right - rect.left)), fScaleV * (rect.bottom - rect.top), clrColour);
	Graphics::DrawRectangle(fScaleH * rect.left, fScaleV * rect.top, fScaleH * (rect.right - rect.left), fScaleV * (rect.bottom - rect.top), clrDarkGrey);
}

Button::Button(int left, int top, int right, int bottom, void (*function)(void*), const char* text, TTF_Font** font)
{
	this->rect = { left, top, right, bottom };
	this->function = function;
	this->text = text;
	this->font = font;
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
	TextSize textsize;
	Graphics::TextMetrics(text, *font, textsize);
	Graphics::WriteText(text,
		fScaleH * ((rect.right + rect.left) / 2) - textsize.width / 2,
		fScaleV * ((rect.bottom + rect.top) / 2) - textsize.height / 2,
		*font);
}