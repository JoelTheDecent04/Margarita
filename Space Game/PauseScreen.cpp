#include "PauseScreen.h"
#include "Game.h"
#include "Space.h"
#include "TitleScreen.h"
#include "ControlsScreen.h"
#include "Utilities.h"
#include "Colours.h"

PauseScreen::PauseScreen(Level* lPrevLevel)
{
	this->lPrevLevel = lPrevLevel;
	nButtonHover = -1;
	vButtons.push_back(Button(500, 150, 780, 250, [](PauseScreen* p) { Game::LoadLevel(p->lPrevLevel, true, false); }, L"Resume"));
	vButtons.push_back(Button(500, 275, 780, 375, [](PauseScreen* p) { Game::LoadLevel(new ControlsScreen(p), false, true); }, L"Controls"));
	vButtons.push_back(Button(500, 400, 780, 500, [](PauseScreen* p) { Game::LoadLevel(new TitleScreen, true, true);}, L"Title Screen"));
	vButtons.push_back(Button(500, 525, 780, 625, [](PauseScreen* p) { Game::Quit(); }, L"Quit"));
}

void PauseScreen::Load(){}

void PauseScreen::Unload() { delete this; }

void PauseScreen::Render()
{
	if (lPrevLevel)
		lPrevLevel->Render();
	Graphics::FillRectangle(0.0f, 0.0f, fScaleH * 1280.0f, fScaleV * 720.0f, clrBlack, 0.7f);

	TextSize textsize;
	Graphics::TextMetrics(L"Game Paused", fScaleV * 44.0f, textsize);
	Graphics::WriteText(L"Game Paused", fScaleH * 640 - textsize.width / 2, fScaleV * 70, fScaleV * 44.0f);

	for (int i = 0; i < vButtons.size(); i++)
	{
		Button& button = vButtons[i];
		Graphics::FillRectangle(fScaleH * button.rect.left, fScaleV * button.rect.top, fScaleH * (button.rect.right - button.rect.left), fScaleV * (button.rect.bottom - button.rect.top), i == nButtonHover ? clrDarkGrey : clrBlack);
		Graphics::DrawRectangle(fScaleH * button.rect.left, fScaleV * button.rect.top, fScaleH * (button.rect.right - button.rect.left), fScaleV * (button.rect.bottom - button.rect.top), clrDarkGrey);
		Graphics::TextMetrics(button.text, fScaleV * 24.0f, textsize);
		Graphics::WriteText(button.text,
			fScaleH * ((button.rect.right + button.rect.left) / 2) - textsize.width / 2,
			fScaleV * ((button.rect.bottom + button.rect.top) / 2) - textsize.height / 2,
			fScaleV * 24.0f);
	}
}

void PauseScreen::Update(double deltatime)
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
	if (key == VK_ESCAPE)
	{
		Game::LoadLevel(lPrevLevel, true, false);
	}
}
