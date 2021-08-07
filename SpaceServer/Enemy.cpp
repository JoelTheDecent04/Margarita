#include "Enemy.h"
#include "OnlineSpaceGame.h"
#include "Player.h"
#include "ServerUtilities.h"

Enemy::Enemy(float fX, float fY)
	: Entity(fX, fY, 38.0f, 55.0f)
{
	fHealth = 100.0f;
	fMaxHealth = 100.0f;
	bAffectedByGravity = false;
	fSecondsUntilNextAttack = 0.0f;
	bLegalPosition = true;
	
	nType = Type::Enemy;

	for (auto& entity : sgSpaceGame->vEntities)
	{
		if (Overlapping(entity.get()))
		{
			bLegalPosition = true;
			break;
		}
	}

	nTexture = TextureID::Enemy;
}
bool Enemy::Update(float deltatime)
{
	float player_dist;
	auto nearest_player = NearestPlayer(&player_dist);

	if (nearest_player)
	{
		if (player_dist > 55.0f)
		{
			float fGradient = (nearest_player->fY - fY) / (nearest_player->fX - fX);
			float fAngle = (float)atan(fGradient);
			if (nearest_player->fX < fX) fAngle += 3.1415926f;

			fSpeedX = (float)cos(fAngle) * (100.0f + sgSpaceGame->fDifficulty / 60.0f);
			fSpeedY = (float)sin(fAngle) * (100.0f + sgSpaceGame->fDifficulty / 60.0f);
		}
		else
		{
			fSpeedX = 0.0f;
			fSpeedY = 0.0f;

			if (fSecondsUntilNextAttack > 0.0f)
				fSecondsUntilNextAttack -= deltatime;
			else
			{
				nearest_player->ChangeHealth(-10.0f, this);
				fSecondsUntilNextAttack = 1.0f;
			}
		}
	}

	return Entity::Update(deltatime);
}
void Enemy::Destroy(Entity* entity)
{
	if (entity && entity->nType == Type::Player)
	{
		Player* player = (Player*)entity;
		player->fMoney += 10.0f;
	}
	sgSpaceGame->nEnemies--;
}