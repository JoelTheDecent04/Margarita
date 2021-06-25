#pragma once
#include "Game.h"
#include "GUI.h"
#include <vector>

class TitleScreen : public Level
{
	std::vector<Button> vButtons;
	int nButtonHover;
public:
	TitleScreen();
	void Load() override {};
	void Unload() override {};
	void Render() override;
	void Update(float deltatime) override;
	void LeftClick() override;
};