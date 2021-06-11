#pragma once
#include "Entity.h"
#include "Space.h"
#include "Powerup.h"
#include <string>
#include <concurrent_vector.h>

class Player : public Entity
{
public:
	Powerup* puCurrentPowerup;

	float fMoney;
	float nEnergy;
	float nMaxEnergy;
	float fEnergyRechargeSpeed;
	float fMaxEnergyRechargeSpeed;
	float fMaxHealthUpgrade;
	float fMovementSpeed;
	float fMaxMovementSpeed;
	float fHealthRegeneration;
	float fMaxHealthRegeneration;
	Player(SpaceGame* game, float fX, float fY);
	bool Update(double deltatime) override;
	void Save(std::fstream& f) override;
	void Load(std::fstream& f) override;
};

extern int keyJump1, keyJump2, keyMoveLeft1, keyMoveLeft2, keyMoveRight1, keyMoveRight2, keyMoveDown1, keyMoveDown2;