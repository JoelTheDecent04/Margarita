#pragma once
#include <Utility>
#include <memory>

#include "../Margarita_generated.h"

class Player;

class Entity
{
public:
	enum class Type
	{
		None, Player, Bomb, Crab, Enemy, Laser, Orb
	};

	float fX, fY;
	float fSpeedX, fSpeedY;

	bool bAffectedByGravity;
	bool bCanCollide;
	bool bCanCollideWithPlayer;

	float fHealth;
	float fMaxHealth;
	float fHeight, fWidth;

	int nTexture;
	int nFrame;

	Type nType;

	Entity(float fX, float fY, float fHeight, float fWidth);
	virtual bool Update(float deltatime);
	virtual void ChangeHealth(float fChange, Entity* e);
	virtual bool Collide(Entity* entity) { return true; };
	bool Overlapping(Entity* e);
	bool WillOverlap(Entity* e, float fNewX, float fNewY);
	float Distance(Entity* entity);
	virtual void Destroy(Entity*) {};

	virtual flatbuffers::Offset<NetEntity> Serialise(flatbuffers::FlatBufferBuilder& packet);

	std::shared_ptr<Player> NearestPlayer(float* nearest_dist);
};