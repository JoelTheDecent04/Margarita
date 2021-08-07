#include "Enemy.h"
#include "Space.h"
#include "Player.h"
#include "Utilities.h"

Enemy::Enemy(float fX, float fY)
	: Entity(tEnemyTexture, fX, fY)
{
	fHealth = 100.0f;
	fMaxHealth = 100.0f;
	bAffectedByGravity = false;
	fWidth = 32.0f;
	fHeight = 49.0f;
	fSecondsUntilNextAttack = 0.0f;
	bLegalPosition = true;

	for (auto entity : Game::sgSpaceGame->vEntities)
	{
		if (entity)
			if (Overlapping(entity.get()))
			{
				bLegalPosition = false;
				break;
			}
	}

	nType = Type::Enemy;
}

bool Enemy::Update(float deltatime)
{
	if (fHealth == 0.0f)
	{
		Destroy();
		return false;
	}

	if (Distance(Game::sgSpaceGame->plPlayer.get()) > 55.0f)
	{
		float fGradient = (Game::sgSpaceGame->plPlayer->fY - fY) / (Game::sgSpaceGame->plPlayer->fX - fX);
		float fAngle = atan(fGradient);
		if (Game::sgSpaceGame->plPlayer->fX < fX) fAngle += 3.1415926;

		fSpeedX = cos(fAngle) * (100.0f + Game::sgSpaceGame->fDifficulty / 60.0f);
		fSpeedY = sin(fAngle) * (100.0f + Game::sgSpaceGame->fDifficulty / 60.0f);
	}
	else
	{
		fSpeedX = 0.0f;
		fSpeedY = 0.0f;

		if (fSecondsUntilNextAttack > 0.0f)
			fSecondsUntilNextAttack -= deltatime;
		else
		{
			Game::sgSpaceGame->plPlayer->ChangeHealth(-10.0f, this);
			fSecondsUntilNextAttack = 1.0f;
		}
	}

	return Entity::Update(deltatime);
}

void Enemy::Destroy()
{
	Game::sgSpaceGame->plPlayer->fMoney += 10.0f;
	Game::sgSpaceGame->nEnemies--;
}