#pragma once
#include "Game.h"
#include "GUI.h"
#include <vector>

class TitleScreen : public Level
{
	//std::vector<Button> vButtons;
	WidgetGroup gui;
	
public:
	TitleScreen();
	~TitleScreen();
	void Render() override;
	void Update(float deltatime) override;
	void LeftClick() override;
};