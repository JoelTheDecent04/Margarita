#pragma once
#include "Entity.h"
#include "Weapon.h"

class Orb : public Entity
{
	float fAge;
public:
	Orb(SpaceGame* game, float fX, float fY, float fSpeedX, float fSpeedY);
	void Update(double deltatime) override;
	void Collide(Entity* entity) override;
};

class OrbWeapon : public Weapon
{
public:
	OrbWeapon();
	void Use(SpaceGame* game, float fX, float fY, float fAngle) override;
};