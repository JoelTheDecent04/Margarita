#pragma once
#include "Game.h"
#include <vector>
#include <Windows.h>

class PauseScreen : public Level
{
public:
	Level* lPrevLevel;
	class Button
	{
	public:
		RECT rect;
		void (*function)(PauseScreen*);
		const wchar_t* text;
		Button(int left, int top, int right, int bottom, void (*function)(PauseScreen*), const wchar_t* text)
		{
			this->rect = { left, top, right, bottom }; this->function = function; this->text = text;
		};
	};
	std::vector<Button> vButtons;
	int nButtonHover;

	PauseScreen(Level* lPrevLevel);
	void Resume();
	void ReturnToTitleScreen();
	void GoToControlsScreen();
	void Quit();
	void Load() override;
	void Unload() override;
	void Render() override;
	void Update(double deltatime) override;
	void LeftClick() override;
	void KeyDown(int key) override;
};