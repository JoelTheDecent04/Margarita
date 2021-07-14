#include "PauseScreen.h"
#include "Game.h"
#include "Space.h"
#include "TitleScreen.h"
#include "ControlsScreen.h"
#include "Utilities.h"
#include "Colours.h"

using namespace Game;

PauseScreen::PauseScreen()
{
	lPrevLevel = lCurrentLevel;
	nButtonHover = -1;
	vButtons.push_back(Button(500, 150, 780, 250, [](void* p) { LoadLevel(((PauseScreen*)p)->lPrevLevel); }, "Resume"));
	vButtons.push_back(Button(500, 275, 780, 375, [](void* p) { 
		//std::shared_ptr<ControlsScreen> pControlsScreen = std::make_shared<ControlsScreen>();
		LoadLevel(std::make_shared<ControlsScreen>());
	}, "Controls")); //TODO fix
	vButtons.push_back(Button(500, 400, 780, 500, [](void* p) { LoadLevel(std::static_pointer_cast<Level>(std::make_shared<TitleScreen>()));}, "Title Screen"));
	vButtons.push_back(Button(500, 525, 780, 625, [](void* p) { Quit(); }, "Quit"));
}

void PauseScreen::Render()
{
	if (lPrevLevel)
		lPrevLevel->Render();
	Graphics::FillRectangle(0.0f, 0.0f, fScaleH * 1280.0f, fScaleV * 720.0f, clrBlack, 0.7f);

	TextSize textsize;
	Graphics::TextMetrics("Game Paused", Graphics::pFont44Relative, textsize);
	Graphics::WriteText("Game Paused", fScaleH * 640 - textsize.width / 2, fScaleV * 70, Graphics::pFont44Relative);

	for (int i = 0; i < vButtons.size(); i++)
	{
		Button& button = vButtons[i];
		Graphics::FillRectangle(fScaleH * button.rect.left, fScaleV * button.rect.top, fScaleH * (button.rect.right - button.rect.left), fScaleV * (button.rect.bottom - button.rect.top), i == nButtonHover ? clrDarkGrey : clrBlack);
		Graphics::DrawRectangle(fScaleH * button.rect.left, fScaleV * button.rect.top, fScaleH * (button.rect.right - button.rect.left), fScaleV * (button.rect.bottom - button.rect.top), clrDarkGrey);
		Graphics::TextMetrics(button.text, Graphics::pFont24Relative, textsize);
		Graphics::WriteText(button.text,
			fScaleH * ((button.rect.right + button.rect.left) / 2) - textsize.width / 2,
			fScaleV * ((button.rect.bottom + button.rect.top) / 2) - textsize.height / 2,
			Graphics::pFont24Relative);
	}
}

void PauseScreen::Update(float deltatime)
{
	int nCursorX, nCursorY;
	GetRelativeMousePos(&nCursorX, &nCursorY);

	bool bMouseOverButton = false;
	int i = 0;
	for (Button& button : vButtons)
	{
		if (PointInRect(button.rect, nCursorX, nCursorY))
		{
			nButtonHover = i;
			bMouseOverButton = true;
			break;
		}
		i++;
	}
	if (!bMouseOverButton) nButtonHover = -1;
}

void PauseScreen::LeftClick()
{
	if (nButtonHover != -1)
		vButtons[nButtonHover].function(this);
}

void PauseScreen::KeyDown(int key)
{
	if (key == SDL_SCANCODE_ESCAPE)
	{
		LoadLevel(lPrevLevel);
	}
}
