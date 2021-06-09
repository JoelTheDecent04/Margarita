#pragma once
#include "Texture.h"
#include "Space.h"
#include <fstream>

class Entity
{
public:
	enum class Type
	{
		None, Player, Bomb, Crab, Enemy, Laser, Orb
	};

	SpaceGame* sgGame;
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

	Entity(SpaceGame* sgGame, Texture* tTexture, float fX, float fY);
	virtual bool Update(double deltatime);
	virtual void ChangeHealth(float fChange);
	virtual bool Collide(Entity* entity) { return true; };
	virtual void Draw();
	bool Overlapping(Entity* e);
	bool WillOverlap(Entity* e, float fNewX, float fNewY);
	float Distance(Entity* entity);
	virtual void Destroy() {};
	virtual void Save(std::fstream& f);
	virtual void Load(std::fstream& f);
};