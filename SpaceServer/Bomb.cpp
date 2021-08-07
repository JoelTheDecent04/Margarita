#include "Bomb.h"
#include "OnlineSpaceGame.h"
#include "Player.h"
#include "ServerUtilities.h"

Bomb::Bomb(float fX, float fY, float fSpeedX, float fSpeedY, int nLevel)
	: Entity(fX, fY, 32.0f, 32.0f)
{
	this->fSpeedX = fSpeedX;
	this->fSpeedY = fSpeedY;
	bCanCollideWithPlayer = false;
	fAge = 0.0f;
	this->nLevel = nLevel;

	nType = Type::Bomb;
	nTexture = TextureID::Bomb;
}
bool Bomb::Collide(Entity* entity)
{
	Explode();
	return false;
}
void Bomb::Explode()
{
	for (auto& entity : sgSpaceGame->vEntities)
	{
		if (entity->nType == Type::Bomb) continue;
		float fDistance = Distance(entity.get());
		float fDamageDistance = 75.0f + 25.0f * nLevel;
		if (fDistance < fDamageDistance)
			entity->ChangeHealth((-250.0f - 20.0f * nLevel) * ((fDamageDistance - fDistance) / fDamageDistance), this);
	}

	fHealth = 0.0f;
}
bool Bomb::Update(float deltatime)
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
	nType = Type::Bomb;
}

void BombWeapon::Use(Player* player, float fX, float fY, float fAngle)
{
	if (player->fEnergy >= 8.0f + 3.0f * nLevel)
	{
		sgSpaceGame->vEntities.push_back(std::make_shared<Bomb>(fX, fY, 300.0f * cos(fAngle), 300.0f * sin(fAngle), nLevel));
		player->fEnergy -= 8.0f + 3.0f * nLevel;
	}
}