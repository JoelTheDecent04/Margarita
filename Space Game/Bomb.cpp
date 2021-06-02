#include "Bomb.h"
#include "Space.h"
#include "Player.h"
#include "BombAnimation.h"

Bomb::Bomb(SpaceGame* game, float fX, float fY, float fSpeedX, float fSpeedY, int nLevel)
	: Entity(game, tBombTexture, fX, fY)
{
	this->fSpeedX = fSpeedX;
	this->fSpeedY = fSpeedY;
	bCanCollideWithPlayer = false;
	fAge = 0.0f;
	this->nLevel = nLevel;
}

bool Bomb::Collide(Entity* entity)
{
	Explode();
	return false;
}

void Bomb::Explode()
{
	sgGame->vBackgroundObjects.push_back(new BombAnimation(this));
	for (Entity* entity : sgGame->vEntities)
	{
		if (entity == nullptr || entity == this) continue;
		float fDistance = Distance(entity);
		float fDamageDistance = 75.0f + 25.0f * nLevel;
		if (fDistance < fDamageDistance)
			entity->ChangeHealth((-250.0f - 20.0f * nLevel) * ((fDamageDistance - fDistance) / fDamageDistance));
	}

	fHealth = 0.0f;
}

bool Bomb::Update(double deltatime)
{
	if (Entity::Update(deltatime) == false) return false;
	fAge += deltatime;
	if (fAge >= 0.75f + 0.25f * nLevel)
	{
		Explode();
		return false;
	}
	return true;
}

BombWeapon::BombWeapon(int nLevel)
{
	this->nLevel = nLevel;
	nCount = nLevel;
	tTexture = tBombTexture;
}

void BombWeapon::Use(SpaceGame* game, float fX, float fY, float fAngle)
{
	if (game->plPlayer->nEnergy >= 8.0f + 3.0f * nLevel)
	{
		game->vEntities.push_back(new Bomb(game, fX, fY, 300.0f * cos(fAngle), 300.0f * sin(fAngle), nLevel));
		game->plPlayer->nEnergy -= 8.0f + 3.0f * nLevel;
	}
}
