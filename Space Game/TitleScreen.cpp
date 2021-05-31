#include "TitleScreen.h"
#include "Game.h"
#include "Space.h"
#include <d2d1.h>

static D2D1::ColorF clrDarkGrey = D2D1::ColorF::DarkGray;
static D2D1::ColorF clrBlack = D2D1::ColorF(0.0f, 0.0f, 0.0f);

TitleScreen::TitleScreen()
{
	nButtonHover = -1;
	vButtons.push_back(Button(500, 200, 780, 300, [] { Game::LoadLevel(new SpaceGame()); }, L"Play"));
	vButtons.push_back(Button(500, 325, 780, 425, [] { MessageBox(Graphics::hWindow, L"Space Game\nVersion Alpha 0.0.8\nBy Joel Kruger\nand Isaac Dyt", L"Info", MB_OK); }, L"Info"));
	vButtons.push_back(Button(500, 450, 780, 550, [] { PostQuitMessage(0); }, L"Quit"));
}

void TitleScreen::Render()
{
	Graphics::Clear(clrBlack);
	if (nButtonHover != -1)
	{
		Button& button = vButtons[nButtonHover];
		Graphics::FillRectangle(fScaleH * button.rect.left, fScaleV * button.rect.top, fScaleH * (button.rect.right - button.rect.left), fScaleV * (button.rect.bottom - button.rect.top), clrDarkGrey);
	}
	for (Button& button : vButtons)
	{
		Graphics::DrawRectangle(fScaleH * button.rect.left, fScaleV * button.rect.top, fScaleH * (button.rect.right - button.rect.left), fScaleV * (button.rect.bottom - button.rect.top), clrDarkGrey);
		DWRITE_TEXT_METRICS tmTextMetrics;
		Graphics::TextMetrics(button.text, fScaleV * 24.0f, tmTextMetrics);
		Graphics::WriteText(button.text,
			fScaleH * ((button.rect.right + button.rect.left) / 2) - tmTextMetrics.width / 2,
			fScaleV * ((button.rect.bottom + button.rect.top) / 2) - tmTextMetrics.height / 2, 
			fScaleV * 24.0f);
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
