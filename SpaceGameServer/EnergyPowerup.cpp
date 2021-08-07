#include "EnergyPowerup.h"
#include "Space.h"
#include "Player.h"

EnergyPowerup::EnergyPowerup(std::shared_ptr<Player>& player)
	: Powerup(player)
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
	plPlayer->nEnergy += 15 * deltatime;
	if (plPlayer->nEnergy > plPlayer->nMaxEnergy)
		plPlayer->nEnergy = plPlayer->nMaxEnergy;
}

EnergyPowerupItem::EnergyPowerupItem(std::shared_ptr<Player>& player)
{
	plPlayer = player; 
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
