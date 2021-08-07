#include "RegenerationPowerup.h"
#include "Space.h"
#include "Player.h"

RegenerationPowerup::RegenerationPowerup(std::shared_ptr<Player>& player)
	: Powerup(player)
{
	fTimeLeft = 10.0f;
}

void RegenerationPowerup::Update(float deltatime)
{
	fTimeLeft -= deltatime;
	if (fTimeLeft <= 0.0f)
	{
		Remove(); //Memory leak
		return;
	}

	plPlayer->fHealth += 25 * deltatime;
	if (plPlayer->fHealth > plPlayer->fMaxHealth)
		plPlayer->fHealth = plPlayer->fMaxHealth;
}

RegenerationPowerupItem::RegenerationPowerupItem(std::shared_ptr<Player>& player)
	: Item(player)
{
	nCount = 1;
	nType = Type::RegenerationPowerup;
	strName = "Regeneration Powerup";
}

void RegenerationPowerupItem::Use(float fX, float fY, float fAngle)
{
	if (plPlayer->puCurrentPowerup == nullptr)
	{
		nCount--;
		plPlayer->puCurrentPowerup = std::static_pointer_cast<Powerup, RegenerationPowerup>(std::make_shared<RegenerationPowerup>());
	}
}
