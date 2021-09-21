#pragma once
#include "Game.h"

#ifdef SUPPORT_MULTIPLAYER
#include "GUI.h"

class JoinServerScreen : public Level
{
	WidgetGroup gui;
	static std::shared_ptr<WidgetTextbox> ip_input_textbox;
public:
	JoinServerScreen();
	~JoinServerScreen();
	void Render() override;
	void Update(float deltatime) override;
	void LeftClick() override;
	void KeyDown(int key) override;
	void KeyPress(const char* keys) override;

	void Connect();
};

#endif