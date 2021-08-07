#pragma once
#include "Texture.h"
#include "nlohmann/json.hpp"
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

	int nTexture;
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

	Entity(int nTexture, float fX, float fY);
	virtual bool Update(float deltatime);
	virtual void ChangeHealth(float fChange, Entity* e);
	virtual bool Collide(Entity* entity) { return true; };
	virtual void Draw();
	bool Overlapping(Entity* e);
	bool WillOverlap(Entity* e, float fNewX, float fNewY);
	float Distance(Entity* entity);
	virtual void Destroy() {};
	virtual nlohmann::json Save();
	virtual void Load(nlohmann::json& j);
};

extern const char* astrEntityName[8];
