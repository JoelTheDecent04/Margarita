#include "Orb.h"
#include "OnlineSpaceGame.h"
#include "ServerUtilities.h"

Orb::Orb(float fX, float fY, float fSpeedX, float fSpeedY, const std::shared_ptr<Entity>& parent)
	: Entity(fX, fY, 32, 32, parent)
{
	this->fSpeedX = fSpeedX;
	this->fSpeedY = fSpeedY;
	bCanCollideWithPlayer = true;

	fAge = 0.0f;
	nType = Type::Orb;
	nTexture = TextureID::Orb;
	bAffectedByGravity = true;
}

bool Orb::Update(float deltatime)
{
	fAge += deltatime;
	if (fAge >= 5.0f)
	{
		Destroy(nullptr);
		return false;
	}

	return Entity::Update(deltatime);
}

bool Orb::Collide(Entity* entity)
{
	entity->ChangeHealth(-100.0f, this);
	Destroy(entity); //entity destroys this
	return false;
}

/*OrbWeapon::OrbWeapon()
{
	nCount = -1;
	nType = Type::Orb;
}

void OrbWeapon::Use(Player* player, float fX, float fY, float fAngle)
{
	if (player->fEnergy >= 5.0f)
	{
		sgSpaceGame->vEntities.push_back(std::make_shared<Orb>(fX, fY, 300.0f * cos(fAngle), 300.0f * sin(fAngle)));
		player->fEnergy -= 5.0f;
	}
}*/