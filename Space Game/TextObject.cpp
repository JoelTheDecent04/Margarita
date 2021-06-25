#include "TextObject.h"
#include "Graphics.h"
#include "Game.h"
#include "Space.h"
#include "Colours.h"

TextObject::TextObject(float fX, float fY, const std::string& strText)
{
	fOpacity = 1.0f;
	fOpacityChangePerSecond = 0.0f;
	this->fX = fX;
	this->fY = fY;
	fSpeedX = 0.0f;
	fSpeedY = 0.0f;
	this->strText = strText;
	clrColour = &clrBlack;
	ppFont = &Graphics::pFont14Relative;
}

TextObject::TextObject() {}

bool TextObject::Update(float deltatime)
{
	fX += fSpeedX * deltatime;
	fY += fSpeedY * deltatime;
	fOpacity += fOpacityChangePerSecond * deltatime;
	if (fOpacity < 0.0f) return false;
	return true;
}

void TextObject::Draw()
{
	Graphics::WriteText(strText.c_str(), fScaleH * (fX - fBackgroundPosition), fScaleV * (fY), *ppFont, *clrColour, fOpacity);
}
