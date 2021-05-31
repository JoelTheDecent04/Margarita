#include "ControlsScreen.h"
#include "PauseScreen.h"
#include "Graphics.h"
#include "Player.h"

static D2D1::ColorF clrBlack = D2D1::ColorF::Black;
static D2D1::ColorF clrDarkGrey = D2D1::ColorF::DarkGray;

ControlsScreen::ControlsScreen(PauseScreen* lPauseScreen)
{
	this->lPauseScreen = lPauseScreen;
}

void ControlsScreen::Load()
{
	vControls.push_back(Control(&keyJump1, &keyJump2, L"Jump"));
	vControls.push_back(Control(&keyMoveLeft1, &keyMoveLeft2, L"Move Left"));
	vControls.push_back(Control(&keyMoveRight1, &keyMoveRight2, L"Move Right"));
	vControls.push_back(Control(&keyMoveDown1, &keyMoveDown2, L"Move Down"));
	vControls.push_back(Control(&keyOpenShop1, &keyOpenShop2, L"Open Shop"));

	vButtons.push_back(Button(500, 600, 780, 680, [](void* c) {((ControlsScreen*)c)->Resume(); }, L"Back"));
	keyToChange = nullptr;
}

void ControlsScreen::Unload()
{
}

void ControlsScreen::Render()
{
	Graphics::Clear(clrBlack);

	DWRITE_TEXT_METRICS tmTextMetrics;
	Graphics::TextMetrics(L"Controls", fScaleV * 44.0f, tmTextMetrics);
	Graphics::WriteText(L"Controls", fScaleH * 640 - tmTextMetrics.width / 2, fScaleV * 70.0f, fScaleV * 44.0f);

	int nY = 200;
	for (Control& control : vControls)
	{
		Graphics::WriteText(control.strDescription, fScaleH * 200, fScaleV * nY, fScaleV * 30.0f);

		std::wstring strKey1 = KeyText(*control.nKey1);
		if (control.nKey1 == keyToChange)
		{
			strKey1.insert(0, 1, '[');
			strKey1 += ']';
		}
		std::wstring strKey2 = KeyText(*control.nKey2);
		if (control.nKey2 == keyToChange)
		{
			strKey2.insert(0, 1, '[');
			strKey2 += ']';
		}

		Graphics::DrawRectangle(fScaleH * 450, fScaleV * nY, fScaleH * 100, fScaleV * 50, clrDarkGrey);
		Graphics::DrawRectangle(fScaleH * 560, fScaleV * nY, fScaleH * 100, fScaleV * 50, clrDarkGrey);

		Graphics::TextMetrics(strKey1.c_str(), fScaleV * 24.0f, tmTextMetrics);
		Graphics::WriteText(strKey1.c_str(), fScaleH * 500.0f - (tmTextMetrics.width / 2), fScaleV * (nY + 25) - (tmTextMetrics.height / 2), fScaleV * 24.0f);
		Graphics::TextMetrics(strKey2.c_str(), fScaleV * 24.0f, tmTextMetrics);
		Graphics::WriteText(strKey2.c_str(), fScaleH * 610.0f - (tmTextMetrics.width / 2), fScaleV * (nY + 25) - (tmTextMetrics.height / 2), fScaleV * 24.0f);
		nY += 60;
	}

	int i = 0;
	for (Button& button : vButtons)
	{
		button.Draw(nButtonHover == i);
		i++;
	}
}

void ControlsScreen::Update(double deltatime)
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

void ControlsScreen::Resume()
{
	Game::LoadLevel(lPauseScreen, true, false);
}

void ControlsScreen::LeftClick()
{
	POINT pntCursorPosition;
	GetCursorPos(&pntCursorPosition);
	ScreenToClient(Graphics::hWindow, &pntCursorPosition);
	pntCursorPosition.x /= fScaleH;
	pntCursorPosition.y /= fScaleV;

	if (nButtonHover != -1)
		vButtons[nButtonHover].function(this);

	int nY = 200;
	for (Control& control : vControls)
	{
		RECT rect1 = { 450, nY, 550, nY + 50 };
		RECT rect2 = { 560, nY, 660, nY + 50 };
		if (PtInRect(&rect1, pntCursorPosition))
		{
			keyToChange = control.nKey1;
			break;
		}
		if (PtInRect(&rect2, pntCursorPosition))
		{
			keyToChange = control.nKey2;
			break;
		}
		nY += 60;
	}
}

void ControlsScreen::KeyDown(int key)
{
	if (key == VK_ESCAPE)
	{
		Resume();
		return;
	}

	if (keyToChange != nullptr)
	{
		*keyToChange = key;
		keyToChange = nullptr;
	}
}

std::wstring ControlsScreen::KeyText(int nKey)
{
	if (nKey >= 'A' && nKey <= 'Z')
	{
		return std::wstring(1, (char)nKey);
	}
	if (nKey >= '0' && nKey <= '9')
	{
		return std::wstring(1, (char)nKey);
	}
	if (nKey == 0) return L"";
	if (nKey == VK_SPACE) return L"SPACE";
	if (nKey == VK_LEFT) return L"LEFT";
	if (nKey == VK_RIGHT) return L"RIGHT";
	if (nKey == VK_UP) return L"UP";
	if (nKey == VK_DOWN) return L"DOWN";

	return L"Unknown";
}
