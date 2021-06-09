#pragma once
#include "Weapon.h"
#include "Entity.h"


class LaserWeapon : public Weapon
{
public:
	enum LaserLevel {
		Normal = 1,
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
	bool Collide(Entity* entity) override;
	bool Update(double deltatime) override;
	void Draw() override;
	void Save(std::fstream& f) override;
	void Load(std::fstream& f) override;
};