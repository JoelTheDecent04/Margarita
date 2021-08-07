#include "Player.h"
#include "Space.h"
#include "Bomb.h"

Player::Player(float fX, float fY)
	: Entity(fX, fY)
{
	fHealth = 200.0f;
	fMaxHealth = 200.0f;
	nEnergy = 150.0f;
	nMaxEnergy = 150.0f;
	fMoney = 0.0f;
	fEnergyRechargeSpeed = 3.0f;
	fMaxEnergyRechargeSpeed = 9.0f;
	fMaxHealthUpgrade = 450.0f;
	fWidth = 50.0f;
	fHeight = 58.0f;
	fMovementSpeed = 150.0f;
	fMaxMovementSpeed = 190.0f;
	puCurrentPowerup = nullptr;
	fHealthRegeneration = 2.0f;
	fMaxHealthRegeneration = 5.0f;

	nType = Type::Player;
}

bool Player::Update(float deltatime)
{
	if (fHealth <= 0.0f)
	{
		Destroy();
		return false;
	}

	if (fSpeedX > 0)
	{
		fSpeedX -= fPlayerDecceleration * deltatime;
		if (fSpeedX < 0)
			fSpeedX = 0;
	}
	if (fSpeedX < 0)
	{
		fSpeedX += fPlayerDecceleration * deltatime;
		if (fSpeedX > 0)
			fSpeedX = 0;
	}
	if (!bOnGround)
		fSpeedY += fGravity * deltatime;

	fX += fSpeedX * deltatime;
	fY += fSpeedY * deltatime;

	if (fX < 0)
		fX = 0;
	if (fX > 5120)
		fX = 5120;
	if (fY <= 0.0f)
	{
		fY = 0.0f;
		fSpeedY = 0.0f;
	}
	if (fY >= 594.0f - fHeight / 2)
	{
		fY = 594.0f - fHeight / 2;
		fSpeedY = 0.0f;
		bOnGround = true;
	}

	nEnergy += (bOnGround ? fEnergyRechargeSpeed : fEnergyRechargeSpeed / 2.0f) * deltatime;
	if (nEnergy > nMaxEnergy) nEnergy = nMaxEnergy;
	fHealth += fHealthRegeneration * deltatime;
	if (fHealth > fMaxHealth) fHealth = fMaxHealth;

	if (fMoney >= 150.0f && vItems.size() <= 2) vItems.push_back(std::static_pointer_cast<Item, BombWeapon>(std::make_shared<BombWeapon>(1))); //TODO improve

	return true;
}

void Player::Destroy()
{
	Game::sgSpaceGame->nPlayers--;
}
