#pragma once
#include "Entity.h"
#include "Space.h"
#include "Powerup.h"
#include <string>
#include <concurrent_vector.h>

class Player : public Entity
{
public:
	std::wstring sName;

	Powerup* puCurrentPowerup;

	float fMoney;
	float nEnergy;
	float nMaxEnergy;
	float fEnergyRechargeSpeed;
	float fMaxEnergyRechargeSpeed;
	float fMaxHealthUpgrade;
	float fMovementSpeed;
	float fMaxMovementSpeed;
	Player(SpaceGame* game, float fX, float fY, Texture* tTexture, std::wstring sName);
	bool Update(double deltatime) override;
};

extern int keyJump1, keyJump2, keyMoveLeft1, keyMoveLeft2, keyMoveRight1, keyMoveRight2, keyMoveDown1, keyMoveDown2;