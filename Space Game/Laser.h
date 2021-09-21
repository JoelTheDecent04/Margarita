#pragma once
#include "Weapon.h"
#include "Entity.h"


class LaserWeapon : public Weapon
{
public:
	enum LaserLevel {
		Normal = 1,
		floatShot
	};
	LaserLevel nLaserLevel;
	LaserWeapon(LaserLevel nLaserLevel);
	void Use(float fX, float fY, float fAngle) override;

};

class LaserBeam : public Entity
{
	bool bFireSecond;
	float fSecondsUntilSecondFire;
	std::shared_ptr<LaserBeam> lbNextShot;
	float fAngle;
public:
	LaserBeam(LaserWeapon* weapon, float fX, float fY, float fSpeedX, float fSpeedY);
	bool Collide(Entity* entity) override;
	bool Update(float deltatime) override;
	void Draw() override;
};