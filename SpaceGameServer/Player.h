#pragma once
#include "Entity.h"
#include "Space.h"
#include "Powerup.h"
#include <string>
#include <memory>

class Player : public Entity
{
public:
	std::shared_ptr<Powerup> puCurrentPowerup;
	std::vector<std::shared_ptr<Item>> vItems;

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
	Player(float fX, float fY);
	bool Update(float deltatime) override;
	void Destroy() override;
};

extern int keyJump1, keyJump2, keyMoveLeft1, keyMoveLeft2, keyMoveRight1, keyMoveRight2, keyMoveDown1, keyMoveDown2;