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

bool Orb::Update(double deltatime)
{
	fAge += deltatime;
	if (fAge >= 5.0f)
	{
		Destroy();
		return false;
	}

	return Entity::Update(deltatime);
}

bool Orb::Collide(Entity* entity)
{
	entity->ChangeHealth(-100.0f);
	Destroy();
	return false;
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
		OutputDebugString(L"OrbWeapon::Use locking\n");
		game->m_r_vEntities.lock(); game->m_u_vEntities.lock(); game->m_w_vEntities.lock(); //In window thread. Adding to vector
		game->vEntities.push_back(new Orb(game, fX, fY, 300.0f * cos(fAngle), 300.0f * sin(fAngle)));
		game->m_r_vEntities.unlock(); game->m_u_vEntities.unlock(); game->m_w_vEntities.unlock(); //In window thread. Adding to vector

		game->plPlayer->nEnergy -= 5.0f;
	}
}