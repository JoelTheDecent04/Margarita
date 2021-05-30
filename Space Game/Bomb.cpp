#include "Bomb.h"
#include "Space.h"
#include "Player.h"

Bomb::Bomb(SpaceGame* game, float fX, float fY, float fSpeedX, float fSpeedY)
	: Entity(game, tBombTexture, fX, fY)
{
	this->fSpeedX = fSpeedX;
	this->fSpeedY = fSpeedY;
	bCanCollideWithPlayer = false;
	fAge = 0.0f;
}

void Bomb::Collide(Entity* entity)
{
	Explode();
}

void Bomb::Explode()
{
	for (Entity* entity : sgGame->vEntities)
	{
		if (entity == nullptr || entity == this) continue;
		float fDistance = Distance(entity);
		if (fDistance < 100.0f)
			entity->ChangeHealth(-250.0f * ((100.0f - fDistance) / 100.0f));
	}

	Destroy();
}

void Bomb::Update(double deltatime)
{
	Entity::Update(deltatime);
	fAge += deltatime;
	if (fAge >= 1.0f)
	{
		Explode();
		return;
	}
}

BombWeapon::BombWeapon()
{
	tTexture = tBombTexture;
}

void BombWeapon::Use(SpaceGame* game, float fX, float fY, float fAngle)
{
	if (game->plPlayer->nEnergy >= 8.0f)
	{
		game->vEntities.push_back(new Bomb(game, fX, fY, 300.0f * cos(fAngle), 300.0f * sin(fAngle)));
		game->plPlayer->nEnergy -= 8.0f;
	}
}
