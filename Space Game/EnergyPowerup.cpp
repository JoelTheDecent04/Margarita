#include "EnergyPowerup.h"
#include "Space.h"
#include "Player.h"

EnergyPowerup::EnergyPowerup(SpaceGame* game)
	: Powerup(game)
{
	fTimeLeft = 10.0f;
}

void EnergyPowerup::Update(double deltatime)
{
	fTimeLeft -= deltatime;
	if (fTimeLeft <= 0.0f)
	{
		Remove();
		return;
	}
	sgGame->plPlayer->nEnergy += 25 * deltatime;
	if (sgGame->plPlayer->nEnergy > sgGame->plPlayer->nMaxEnergy)
		sgGame->plPlayer->nEnergy = sgGame->plPlayer->nMaxEnergy;
}

EnergyPowerupItem::EnergyPowerupItem()
{
	tTexture = tNoTexture;
	nCount = 1;
}

void EnergyPowerupItem::Use(SpaceGame* game, float fX, float fY, float fAngle)
{
	nCount--;
	game->plPlayer->vPowerups.push_back(new EnergyPowerup(game));
}
