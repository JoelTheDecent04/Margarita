#include "Enemy.h"
#include "Space.h"
#include "Player.h"
#include "Utilities.h"

Enemy::Enemy(SpaceGame* game, float fX, float fY)
	: Entity(game, tEnemyTexture, fX, fY)
{
	fHealth = 100.0f;
	fMaxHealth = 100.0f;
	bAffectedByGravity = false;
	fWidth = 41.0f;
	fHeight = 52.0f;
	fSecondsUntilNextAttack = 0.0f;
	bLegalPosition = true;

	for (Entity* entity : game->vEntities)
	{
		if (entity)
			if (Overlapping(entity))
			{
				bLegalPosition = false;
				break;
			}
	}
}

void Enemy::Update(double deltatime)
{
	Entity::Update(deltatime);

	if (Distance(sgGame->plPlayer) > 80.0f)
	{
		float fGradient = (sgGame->plPlayer->fY - fY) / (sgGame->plPlayer->fX - fX);
		float fAngle = atan(fGradient);
		if (sgGame->plPlayer->fX < fX) fAngle += 3.1415926;

		fSpeedX = cos(fAngle) * (100.0f + sgGame->fDifficulty / 60.0f);
		fSpeedY = sin(fAngle) * (100.0f + sgGame->fDifficulty / 60.0f);
	}
	else
	{
		fSpeedX = 0.0f;
		fSpeedY = 0.0f;

		if (fSecondsUntilNextAttack > 0.0f)
			fSecondsUntilNextAttack -= deltatime;
		else
		{
			sgGame->plPlayer->ChangeHealth(-10.0f);
			fSecondsUntilNextAttack = 1.0f;
		}
	}
}

void Enemy::Destroy()
{
	sgGame->plPlayer->fMoney += 10.0f;
	Entity::Destroy();
}