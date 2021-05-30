#include "PauseScreen.h"
#include "Game.h"
#include "Space.h"
#include "TitleScreen.h"

static D2D1::ColorF clrDarkGrey = D2D1::ColorF::DarkGray;
static D2D1::ColorF clrWhite = D2D1::ColorF::White;
static D2D1::ColorF clrBlack = D2D1::ColorF(0.0f, 0.0f, 0.0f);

PauseScreen::PauseScreen(Level* lPrevLevel)
{
	this->lPrevLevel = lPrevLevel;
	nButtonHover = -1;
	vButtons.push_back(Button(500, 200, 780, 300, [](PauseScreen* p) { p->Resume(); }, L"Resume"));
	vButtons.push_back(Button(500, 325, 780, 425, [](PauseScreen* p) { p->ReturnToTitleScreen(); }, L"Title Screen"));
	vButtons.push_back(Button(500, 450, 780, 550, [](PauseScreen* p) { p->Quit(); }, L"Quit"));
	bRenderedFirstFrame = false;
}

void PauseScreen::Resume()
{
	Game::LoadLevel(lPrevLevel, true, false);
}

void PauseScreen::ReturnToTitleScreen()
{
	lPrevLevel->Unload();
	Game::LoadLevel(new TitleScreen());
}

void PauseScreen::Quit()
{
	PostQuitMessage(0);
}

void PauseScreen::Load()
{
}

void PauseScreen::Unload()
{
}

void PauseScreen::Render()
{
	if (!bRenderedFirstFrame)
	{
		Graphics::FillRectangle(0.0f, 0.0f, fScaleH * 1280.0f, fScaleV * 720.0f, clrBlack, 0.7f);
		bRenderedFirstFrame = true;
	}

	DWRITE_TEXT_METRICS tmTextMetrics;
	Graphics::TextMetrics(L"Game Paused", fScaleV * 36.0f, tmTextMetrics);
	Graphics::WriteText(L"Game Paused", fScaleH * 640 - tmTextMetrics.width / 2, fScaleV * 100, fScaleV * 36.0f);

	for (int i = 0; i < vButtons.size(); i++)
	{
		Button& button = vButtons[i];
		Graphics::FillRectangle(fScaleH * button.rect.left, fScaleV * button.rect.top, fScaleH * (button.rect.right - button.rect.left), fScaleV * (button.rect.bottom - button.rect.top), i == nButtonHover ? clrDarkGrey : clrBlack);
		Graphics::DrawRectangle(fScaleH * button.rect.left, fScaleV * button.rect.top, fScaleH * (button.rect.right - button.rect.left), fScaleV * (button.rect.bottom - button.rect.top), clrDarkGrey);
		DWRITE_TEXT_METRICS tmTextMetrics;
		Graphics::TextMetrics(button.text, fScaleV * 24.0f, tmTextMetrics);
		Graphics::WriteText(button.text,
			fScaleH * ((button.rect.right + button.rect.left) / 2) - tmTextMetrics.width / 2,
			fScaleV * ((button.rect.bottom + button.rect.top) / 2) - tmTextMetrics.height / 2,
			fScaleV * 24.0f);
	}
}

void PauseScreen::Update(double deltatime)
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

void PauseScreen::LeftClick()
{
	if (nButtonHover != -1)
		vButtons[nButtonHover].function(this);
}
