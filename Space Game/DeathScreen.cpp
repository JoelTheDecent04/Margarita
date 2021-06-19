#include "DeathScreen.h"
#include "Game.h"
#include "Space.h"
#include "Colours.h"
#include <fstream>

void DeathScreen::Render()
{
	if (!bRenderedFirstFrame)
	{
		Graphics::FillRectangle(0.0f, 0.0f,nScreenWidth, nScreenHeight, clrBlack, 0.7f);
		bRenderedFirstFrame = true;
	}

	DWRITE_TEXT_METRICS tmTextMetrics;
	Graphics::TextMetrics(L"You Suck!", fScaleV * 36.0f, tmTextMetrics);
	Graphics::WriteText(L"You Suck!", fScaleH * 640 - (tmTextMetrics.width / 2), 100, fScaleV * 36.0f);

	for (int i = 0; i < vButtons.size(); i++)
		vButtons[i].Draw(i == nButtonHover);
}

void DeathScreen::Update(double deltatime)
{
	POINT pntCursorPosition;
	GetCursorPos(&pntCursorPosition);
	ScreenToClient(Graphics::hWindow, &pntCursorPosition);
	pntCursorPosition.x /= fScaleH;
	pntCursorPosition.y /= fScaleV;

	bool bMouseOverButton = false;
	int i = 0;
	for (Button& button : vButtons)
	{
		if (PtInRect(&button.rect, pntCursorPosition))
		{
			nButtonHover = i;
			bMouseOverButton = true;
			break;
		}
		i++;
	}
	if (!bMouseOverButton) nButtonHover = -1;
}

void DeathScreen::LeftClick()
{
	if (nButtonHover != -1)
		vButtons[nButtonHover].function(this);
}

void DeathScreen::Load()
{
	nButtonHover = -1;
	vButtons.push_back(Button(500, 200, 780, 300, [](void*) { Game::LoadLevel(new SpaceGame()); }, L"New Game"));
	vButtons.push_back(Button(500, 320, 780, 420, [](void*) { Game::Quit(); }, L"Quit"));
	bRenderedFirstFrame = false;

	remove("savegame.txt");
}