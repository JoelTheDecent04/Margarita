#pragma once
#include "Texture.h"
#include "Space.h"

class Entity
{
public:
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

	Entity(SpaceGame* sgGame, Texture* tTexture, float fX, float fY);
	virtual void Update(double deltatime);
	virtual void ChangeHealth(float fChange);
	virtual void Collide(Entity* entity) {};
	void Draw();
	bool Overlapping(Entity* e);
	bool WillOverlap(Entity* e, float fNewX, float fNewY);
	float Distance(Entity* entity);
	virtual void Destroy();
};