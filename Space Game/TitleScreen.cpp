#include "TitleScreen.h"
#include "Game.h"
#include "Space.h"
#include <d2d1.h>

static D2D1::ColorF clrDarkGrey = D2D1::ColorF::DarkGray;
static D2D1::ColorF clrWhite = D2D1::ColorF::White;
static D2D1::ColorF clrBlack = D2D1::ColorF(0.0f, 0.0f, 0.0f);


Texture* tButtons;

TitleScreen::TitleScreen()
{
	nButtonHover = -1;
	tButtons = new Texture(L"buttons.png", 1908, 857, 280, 125);

	vButtons.push_back(Button(500, 200, 780, 325, [] { Game::LoadLevel(new SpaceGame()); }, L"Play"));
	vButtons.push_back(Button(500, 500, 780, 625, [] { PostQuitMessage(0); }, L"Quit"));
	vButtons.push_back(Button(500, 350, 780, 475, [] { MessageBox(Graphics::hWindow, L"Space Game\nVersion Alpha 0.1.1\nBy Joel Kruger\nand Isaac Dyt", L"Info", MB_OK); }, L"Info"));
}

void TitleScreen::Render()
{
	Graphics::Clear(clrBlack);

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
