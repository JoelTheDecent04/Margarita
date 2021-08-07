#include "Bomb.h"
#include "Space.h"
#include "Player.h"
#include "BombAnimation.h"

Bomb::Bomb(float fX, float fY, float fSpeedX, float fSpeedY, int nLevel)
	: Entity(TextureID::Bomb, fX, fY)
{
	this->fSpeedX = fSpeedX;
	this->fSpeedY = fSpeedY;
	bCanCollideWithPlayer = false;
	fAge = 0.0f;
	this->nLevel = nLevel;

	nType = Type::Bomb;
}

bool Bomb::Collide(Entity* entity)
{
	Explode();
	return false;
}

void Bomb::Explode()
{
	Game::sgSpaceGame->vBackgroundObjects.push_back(std::make_shared<BombAnimation>(this));
	for (auto entity : Game::sgSpaceGame->vEntities)
	{
		if (entity.get() == this) continue;
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

nlohmann::json Bomb::Save()
{
	nlohmann::json j = Entity::Save();
	j["age"] = fAge;
	j["level"] = nLevel;
	return j;
}

void Bomb::Load(nlohmann::json& j)
{
	fAge = j["age"].get<float>();
	nLevel = j["level"].get<int>();
}

BombWeapon::BombWeapon(int nLevel)
{
	this->nLevel = nLevel;
	nCount = nLevel;
	nTexture = TextureID::Bomb;
	nType = Type::Bomb;
	strName = "Bomb";
}

void BombWeapon::Use(float fX, float fY, float fAngle)
{
	if (Game::sgSpaceGame->plPlayer->nEnergy >= 8.0f + 3.0f * nLevel)
	{
		Game::sgSpaceGame->vEntities.push_back(std::make_shared<Bomb>(fX, fY, 300.0f * cos(fAngle), 300.0f * sin(fAngle), nLevel));
		Game::sgSpaceGame->plPlayer->nEnergy -= 8.0f + 3.0f * nLevel;
	}
}

void BombWeapon::Load(nlohmann::json& j)
{
	Item::Load(j);
	nLevel = j["level"].get<int>();
	nCount = nLevel;
}

nlohmann::json BombWeapon::Save()
{
	nlohmann::json j = Item::Save();
	j["level"] = nLevel;
	return j;
}
