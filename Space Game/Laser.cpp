#include "Laser.h"
#include "Space.h"
#include "Player.h"
#include <math.h>

LaserBeam::LaserBeam(SpaceGame* game, float fX, float fY, float fSpeedX, float fSpeedY)
	: Entity(game, tLaserBeamTexture, fX, fY)
{
	bAffectedByGravity = false;
	this->fSpeedX = fSpeedX;
	this->fSpeedY = fSpeedY;
	bCanCollideWithPlayer = false;
}

void LaserBeam::Collide(Entity* entity)
{
	entity->ChangeHealth(-50.0f);
	Destroy();
}

LaserWeapon::LaserWeapon()
{
	tTexture = tLaserTexture;
}

void LaserWeapon::Use(SpaceGame* game, float fX, float fY, float fAngle)
{
	if (game->plPlayer->nEnergy >= 4)
	{
		game->vEntities.push_back(new LaserBeam(game, fX, fY, 1000.0f * cos(fAngle), 1000.0f * sin(fAngle)));
		game->plPlayer->nEnergy -= 4;
	}
}