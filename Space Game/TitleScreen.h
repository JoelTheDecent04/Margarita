#pragma once
#include "Game.h"
#include "GUI.h"
#include <vector>

class TitleScreen : public Level
{
	/*class Button
	{
	public:
		Rect rect;
		void (*function)();
		const wchar_t* text;
		Button(int left, int top, int right, int bottom, void (*function)(), const wchar_t* text) 
		{
			this->rect = { left, top, right, bottom }; this->function = function; this->text = text; 
		};
	};*/
	std::vector<Button> vButtons;
	int nButtonHover;
public:
	TitleScreen();
	void Load() override {};
	void Unload() override {};
	void Render() override;
	void Update(double deltatime) override;
	void LeftClick() override;
};