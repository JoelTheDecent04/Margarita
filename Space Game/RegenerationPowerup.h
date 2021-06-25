#pragma once
#include "Powerup.h"
#include "Space.h"

class RegenerationPowerup : public Powerup
{
public:
	RegenerationPowerup(SpaceGame* sgGame);
	void Update(float deltatime) override;
};


class RegenerationPowerupItem : public Item
{
public:
	RegenerationPowerupItem();
	void Use(SpaceGame* game, float fX, float fY, float fAngle) override;
};