#pragma once
#include "Entity.h"
#include "Weapon.h"

class Bomb : public Entity
{
	float fAge;
public:
	Bomb(SpaceGame* game, float fX, float fY, float fSpeedX, float fSpeedY);
	void Collide(Entity* entity) override;
	void Explode();
	void Update(double deltatime) override;
};

class BombWeapon : public Weapon
{
public:
	BombWeapon();
	void Use(SpaceGame* game, float fX, float fY, float fAngle) override;
	
};