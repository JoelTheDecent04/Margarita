#include "Crab.h"
#include "Space.h"
#include "Player.h"

Crab::Crab(SpaceGame* game, float fX)
	: Entity(game, tCrabTexture, fX, 594 - (48 / 2)),
	animation(tCharacterTexture, 14, 60.0f, &nFrame)
{
	fHealth = 200.0f;
	fMaxHealth = 200.0f;
	bAffectedByGravity = false;
	fWidth = 88.0f;
	fHeight = 44.0f;
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
bool Crab::Update(double deltatime)
{
	if (abs(sgGame->plPlayer->fX - fX) > 80.0f)
		fSpeedX = sgGame->plPlayer->fX > fX ? 100.0f + sgGame->fDifficulty / 60.0f : -100.0f - sgGame->fDifficulty / 60.0f;
	else
	{
		fSpeedX = 0.0f;
		if (fSecondsUntilNextAttack > 0.0f)
			fSecondsUntilNextAttack -= deltatime;
		else if (Distance(sgGame->plPlayer) <= 90.0f)
		{
			sgGame->plPlayer->ChangeHealth(-15.0f);
			fSecondsUntilNextAttack = 1.0f;
		}
	}

	animation.Update(deltatime);
	
	if (Entity::Update(deltatime) == false) return false;

	if (abs(fSpeedX) > 0.0f)
		animation.Start();
	else
		animation.Stop();
}
void Crab::Destroy()
{
	sgGame->plPlayer->fMoney += 15.0f;
	sgGame->nEnemies--;
	Entity::Destroy();
}