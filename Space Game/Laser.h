#pragma once
#include "Weapon.h"
#include "Entity.h"


class LaserWeapon : public Weapon
{
public:
	enum LaserLevel {
		Normal,
		DoubleShot
	};
	LaserLevel nLaserLevel;
	LaserWeapon(LaserLevel nLaserLevel);
	void Use(SpaceGame* game, float fX, float fY, float fAngle) override;
};

class LaserBeam : public Entity
{
	bool bFireSecond;
	float fSecondsUntilSecondFire;
	LaserBeam* lbNextShot;
	float fAngle;
public:
	LaserBeam(SpaceGame* game, LaserWeapon* weapon, float fX, float fY, float fSpeedX, float fSpeedY);
	void Collide(Entity* entity) override;
	void Update(double deltatime) override;
	void Draw() override;
};