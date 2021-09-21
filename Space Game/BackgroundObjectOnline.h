#pragma once
#include "Game.h"

#ifdef SUPPORT_MULTIPLAYER
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class BackgroundObjectOnline
{
public:
	virtual void Draw() = 0;
	virtual bool Update(float deltatime) = 0;
};

class TextObjectOnline : public BackgroundObjectOnline
{
public:
	float fOpacity;
	float fOpacityChangePerSecond;
	float fX, fY;
	float fSpeedX, fSpeedY;
	TTF_Font** ppFont;
	SDL_Color* clrColour;
	std::string strText;
	TextObjectOnline(float fX, float fY, const std::string& strText);
	TextObjectOnline();
	bool Update(float deltatime) override;
	void Draw() override;
};

class EntityHealthChangeObject : public TextObjectOnline
{

};

#endif