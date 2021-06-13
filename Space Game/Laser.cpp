#include "Laser.h"
#include "Space.h"
#include "Player.h"
#include <math.h>
#include <Windows.h>
#define PI 3.1415926f

LaserBeam::LaserBeam(SpaceGame* game, LaserWeapon* weapon, float fX, float fY, float fSpeedX, float fSpeedY)
	: Entity(game, tLaserBeamTexture, fX, fY)
{
	bAffectedByGravity = false;
	this->fSpeedX = fSpeedX;
	this->fSpeedY = fSpeedY;
	bCanCollideWithPlayer = false;
	bCanCollide = false;

	fWidth = 0.0f;
	fHeight = 0.0f;

	if (weapon && weapon->nLaserLevel == LaserWeapon::DoubleShot)
	{
		bFireSecond = true;
		fSecondsUntilSecondFire = 0.1f;
		lbNextShot = std::make_shared<LaserBeam>(game, nullptr, 0.0f, 0.0f, fSpeedX, fSpeedY);
	}
	else
		bFireSecond = false;

	float fGradient = fSpeedY / fSpeedX;
	fAngle = atan(fGradient) * 180 / 3.1415926f;
	if (fSpeedX < 0.0f)
		fAngle += 180.0f;

	nType = Type::Laser;
}

bool LaserBeam::Collide(Entity* entity)
{
	entity->ChangeHealth(-50.0f);
	Destroy();
	return false;
}

void LaserBeam::Draw()
{
	tTexture->Draw(nFrame, (fX - fBackgroundPosition - (tTexture->fTextureDrawnWidth / 2)), fY - tTexture->fTextureDrawnHeight / 2, false, fAngle);
}

void LaserBeam::Save(std::fstream& f)
{
	Entity::Save(f);
	f << fAngle << " ";
}

void LaserBeam::Load(std::fstream& f)
{
	Entity::Load(f);
	f >> fAngle;
}

bool LaserBeam::Update(double deltatime)
{
	float fBulletX = fX + cos(PI * fAngle / 180.0f) * 16.0f; //Get new position
	float fBulletY = fY + sin(PI * fAngle / 180.0f) * 16.0f;

	float fNewX = fX + fSpeedX * deltatime;
	float fNewY = fY + fSpeedY * deltatime;

	bool bCollided = false;
	for (auto entity : sgGame->vEntities) //Check for collisions
	{
		if (!entity) continue;
		if (entity.get() == this || !entity->bCanCollide) continue;
		if (entity == sgGame->plPlayer && !bCanCollideWithPlayer) continue;
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

	if (fX < 0.0f || fX > 5120.0f) { Destroy(); return false; }

	if (bSpeedChanged)
	{
		float fGradient = fSpeedY / fSpeedX;
		fAngle = atan(fGradient) * 180 / 3.1415926f;
		if (fSpeedX < 0.0f)
			fAngle += 180.0f;
	}

	if (bFireSecond)
	{
		fSecondsUntilSecondFire -= deltatime;
		if (fSecondsUntilSecondFire <= 0.0f)
		{
			lbNextShot->fX = sgGame->plPlayer->fX;
			lbNextShot->fY = sgGame->plPlayer->fY;

			sgGame->vEntities.push_back(lbNextShot);
			bFireSecond = false;
		}
	}
	
	return true;
}

LaserWeapon::LaserWeapon(LaserLevel nLaserLevel)
{
	tTexture = tLaserTexture;
	this->nLaserLevel = nLaserLevel;
	nCount = nLaserLevel;
	nType = Type::Laser;
	strName = L"Laser";
}

void LaserWeapon::Use(SpaceGame* game, float fX, float fY, float fAngle)
{
	switch (nLaserLevel)
	{
	case Normal:
		if (game->plPlayer->nEnergy >= 4)
		{
			game->vEntities.push_back(std::make_shared<LaserBeam>(game, this, fX, fY, 1000.0f * cos(fAngle), 1000.0f * sin(fAngle)));

			game->plPlayer->nEnergy -= 4;
		}
		break;
	case DoubleShot:
		if (game->plPlayer->nEnergy >= 6)
		{
			game->vEntities.push_back(std::make_shared<LaserBeam>(game, this, fX, fY, 1000.0f * cos(fAngle), 1000.0f * sin(fAngle)));
			game->plPlayer->nEnergy -= 6;
		}
		break;
	}
}

void LaserWeapon::Save(std::fstream& f)
{
	Item::Save(f);
	f << (int)nLaserLevel << " ";
}

void LaserWeapon::Load(std::fstream& f)
{
	Item::Load(f);
	int level;
	f >> level;
	nLaserLevel = (LaserLevel)level;
	nCount = nLaserLevel;
}
