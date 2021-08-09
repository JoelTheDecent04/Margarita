#pragma once
#include "Weapon.h"
#include "Entity.h"

/*class LaserWeapon : public Weapon
{
public:
	enum { Normal = 1, DoubleShot };
	int nLaserLevel;
	LaserWeapon(int nLaserLevel);
	void Use(Player* p, float fX, float fY, float fAngle);
};*/

class LaserBeam : public Entity
{
	bool bFireSecond;
	float fSecondsUntilNextFire;
	std::shared_ptr<LaserBeam> lbNextShot;
	float fAngle;
public:
	LaserBeam(float fX, float fY, float fAngle);
	bool Collide(Entity* entity) override;
	bool Update(float deltatime);
	flatbuffers::Offset<NetEntity> Serialise(flatbuffers::FlatBufferBuilder& packet) override;
};