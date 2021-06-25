#pragma once
#include "Game.h"
#include "Space.h"
#include "GUI.h"
#include <vector>

class DeathScreen : public Level
{
	SpaceGame* sgSpaceGame;
	std::vector<Button> vButtons;
	int nButtonHover;
public:
	DeathScreen(SpaceGame* sgSpaceGame) { this->sgSpaceGame = sgSpaceGame; };
	void Load();
	void Unload() override;
	void Render() override;
	void Update(float deltatime) override;
	void LeftClick() override;
};