#include "TitleScreen.h"
#include "Game.h"
#include "Space.h"
#include "Utilities.h"
#include "Colours.h"
#include <d2d1.h>

static Texture* tButtons;
static Texture* tTitlescreenBackground;

TitleScreen::TitleScreen()
{
	nButtonHover = -1;
	tButtons = new Texture(L"buttons.png", 1908, 857, 280, 125);

	vButtons.push_back(Button(500, 200, 780, 325, [] {
		if (Game::sgSpaceGame)
			Game::LoadLevel(Game::sgSpaceGame, true, false);
		else
			Game::LoadLevel(new SpaceGame());
	}, L"Play"));
	vButtons.push_back(Button(500, 500, 780, 625, [] { Game::Quit(); }, L"Quit"));
	vButtons.push_back(Button(500, 350, 780, 475, [] {  }, L"Info"));

	tTitlescreenBackground = new Texture(L"titlescreen_background.png", 3022, 1700, 1280.0f, 720.0f);
}

void TitleScreen::Render()
{
	tTitlescreenBackground->Draw(0, 0.0f, 0.0f);

	TextSize textsize;
	Graphics::TextMetrics(L"Margarita", fScaleV * 125.0f, textsize, L"Chiller");
	Graphics::WriteText(L"Margarita", fScaleH * 640 - textsize.width / 2, fScaleV * 30, fScaleV * 125.0f, clrRed, 1.0f, L"Chiller");

	int nIndex = 0;
	for (Button& button : vButtons)
	{
		tButtons->Draw(nIndex, button.rect.left, button.rect.top);
		if (nButtonHover == nIndex)
		{
			Graphics::FillRectangle(fScaleH * button.rect.left, fScaleV * button.rect.top, fScaleH * (button.rect.right - button.rect.left), fScaleV * (button.rect.bottom - button.rect.top), clrBlack, 0.35f);
		}
		nIndex++;
	}
}

void TitleScreen::Update(double deltatime)
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

void TitleScreen::LeftClick()
{
	if (nButtonHover != -1)
		vButtons[nButtonHover].function();
}
