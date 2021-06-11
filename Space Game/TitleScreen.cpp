#include "TitleScreen.h"
#include "Game.h"
#include "Space.h"
#include <d2d1.h>

static D2D1::ColorF clrDarkGrey = D2D1::ColorF::DarkGray;
static D2D1::ColorF clrWhite = D2D1::ColorF::White;
static D2D1::ColorF clrBlack = D2D1::ColorF(0.0f, 0.0f, 0.0f);
static D2D1::ColorF clrRed = D2D1::ColorF::Red;

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

	DWRITE_TEXT_METRICS tmTextMetrics;
	Graphics::TextMetrics(L"Margarita", fScaleH * 125.0f, tmTextMetrics, L"Chiller");
	Graphics::WriteText(L"Margarita", fScaleH * 640 - tmTextMetrics.width / 2, fScaleV * 30, fScaleH * 125.0f, clrRed, 1.0f, L"Chiller");

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

void TitleScreen::LeftClick()
{
	if (nButtonHover != -1)
		vButtons[nButtonHover].function();
}
