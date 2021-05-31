#pragma once
#include "Powerup.h"
#include "Space.h"

class EnergyPowerup : public Powerup
{
public:
	EnergyPowerup(SpaceGame* sgGame);
	void Update(double deltatime) override;
};

class EnergyPowerupItem : public Item
{
public:
	EnergyPowerupItem();
	void Use(SpaceGame* game, float fX, float fY, float fAngle) override;
};