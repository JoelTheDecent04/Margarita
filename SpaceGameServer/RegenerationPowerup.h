#pragma once
#include "Powerup.h"
#include "Space.h"

class RegenerationPowerup : public Powerup
{
public:
	RegenerationPowerup(std::shared_ptr<Player>& player);
	void Update(float deltatime) override;
};


class RegenerationPowerupItem : public Item
{
public:
	RegenerationPowerupItem(std::shared_ptr<Player>& player);
	void Use(float fX, float fY, float fAngle) override;
};