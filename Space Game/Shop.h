#pragma once
#include "Game.h"
#include "Space.h"
#include "GUI.h"
#include <Windows.h>
#include <vector>
#include <d2d1.h>

class ShopScreen : public Level
{
public:
	int nUpgrades;
	std::vector<Button> vButtons;
	std::vector<Bar> vBars;
	int nButtonHover;
	SpaceGame* lGameLevel;
	ShopScreen(SpaceGame* lGameLevel);
	void Load() override {};
	void Unload() override {};
	void Render() override;
	void Update(double deltatime) override;
	void LeftClick() override;
	void Resume();
	void UpgradeHealth();
	void UpgradeEnergyRecharge();
};