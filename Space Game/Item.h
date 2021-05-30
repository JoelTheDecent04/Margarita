#pragma once
#include "Texture.h"
#include "Space.h"

class Item
{
public:
	int nCount;
	Texture* tTexture;
	virtual void Use(SpaceGame* game, float fX, float fY, float fAngle) = 0;
	Item() { nCount = 0; tTexture = nullptr; }
};