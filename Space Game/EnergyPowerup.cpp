#include "EnergyPowerup.h"
#include "Space.h"
#include "Player.h"

EnergyPowerup::EnergyPowerup(SpaceGame* game)
	: Powerup(game)
{
	fTimeLeft = 10.0f;
}

void EnergyPowerup::Update(float deltatime)
{
	fTimeLeft -= deltatime;
	if (fTimeLeft <= 0.0f)
	{
		Remove();
		return;
	}
	sgGame->plPlayer->nEnergy += 15 * deltatime;
	if (sgGame->plPlayer->nEnergy > sgGame->plPlayer->nMaxEnergy)
		sgGame->plPlayer->nEnergy = sgGame->plPlayer->nMaxEnergy;
}

EnergyPowerupItem::EnergyPowerupItem()
{
	tTexture = tEnergyPowerupTexture;
	nCount = 1;
	nType = Type::EnergyPowerup;
	strName = "Energy Powerup";
}

void EnergyPowerupItem::Use(SpaceGame* game, float fX, float fY, float fAngle)
{
	if (game->plPlayer->puCurrentPowerup == nullptr)
	{
		nCount--;
		game->plPlayer->puCurrentPowerup = std::make_shared<EnergyPowerup>(game);
	}
}
