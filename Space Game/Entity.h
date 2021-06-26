#pragma once
#include "Texture.h"
#include <fstream>
#include <memory>

class SpaceGame;

class Entity
{
public:
	enum class Type
	{
		None, Player, Bomb, Crab, Enemy, Laser, Orb, Light
	};

	Texture* tTexture;
	int nFrame;
	float fX, fY;
	float fSpeedX, fSpeedY;
	bool bOnGround;
	bool bAffectedByGravity;
	bool bCanCollide;
	bool bCanCollideWithPlayer;
	float fHealth;
	float fMaxHealth;
	float fHeight, fWidth;

	Type nType;

	Entity(Texture* tTexture, float fX, float fY);
	virtual bool Update(float deltatime);
	virtual void ChangeHealth(float fChange, Entity* e);
	virtual bool Collide(Entity* entity) { return true; };
	virtual void Draw();
	bool Overlapping(Entity* e);
	bool WillOverlap(Entity* e, float fNewX, float fNewY);
	float Distance(Entity* entity);
	virtual void Destroy() {};
	virtual void Save(std::fstream& f);
	virtual void Load(std::fstream& f);
};

extern const char* astrEntityName[8];
