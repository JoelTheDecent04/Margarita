#include "Laser.h"
#include "OnlineSpaceGame.h"
#include "Player.h"
#include "ServerUtilities.h"

#include <iostream>

#define PI 3.1415926f

/*LaserWeapon::LaserWeapon(int nLaserLevel)
{
	this->nLaserLevel = nLaserLevel;
	nCount = nLaserLevel;
	nType = Type::Laser;
}
void LaserWeapon::Use(Player* player, float fX, float fY, float fAngle)
{
	switch (nLaserLevel)
	{
	case Normal:
		if (player->fEnergy >= 4)
		{
			sgSpaceGame->vEntities.push_back(std::make_shared<LaserBeam>(fX, fY, fAngle));

			player->fEnergy -= 4;
		}
		break;
	case DoubleShot:
		if (player->fEnergy >= 6)
		{
			sgSpaceGame->vEntities.push_back(std::make_shared<LaserBeam>(this, fX, fY, fAngle));
			player->fEnergy -= 6;
		}
		break;
	}
}*/

LaserBeam::LaserBeam(float fX, float fY, float fAngle)
	: Entity(fX, fY, 0.0f, 0.0f)
{
	//std::cout << "Creating laser at " << fX << '\n';

	bAffectedByGravity = false;
	this->fSpeedX = 1000.0f * cos(fAngle);
	this->fSpeedY = 1000.0f * sin(fAngle);
	bCanCollideWithPlayer = false;
	bCanCollide = false;
	
	//TODO copy logic from double shot

	this->fAngle = fAngle * 180.0f / PI;

	nType = Type::Laser;
	nTexture = TextureID::Laserbeam;
}
bool LaserBeam::Collide(Entity* entity)
{
	entity->ChangeHealth(-50.0f, this);
	Destroy(entity); //entity destroys laserbeam
	return false;
}
bool LaserBeam::Update(float deltatime) 
{
	float fBulletX = fX + cos(PI * fAngle / 180.0f) * 16.0f; //Get new position
	float fBulletY = fY + sin(PI * fAngle / 180.0f) * 16.0f;

	float fNewX = fX + fSpeedX * deltatime;
	float fNewY = fY + fSpeedY * deltatime;

	bool bCollided = false;
	for (auto& entity : sgSpaceGame->vEntities) //Check for collisions
	{
		if (!entity) continue;
		if (entity.get() == this || !entity->bCanCollide) continue;
		if (entity->nType == Type::Player && !bCanCollideWithPlayer) continue;
		if (entity->WillOverlap(this, fBulletX, fBulletY))
		{
			bCollided = true;
			return Collide(entity.get());
		}
	}
	if (!bCollided)
	{
		fX = fNewX;
		fY = fNewY;
	}

	bool bSpeedChanged = false;

	if (fBulletY >= 594.0f) //If below ground
	{
		fY = 594.0f - sin(PI * fAngle / 180.0f) * 16.0f;
		fX += 2.0f * cos(PI * fAngle / 180.0f) * 16.0f;
		fSpeedY = -(fSpeedY) * 0.6f; //bounce
		bSpeedChanged = true;
	}

	if (fX < 0.0f || fX > 5120.0f || fY < 0.0f) { Destroy(nullptr); return false; }

	if (bSpeedChanged)
	{
		float fGradient = fSpeedY / fSpeedX;
		fAngle = atan(fGradient) * 180 / 3.1415926f;
		if (fSpeedX < 0.0f)
			fAngle += 180.0f;
	}

	/*if (bFireSecond)
	{
		fSecondsUntilSecondFire -= deltatime;
		if (fSecondsUntilSecondFire <= 0.0f)
		{
			lbNextShot->fX = Game::sgSpaceGame->plPlayer->fX;
			lbNextShot->fY = Game::sgSpaceGame->plPlayer->fY;

			Game::sgSpaceGame->vEntities.push_back(lbNextShot);
			bFireSecond = false;
		}
	}*/

	return true;

}

flatbuffers::Offset<NetEntity> LaserBeam::Serialise(flatbuffers::FlatBufferBuilder& packet)
{
	return CreateNetEntity(packet, fX - 32 / 2.0f, fY - 10 / 2.0f, nTexture, fAngle, nFrame); //32, 10 are texture width and height. Laserbeam manages its on collisions
}