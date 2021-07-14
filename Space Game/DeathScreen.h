#pragma once
#include "Game.h"
#include "Space.h"
#include "GUI.h"
#include <vector>

class DeathScreen : public Level
{
	std::vector<Button> vButtons;
	int nButtonHover;
public:
	DeathScreen();
	~DeathScreen();
	void Render() override;
	void Update(float deltatime) override;
	void LeftClick() override;
};