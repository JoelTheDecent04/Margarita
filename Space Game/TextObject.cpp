#include "TextObject.h"
#include "Graphics.h"
#include "Game.h"
#include "Space.h"

static D2D1::ColorF clrBlack = D2D1::ColorF(0.0f, 0.0f, 0.0f);

TextObject::TextObject(float fX, float fY, const std::wstring& strText)
{
	fOpacity = 1.0f;
	fOpacityChangePerSecond = 0.0f;
	this->fX = fX;
	this->fY = fY;
	fSpeedX = 0.0f;
	fSpeedY = 0.0f;
	this->strText = strText;
	clrColour = &clrBlack;
	fSize = 14.0f;
}

TextObject::TextObject() {}

void TextObject::Update(double deltatime)
{
	fX += fSpeedX * deltatime;
	fY += fSpeedY * deltatime;
	fOpacity += fOpacityChangePerSecond * deltatime;
	if (fOpacity < 0.0f) fOpacity = 0.0f;
}

void TextObject::Draw()
{
	Graphics::WriteText(strText.c_str(), fScaleH * (fX - fBackgroundPosition), fScaleV * (fY), fScaleV * fSize, *clrColour, fOpacity);
}
