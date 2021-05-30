#pragma once
#include "BackgroundObject.h"
#include <string>
#include <d2d1.h>

class TextObject : public BackgroundObject
{
public:
	float fOpacity;
	float fOpacityChangePerSecond;
	float fX, fY;
	float fSpeedX, fSpeedY;
	float fSize;
	D2D1::ColorF* clrColour;
	std::wstring strText;
	TextObject(float fX, float fY, const std::wstring& strText);
	TextObject();
	void Update(double deltatime) override;
	void Draw() override;
};