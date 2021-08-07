#pragma once
class Player;

class Item
{
public:
	enum class Type { None, Bomb, Laser, Orb, EnergyPowerup, RegenerationPowerup};
	Type nType;
	int nCount;
	virtual void Use(Player* p, float fX, float fY, float fAngle) = 0;
	Item()
	{
		nCount = 0;
		nType = Type::None;
	}
};

