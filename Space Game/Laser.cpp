#include "Laser.h"
#include "Space.h"
#include "Player.h"
#include <math.h>
#define PI 3.1415926f

LaserBeam::LaserBeam(LaserWeapon* weapon, float fX, float fY, float fSpeedX, float fSpeedY)
	: Entity(TextureID::Laserbeam, fX, fY)
{
	bAffectedByGravity = false;
	this->fSpeedX = fSpeedX;
	this->fSpeedY = fSpeedY;
	bCanCollideWithPlayer = false;
	bCanCollide = false;

	fWidth = 0.0f;
	fHeight = 0.0f;

	if (weapon && weapon->nLaserLevel == LaserWeapon::floatShot)
	{
		bFireSecond = true;
		fSecondsUntilSecondFire = 0.1f;
		lbNextShot = std::make_shared<LaserBeam>(nullptr, 0.0f, 0.0f, fSpeedX, fSpeedY);
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
	entity->ChangeHealth(-50.0f, this);
	Destroy();
	return false;
}

void LaserBeam::Draw()
{
	textures[nTexture]->Draw(nFrame, (fX - fBackgroundPosition - (textures[nTexture]->fTextureDrawnWidth / 2)), fY - textures[nTexture]->fTextureDrawnHeight / 2, false, fAngle);
}

nlohmann::json LaserBeam::Save()
{
	nlohmann::json j = Entity::Save();
	j["angle"] = fAngle;
	return j;
}

void LaserBeam::Load(nlohmann::json& j)
{
	Entity::Load(j);
	fAngle = j["angle"].get<float>();
}

bool LaserBeam::Update(float deltatime)
{
	float fBulletX = fX + cos(PI * fAngle / 180.0f) * 16.0f; //Get new position
	float fBulletY = fY + sin(PI * fAngle / 180.0f) * 16.0f;

	float fNewX = fX + fSpeedX * deltatime;
	float fNewY = fY + fSpeedY * deltatime;

	bool bCollided = false;
	for (auto entity : Game::sgSpaceGame->vEntities) //Check for collisions
	{
		if (!entity) continue;
		if (entity.get() == this || !entity->bCanCollide) continue;
		if (entity == Game::sgSpaceGame->plPlayer && !bCanCollideWithPlayer) continue;
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
			lbNextShot->fX = Game::sgSpaceGame->plPlayer->fX;
			lbNextShot->fY = Game::sgSpaceGame->plPlayer->fY;

			Game::sgSpaceGame->vEntities.push_back(lbNextShot);
			bFireSecond = false;
		}
	}

	if (fX < 0.0f || fX > 5120.0f || fY < -250.0f)
	{
		return false;
	}
	
	return true;
}

LaserWeapon::LaserWeapon(LaserLevel nLaserLevel)
{
	nTexture = TextureID::Laser;
	this->nLaserLevel = nLaserLevel;
	nCount = nLaserLevel;
	nType = Type::Laser;
	strName = "Laser";
}

void LaserWeapon::Use(float fX, float fY, float fAngle)
{
	switch (nLaserLevel)
	{
	case Normal:
		if (Game::sgSpaceGame->plPlayer->nEnergy >= 4)
		{
			Game::sgSpaceGame->vEntities.push_back(std::make_shared<LaserBeam>(this, fX, fY, 1000.0f * cos(fAngle), 1000.0f * sin(fAngle)));

			Game::sgSpaceGame->plPlayer->nEnergy -= 4;
		}
		break;
	case floatShot:
		if (Game::sgSpaceGame->plPlayer->nEnergy >= 6)
		{
			Game::sgSpaceGame->vEntities.push_back(std::make_shared<LaserBeam>(this, fX, fY, 1000.0f * cos(fAngle), 1000.0f * sin(fAngle)));
			Game::sgSpaceGame->plPlayer->nEnergy -= 6;
		}
		break;
	}
}

nlohmann::json LaserWeapon::Save()
{
	nlohmann::json j = Item::Save();
	j["laser_level"] = nLaserLevel;
	return j;
}

void LaserWeapon::Load(nlohmann::json& j)
{
	Item::Load(j);
	nLaserLevel = j["laser_level"].get<LaserLevel>();
	nCount = nLaserLevel;
}
