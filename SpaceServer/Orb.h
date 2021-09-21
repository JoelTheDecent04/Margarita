#pragma once
#include "Weapon.h"
#include "Player.h"

/*class OrbWeapon : public Weapon
{
public:
	OrbWeapon();
	void Use(Player* player, float fX, float fY, float fAngle) override;
};*/

class Orb : public Entity
{
	float fAge;
public:
	Orb(float fX, float fY, float fSpeedX, float fSpeedY, const std::shared_ptr<Entity>& parent = nullptr);
	bool Update(float deltatime) override;
	bool Collide(Entity* entity) override;
};