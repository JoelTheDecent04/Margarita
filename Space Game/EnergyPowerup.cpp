#include "EnergyPowerup.h"
#include "Space.h"
#include "Player.h"

EnergyPowerup::EnergyPowerup()
	: Powerup()
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
	Game::sgSpaceGame->plPlayer->nEnergy += 15 * deltatime;
	if (Game::sgSpaceGame->plPlayer->nEnergy > Game::sgSpaceGame->plPlayer->nMaxEnergy)
		Game::sgSpaceGame->plPlayer->nEnergy = Game::sgSpaceGame->plPlayer->nMaxEnergy;
}

EnergyPowerupItem::EnergyPowerupItem()
{
	tTexture = tEnergyPowerupTexture;
	nCount = 1;
	nType = Type::EnergyPowerup;
	strName = "Energy Powerup";
}

void EnergyPowerupItem::Use(float fX, float fY, float fAngle)
{
	if (Game::sgSpaceGame->plPlayer->puCurrentPowerup == nullptr)
	{
		nCount--;
		Game::sgSpaceGame->plPlayer->puCurrentPowerup = std::make_shared<EnergyPowerup>();
	}
}
