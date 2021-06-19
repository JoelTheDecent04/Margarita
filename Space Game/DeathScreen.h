#pragma once
#include "Game.h"
#include "GUI.h"
#include <vector>

class DeathScreen : public Level
{
	std::vector<Button> vButtons;
	int nButtonHover;
	bool bRenderedFirstFrame;
public:
	DeathScreen() {};
	void Load();
	void Unload() override {};
	void Render() override;
	void Update(double deltatime) override;
	void LeftClick() override;
};