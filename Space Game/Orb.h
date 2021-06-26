#pragma once
#include "Entity.h"
#include "Weapon.h"

class Orb : public Entity
{
	float fAge;
public:
	Orb(float fX, float fY, float fSpeedX, float fSpeedY);
	bool Update(float deltatime) override;
	bool Collide(Entity* entity) override;
};

class OrbWeapon : public Weapon
{
public:
	OrbWeapon();
	void Use(float fX, float fY, float fAngle) override;
};