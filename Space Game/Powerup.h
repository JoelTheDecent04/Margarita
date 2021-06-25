#pragma once
#include "Item.h"

class Powerup
{
public:
	SpaceGame* sgGame;
	float fTimeLeft;
	Powerup(SpaceGame* game) { sgGame = game; }

	virtual void Update(float deltatime) = 0;
	void Remove();
};