#pragma once
#include "Game.h"
#include "GUI.h"
#include <vector>

class PauseScreen : public Level
{
public:
	Level* lPrevLevel;
	std::vector<Button> vButtons;
	int nButtonHover;

	PauseScreen(Level* lPrevLevel);
	void Load() override;
	void Unload() override;
	void Render() override;
	void Update(float deltatime) override;
	void LeftClick() override;
	void KeyDown(int key) override;
};