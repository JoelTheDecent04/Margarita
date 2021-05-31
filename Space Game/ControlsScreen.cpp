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

	vControls.push_back(Control(&keyChangeWeapon1[0], &keyChangeWeapon2[0], L"Slot 1"));
	vControls.push_back(Control(&keyChangeWeapon1[1], &keyChangeWeapon2[1], L"Slot 2"));
	vControls.push_back(Control(&keyChangeWeapon1[2], &keyChangeWeapon2[2], L"Slot 3"));
	vControls.push_back(Control(&keyChangeWeapon1[3], &keyChangeWeapon2[3], L"Slot 4"));
	vControls.push_back(Control(&keyChangeWeapon1[4], &keyChangeWeapon2[4], L"Slot 5"));
	vControls.push_back(Control(&keyChangeWeapon1[5], &keyChangeWeapon2[5], L"Slot 6"));
	vControls.push_back(Control(&keyChangeWeapon1[6], &keyChangeWeapon2[6], L"Slot 7"));
	vControls.push_back(Control(&keyChangeWeapon1[7], &keyChangeWeapon2[7], L"Slot 8"));
	vControls.push_back(Control(&keyChangeWeapon1[8], &keyChangeWeapon2[8], L"Slot 9"));

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
	Graphics::WriteText(L"Controls", fScaleH * 640 - tmTextMetrics.width / 2, fScaleV * 40.0f, fScaleV * 40.0f);

	int nY = 100;
	for (Control& control : vControls)
	{
		Graphics::WriteText(control.strDescription, fScaleH * 200, fScaleV * nY, fScaleV * 24.0f);

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

		Graphics::DrawRectangle(fScaleH * 450, fScaleV * nY, fScaleH * 100, fScaleV * 28, clrDarkGrey);
		Graphics::DrawRectangle(fScaleH * 560, fScaleV * nY, fScaleH * 100, fScaleV * 28, clrDarkGrey);

		Graphics::TextMetrics(strKey1.c_str(), fScaleV * 20.0f, tmTextMetrics);
		Graphics::WriteText(strKey1.c_str(), fScaleH * 500.0f - (tmTextMetrics.width / 2), fScaleV * (nY + 14) - (tmTextMetrics.height / 2), fScaleV * 20.0f);
		Graphics::TextMetrics(strKey2.c_str(), fScaleV * 20.0f, tmTextMetrics);
		Graphics::WriteText(strKey2.c_str(), fScaleH * 610.0f - (tmTextMetrics.width / 2), fScaleV * (nY + 14) - (tmTextMetrics.height / 2), fScaleV * 20.0f);
		nY += 35;
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

	int nY = 100;
	for (Control& control : vControls)
	{
		RECT rect1 = { 450, nY, 550, nY + 28 };
		RECT rect2 = { 560, nY, 660, nY + 28 };
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
		nY += 35;
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
