#pragma once
#include "Texture.h"
#include "Space.h"
#include <fstream>

class Item
{
public:
	enum class Type { None, Bomb, Laser, Orb, EnergyPowerup, RegenerationPowerup };
	Type nType;
	int nCount;
	const char* strName;
	int nTexture;
	virtual void Use(float fX, float fY, float fAngle) = 0;
	Item();
	virtual nlohmann::json Save();
	virtual void Load(nlohmann::json& f);
};