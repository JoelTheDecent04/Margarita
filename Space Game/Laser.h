#pragma once
#include "Weapon.h"
#include "Entity.h"


class LaserWeapon : public Weapon
{
public:
	LaserWeapon();
	void Use(SpaceGame* game, float fX, float fY, float fAngle) override;
};

class LaserBeam : public Entity
{
	float fAngle;
public:
	LaserBeam(SpaceGame* game, float fX, float fY, float fSpeedX, float fSpeedY);
	void Collide(Entity* entity) override;
	void Update(double deltatime) override;
	void Draw() override;
};