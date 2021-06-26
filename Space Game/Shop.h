#pragma once
#include "Game.h"
#include "Space.h"
#include "GUI.h"
#include <vector>

class ShopScreen : public Level
{
public:
	int nUpgrades;
	std::vector<Button> vButtons;
	std::vector<Bar> vBars;
	int nButtonHover;
	std::shared_ptr<SpaceGame> lGameLevel;
	ShopScreen();
	void Render() override;
	void Update(float deltatime) override;
	void LeftClick() override;
	void KeyDown(int key) override;
	void Resume();

	void UpgradeHealth();
	void UpgradeEnergyRecharge();
	void UpgradeMovementSpeed();
	void BuyLaserUpgrade();
	void BuyEnergyPowerup();
	void BuyRegenerationPowerup();
	void BuyBombUpgrade();
	void UpgradeRegeneration();
};