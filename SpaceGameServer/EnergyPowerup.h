#pragma once
#include "Powerup.h"
#include "Space.h"

class EnergyPowerup : public Powerup
{
public:
	EnergyPowerup(std::shared_ptr<Player>& player);
	void Update(float deltatime) override;
};

class EnergyPowerupItem : public Item
{
public:
	EnergyPowerupItem(std::shared_ptr<Player>& player);
	void Use(float fX, float fY, float fAngle) override;
};