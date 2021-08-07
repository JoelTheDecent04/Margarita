#pragma once
#include "Space.h"
#include "Player.h"
#include <fstream>

class Item
{
public:
	enum class Type { None, Bomb, Laser, Orb, EnergyPowerup, RegenerationPowerup };
	Type nType;
	int nCount;
	const char* strName;
	std::shared_ptr<Player> plPlayer;

	virtual void Use(float fX, float fY, float fAngle) = 0;
	Item(std::shared_ptr<Player>& player);
};