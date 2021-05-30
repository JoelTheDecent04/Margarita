#pragma once
#include "Entity.h"
#include "Space.h"
#include <string>

class Player : public Entity
{
public:
	std::wstring sName;
	float fMoney;
	float nEnergy;
	float nMaxEnergy;
	float fEnergyRechargeSpeed;
	float fMaxEnergyRechargeSpeed;
	float fMaxHealthUpgrade;
	Player(SpaceGame* game, float fX, float fY, Texture* tTexture, std::wstring sName);
	void Update(double deltatime) override;
	void Destroy() override;
};