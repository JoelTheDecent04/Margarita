#include "RegenerationPowerup.h"
#include "Space.h"
#include "Player.h"

RegenerationPowerup::RegenerationPowerup()
	: Powerup()
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

	Game::sgSpaceGame->plPlayer->fHealth += 25 * deltatime;
	if (Game::sgSpaceGame->plPlayer->fHealth > Game::sgSpaceGame->plPlayer->fMaxHealth)
		Game::sgSpaceGame->plPlayer->fHealth = Game::sgSpaceGame->plPlayer->fMaxHealth;
}

RegenerationPowerupItem::RegenerationPowerupItem()
{
	nTexture = TextureID::RegenerationPowerup;
	nCount = 1;
	nType = Type::RegenerationPowerup;
	strName = "Regeneration Powerup";
}

void RegenerationPowerupItem::Use(float fX, float fY, float fAngle)
{
	if (Game::sgSpaceGame->plPlayer->puCurrentPowerup == nullptr)
	{
		nCount--;
		Game::sgSpaceGame->plPlayer->puCurrentPowerup = std::make_shared<RegenerationPowerup>();
	}
}
