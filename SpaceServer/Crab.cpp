#include "Crab.h"
#include "Player.h"
#include "OnlineSpaceGame.h"
#include "ServerUtilities.h"

Crab::Crab(float fX)
	: Entity(fX, 594 - (48 / 2), 96, 48)
{
	fHealth = 200.0f;
	fMaxHealth = 200.0f;
	bAffectedByGravity = false;
	fSecondsUntilNextAttack = 0.0f;
	bLegalPosition = true;

	nType = Type::Crab;

	for (auto& entity : sgSpaceGame->vEntities)
	{
		if (Overlapping(entity.get()))
		{
			bLegalPosition = false;
			break;
		}
	}
	nTexture = TextureID::Crab;
}
bool Crab::Update(float deltatime)
{
	float player_dist;
	auto player = NearestPlayer(&player_dist);

	if (player)
	{
		if (player_dist > 80.0f) //Check if okay
			fSpeedX = player->fX > fX ? 100.0f + sgSpaceGame->fDifficulty / 60.0f : -100.0f - sgSpaceGame->fDifficulty / 60.0f;
		else
		{
			fSpeedX = 0.0f;
			if (fSecondsUntilNextAttack > 0.0f)
				fSecondsUntilNextAttack -= deltatime;
			else if (player_dist <= 90.0f)
			{
				player->ChangeHealth(-15.0f, this);
				fSecondsUntilNextAttack = 1.0f;
			}
		}
	}
	return Entity::Update(deltatime);
}
void Crab::Destroy(Entity* entity)
{
	if (entity && entity->nType == Type::Player)
	{
		Player* player = (Player*)entity;
		player->fMoney += 15.0f;
	}
	sgSpaceGame->nEnemies--;
}