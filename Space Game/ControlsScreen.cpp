#include "ControlsScreen.h"
#include "PauseScreen.h"
#include "Graphics.h"
#include "Player.h"
#include "Utilities.h"
#include "Colours.h"

using namespace Game;

ControlsScreen::ControlsScreen()
{
	this->lPreviousLevel = Game::lCurrentLevel;

	vControls.push_back(Control(&keyJump1, &keyJump2, "Jump"));
	vControls.push_back(Control(&keyMoveLeft1, &keyMoveLeft2, "Move Left"));
	vControls.push_back(Control(&keyMoveRight1, &keyMoveRight2, "Move Right"));
	vControls.push_back(Control(&keyMoveDown1, &keyMoveDown2, "Move Down"));
	vControls.push_back(Control(&keyOpenShop1, &keyOpenShop2, "Open Shop"));
	vControls.push_back(Control(&keyNextWave1, &keyNextWave2, "Next Wave"));

	vControls.push_back(Control(&keyChangeWeapon1[0], &keyChangeWeapon2[0], "Slot 1"));
	vControls.push_back(Control(&keyChangeWeapon1[1], &keyChangeWeapon2[1], "Slot 2"));
	vControls.push_back(Control(&keyChangeWeapon1[2], &keyChangeWeapon2[2], "Slot 3"));
	vControls.push_back(Control(&keyChangeWeapon1[3], &keyChangeWeapon2[3], "Slot 4"));
	vControls.push_back(Control(&keyChangeWeapon1[4], &keyChangeWeapon2[4], "Slot 5"));
	vControls.push_back(Control(&keyChangeWeapon1[5], &keyChangeWeapon2[5], "Slot 6"));
	vControls.push_back(Control(&keyChangeWeapon1[6], &keyChangeWeapon2[6], "Slot 7"));
	vControls.push_back(Control(&keyChangeWeapon1[7], &keyChangeWeapon2[7], "Slot 8"));
	//vControls.push_back(Control(&keyChangeWeapon1[8], &keyChangeWeapon2[8], "Slot 9"));

	vButtons.push_back(Button(500, 600, 780, 680, [](void* c) {((ControlsScreen*)c)->Resume(); }, "Back"));
	keyToChange = nullptr;
}

ControlsScreen::~ControlsScreen()
{
}

void ControlsScreen::Render()
{
	Graphics::Clear(clrBlack);

	TextSize textsize;
	Graphics::TextMetrics("Controls", Graphics::pFont44Relative, textsize);
	Graphics::WriteText("Controls", fScaleH * 640 - textsize.width / 2, fScaleV * 20.0f, Graphics::pFont44Relative);

	int nY = 100;
	for (Control& control : vControls)
	{
		Graphics::WriteText(control.strDescription, fScaleH * 200, fScaleV * nY, Graphics::pFont24Relative);

		std::string strKey1 = KeyText(*control.nKey1);
		if (control.nKey1 == keyToChange)
		{
			strKey1.insert(0, 1, '[');
			strKey1 += ']';
		}
		std::string strKey2 = KeyText(*control.nKey2);
		if (control.nKey2 == keyToChange)
		{
			strKey2.insert(0, 1, '[');
			strKey2 += ']';
		}

		Graphics::DrawRectangle(fScaleH * 450, fScaleV * nY, fScaleH * 100, fScaleV * 28, clrDarkGrey);
		Graphics::DrawRectangle(fScaleH * 560, fScaleV * nY, fScaleH * 100, fScaleV * 28, clrDarkGrey);

		Graphics::TextMetrics(strKey1.c_str(), Graphics::pFont20Relative, textsize);
		Graphics::WriteText(strKey1.c_str(), fScaleH * 500.0f - (textsize.width / 2), fScaleV * (nY + 14) - (textsize.height / 2), Graphics::pFont20Relative);
		Graphics::TextMetrics(strKey2.c_str(), Graphics::pFont20Relative, textsize);
		Graphics::WriteText(strKey2.c_str(), fScaleH * 610.0f - (textsize.width / 2), fScaleV * (nY + 14) - (textsize.height / 2), Graphics::pFont20Relative);
		nY += 35;
	}

	int i = 0;
	for (Button& button : vButtons)
	{
		button.Draw(nButtonHover == i);
		i++;
	}
}

void ControlsScreen::Update(float deltatime)
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

void ControlsScreen::Resume()
{
	LoadLevel(lPreviousLevel);
}

void ControlsScreen::LeftClick()
{
	int nCursorX, nCursorY;
	GetRelativeMousePos(&nCursorX, &nCursorY);

	if (nButtonHover != -1)
		vButtons[nButtonHover].function(this);

	int nY = 100;
	for (Control& control : vControls)
	{
		Rect rect1 = { 450, nY, 550, nY + 28 };
		Rect rect2 = { 560, nY, 660, nY + 28 };
		if (PointInRect(rect1, nCursorX, nCursorY))
		{
			keyToChange = control.nKey1;
			break;
		}
		if (PointInRect(rect2, nCursorX, nCursorY))
		{
			keyToChange = control.nKey2;
			break;
		}
		nY += 35;
	}
}

void ControlsScreen::KeyDown(int key)
{
	if (key == SDL_SCANCODE_ESCAPE)
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

const char* ControlsScreen::KeyText(int nKey)
{
	return SDL_GetKeyName(SDL_GetKeyFromScancode((SDL_Scancode)nKey));
}
