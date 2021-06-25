#pragma once
#include "Game.h"
#include "GUI.h"
#include <vector>
#include <string>

class PauseScreen;

class ControlsScreen : public Level
{
	PauseScreen* lPauseScreen;
	struct Control
	{
		int* nKey1, * nKey2;
		const char* strDescription;
		Control(int* nKey1, int* nKey2, const char* strDescription) { this->nKey1 = nKey1; this->nKey2 = nKey2; this->strDescription = strDescription; };
	};
	std::vector<Control> vControls;
	std::vector<Button> vButtons;
	int nButtonHover;

	int* keyToChange;

public:
	ControlsScreen(PauseScreen* lPauseScreen);
	void Load() override;
	void Unload() override;
	void Render() override;
	void Update(float deltatime) override;
	void Resume();
	void KeyDown(int key) override;
	void LeftClick() override;
	static const char* KeyText(int nKey);
};