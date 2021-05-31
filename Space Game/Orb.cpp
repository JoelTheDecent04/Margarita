#include "Orb.h"
#include "Space.h"
#include "Player.h"

Orb::Orb(SpaceGame* game, float fX, float fY, float fSpeedX, float fSpeedY)
	: Entity(game, tOrbTexture, fX, fY)
{
	this->fSpeedX = fSpeedX;
	this->fSpeedY = fSpeedY;
	bCanCollideWithPlayer = false;
}

void Orb::Update(double deltatime)
{
	Entity::Update(deltatime);
	fAge += deltatime;
	if (fAge >= 5.0f)
		Destroy();
	
}

void Orb::Collide(Entity* entity)
{
	entity->ChangeHealth(-100.0f);
	Destroy();
}

OrbWeapon::OrbWeapon()
{
	nCount = -1;
	tTexture = tOrbTexture;
}

void OrbWeapon::Use(SpaceGame* game, float fX, float fY, float fAngle)
{
	if (game->plPlayer->nEnergy >= 5.0f)
	{
		game->vEntities.push_back(new Orb(game, fX, fY, 300.0f * cos(fAngle), 300.0f * sin(fAngle)));
		game->plPlayer->nEnergy -= 5.0f;
	}
}