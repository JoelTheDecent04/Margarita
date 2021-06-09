#pragma once
#include "Entity.h"
#include "Weapon.h"

class Bomb : public Entity
{
	float fAge;
public:
	int nLevel;
	Bomb(SpaceGame* game, float fX, float fY, float fSpeedX, float fSpeedY, int nLevel);
	bool Collide(Entity* entity) override;
	void Explode();
	bool Update(double deltatime) override;
	void Save(std::fstream& f) override;
	void Load(std::fstream& f) override;
};

class BombWeapon : public Weapon
{
public:
	int nLevel;
	BombWeapon(int nLevel);
	void Use(SpaceGame* game, float fX, float fY, float fAngle) override;
	
};