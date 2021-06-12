#include "RegenerationPowerup.h"
#include "Space.h"
#include "Player.h"

RegenerationPowerup::RegenerationPowerup(SpaceGame* sgGame)
	: Powerup(sgGame)
{
	fTimeLeft = 10.0f;
}

void RegenerationPowerup::Update(double deltatime)
{
	fTimeLeft -= deltatime;
	if (fTimeLeft <= 0.0f)
	{
		Remove(); //Memory leak
		return;
	}

	sgGame->plPlayer->fHealth += 25 * deltatime;
	if (sgGame->plPlayer->fHealth > sgGame->plPlayer->fMaxHealth)
		sgGame->plPlayer->fHealth = sgGame->plPlayer->fMaxHealth;
}

RegenerationPowerupItem::RegenerationPowerupItem()
{
	tTexture = tRegenerationPowerupTexture;
	nCount = 1;
	nType = Type::RegenerationPowerup;
}

void RegenerationPowerupItem::Use(SpaceGame* game, float fX, float fY, float fAngle)
{
	if (game->plPlayer->puCurrentPowerup == nullptr)
	{
		nCount--;
		game->plPlayer->puCurrentPowerup = std::make_shared<RegenerationPowerup>(game);
	}
}
