#include "Orb.h"
#include "Space.h"
#include "Player.h"

Orb::Orb(float fX, float fY, float fSpeedX, float fSpeedY)
	: Entity(TextureID::Orb, fX, fY)
{
	this->fSpeedX = fSpeedX;
	this->fSpeedY = fSpeedY;
	bCanCollideWithPlayer = false;

	fAge = 0.0f;
	nType = Type::Orb;
}

bool Orb::Update(float deltatime)
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
	entity->ChangeHealth(-100.0f, this);
	Destroy();
	return false;
}

OrbWeapon::OrbWeapon()
{
	nCount = -1;
	nTexture = TextureID::Orb;
	nType = Type::Orb;
	strName = "Orb";
}

void OrbWeapon::Use(float fX, float fY, float fAngle)
{
	if (Game::sgSpaceGame->plPlayer->nEnergy >= 5.0f)
	{
		Game::sgSpaceGame->vEntities.push_back(std::make_shared<Orb>(fX, fY, 300.0f * cos(fAngle), 300.0f * sin(fAngle)));
		Game::sgSpaceGame->plPlayer->nEnergy -= 5.0f;
	}
}