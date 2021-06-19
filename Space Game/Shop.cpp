#include "Shop.h"
#include "Game.h"
#include "Space.h"
#include "Player.h"
#include "EnergyPowerup.h"
#include "RegenerationPowerup.h"
#include "Laser.h"
#include "Bomb.h"
#include "Utilities.h"
#include "Colours.h"

ShopScreen::ShopScreen(SpaceGame* lGameLevel)
{
	this->lGameLevel = lGameLevel;
	nButtonHover = -1;
	vButtons.push_back(Button(520, 600, 760, 680, [](void* s) { ((ShopScreen*)s)->Resume(); }, L"Back", 24.0f));

	vButtons.push_back(Button(130, 150, 490, 190, [](void* s) { ((ShopScreen*)s)->UpgradeRegeneration(); }, L"Health Regeneration ($100)\n", 12.0f));
	vBars.push_back(Bar(210, 173, 410, 185, clrDodgerBlue, &lGameLevel->plPlayer->fHealthRegeneration, &lGameLevel->plPlayer->fMaxHealthRegeneration));
	vButtons.push_back(Button(130, 200, 490, 240, [](void* s) { ((ShopScreen*)s)->UpgradeHealth(); }, L"Max Health ($150)\n", 12.0f));
	vBars.push_back(Bar(210, 223, 410, 235, clrRed, &lGameLevel->plPlayer->fMaxHealth, &lGameLevel->plPlayer->fMaxHealthUpgrade));
	vButtons.push_back(Button(130, 250, 490, 290, [](void* s) { ((ShopScreen*)s)->UpgradeEnergyRecharge(); }, L"Recharge Speed ($150)\n", 12.0f));
	vBars.push_back(Bar(210, 273, 410, 285, clrGreen, &lGameLevel->plPlayer->fEnergyRechargeSpeed, &lGameLevel->plPlayer->fMaxEnergyRechargeSpeed));
	vButtons.push_back(Button(130, 300, 490, 340, [](void* s) { ((ShopScreen*)s)->UpgradeMovementSpeed(); }, L"Movement Speed ($50)\n", 12.0f));
	vBars.push_back(Bar(210, 323, 410, 335, clrDarkBlue, &lGameLevel->plPlayer->fMovementSpeed, &lGameLevel->plPlayer->fMaxMovementSpeed));

	vButtons.push_back(Button(610, 150, 800, 190, [](void* s) { ((ShopScreen*)s)->BuyEnergyPowerup(); }, L"Energy Powerup ($200)", 16.0f));
	vButtons.push_back(Button(610, 200, 800, 240, [](void* s) { ((ShopScreen*)s)->BuyRegenerationPowerup(); }, L"Regeneration Powerup ($250)", 13.0f));

	vButtons.push_back(Button(130, 430, 320, 470, [](void* s) { ((ShopScreen*)s)->BuyLaserUpgrade(); }, L"Laser ($400)", 16.0f));
	vButtons.push_back(Button(130, 480, 320, 520, [](void* s) { ((ShopScreen*)s)->BuyBombUpgrade(); }, L"Bomb ($500)", 16.0f));

	nUpgrades = 0;
}

void ShopScreen::Render()
{
	Graphics::Clear(clrBlack);
	Graphics::DrawRectangle(fScaleH * 70.0f, fScaleV * 100.0f, fScaleH * 480.0f, fScaleV * 280.0f, clrCustomDarkBlue, 1.0f, 3.0f);
	Graphics::DrawRectangle(fScaleH * 550.0f, fScaleV * 100.0f, fScaleH * 350.0f, fScaleV * 280.0f, clrCustomDarkBlue, 1.0f, 3.0f);
	Graphics::DrawRectangle(fScaleH * 900.0f, fScaleV * 100.0f, fScaleH * 1.43f * 216.0f, fScaleV * 280.0f, clrCustomDarkBlue, 1.0f, 3.0f);
	Graphics::DrawRectangle(fScaleH * 70.0f, fScaleV * 380.0f, fScaleH * 350.0f, fScaleV * 180.0f, clrCustomDarkBlue, 1.0f, 3.0f);
	tCharacterTexture->DrawDifferentSize(1, 900.0f + 8, 140.0f, 1.43f * 200.0f, 200.0f);

	TextSize textsize;
	Graphics::WriteText(L"Upgrades", fScaleH * 130, fScaleV * 115, fScaleV * 20.0f);

	for (int i = 0; i < vButtons.size(); i++)
		vButtons[i].Draw(i == nButtonHover);

	for (Bar& bar : vBars)
		bar.Draw();

	wchar_t txtBuf[64];
	swprintf_s(txtBuf, 64, L"%d / %d", (int)lGameLevel->plPlayer->fHealthRegeneration, (int)lGameLevel->plPlayer->fMaxHealthRegeneration); //Max health
	Graphics::TextMetrics(txtBuf, fScaleV * 9.0f, textsize);
	Graphics::WriteText(txtBuf, fScaleH * 310 - textsize.width / 2, fScaleV * 179.0f - textsize.height / 2, fScaleV * 9.0f);

	swprintf_s(txtBuf, 64, L"%d / %d", (int)lGameLevel->plPlayer->fMaxHealth, (int)lGameLevel->plPlayer->fMaxHealthUpgrade); //Max health
	Graphics::TextMetrics(txtBuf, fScaleV * 9.0f, textsize);
	Graphics::WriteText(txtBuf, fScaleH * 310 - textsize.width / 2, fScaleV * 229.0f - textsize.height / 2, fScaleV * 9.0f);

	swprintf_s(txtBuf, 64, L"%d / %d", (int)lGameLevel->plPlayer->fEnergyRechargeSpeed, (int)lGameLevel->plPlayer->fMaxEnergyRechargeSpeed); //Energy recharge
	Graphics::TextMetrics(txtBuf, fScaleV * 9.0f, textsize);
	Graphics::WriteText(txtBuf, fScaleH * 310 - textsize.width / 2, fScaleV * 279.0f - textsize.height / 2, fScaleV * 9.0f);

	swprintf_s(txtBuf, 64, L"%d / %d", (int)lGameLevel->plPlayer->fMovementSpeed, (int)lGameLevel->plPlayer->fMaxMovementSpeed); //Energy recharge
	Graphics::TextMetrics(txtBuf, fScaleV * 9.0f, textsize);
	Graphics::WriteText(txtBuf, fScaleH * 310 - textsize.width / 2, fScaleV * 329.0f - textsize.height / 2, fScaleV * 9.0f);

	swprintf_s(txtBuf, 64, L"$%d", (int)lGameLevel->plPlayer->fMoney); //Money (corner)
	Graphics::TextMetrics(txtBuf, fScaleV * 16.0f, textsize);
	Graphics::WriteText(txtBuf, fScaleH * (1280 - 5) - textsize.width, 5, fScaleV * 16.0f);

	Graphics::WriteText(L"Powerups", fScaleH * 610, fScaleV * 115, fScaleV * 20.0f);

	Graphics::WriteText(L"Weapon Upgrades", fScaleH * 130.0f, fScaleV * 395.0f, fScaleV * 20.0f);
}

void ShopScreen::Update(double deltatime)
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
	vButtons[1].bClickable = (lGameLevel->plPlayer->fMoney >= 100.0f);
	vButtons[2].bClickable = (lGameLevel->plPlayer->fMoney >= 150.0f);
	vButtons[3].bClickable = (lGameLevel->plPlayer->fMoney >= 150.0f);
	vButtons[4].bClickable = (lGameLevel->plPlayer->fMoney >= 50.0f);
	vButtons[5].bClickable = (lGameLevel->plPlayer->fMoney >= 200.0f);
	vButtons[6].bClickable = (lGameLevel->plPlayer->fMoney >= 250.0f);
	vButtons[7].bClickable = (lGameLevel->plPlayer->fMoney >= 400.0f && (((LaserWeapon*)lGameLevel->vItems[0].get())->nLaserLevel != LaserWeapon::DoubleShot));
	vButtons[8].bClickable = (lGameLevel->plPlayer->fMoney >= 500.0f);
}

void ShopScreen::LeftClick()
{
	if (nButtonHover != -1)
		vButtons[nButtonHover].function(this);
}

void ShopScreen::Resume()
{
	Game::LoadLevel(lGameLevel, true, false);
}

void ShopScreen::UpgradeHealth()
{
	if (lGameLevel->plPlayer->fMoney >= 150.0f && lGameLevel->plPlayer->fMaxHealth < lGameLevel->plPlayer->fMaxHealthUpgrade)
	{
		lGameLevel->plPlayer->fMoney -= 150.0f;
		lGameLevel->plPlayer->fMaxHealth += 50.0f;
	}
}

void ShopScreen::UpgradeEnergyRecharge()
{
	if (lGameLevel->plPlayer->fMoney >= 150.0f && lGameLevel->plPlayer->fEnergyRechargeSpeed < lGameLevel->plPlayer->fMaxEnergyRechargeSpeed)
	{
		lGameLevel->plPlayer->fMoney -= 150.0f;
		lGameLevel->plPlayer->fEnergyRechargeSpeed += 1.0f;
	}
}

void ShopScreen::UpgradeMovementSpeed()
{
	if (lGameLevel->plPlayer->fMoney >= 50.0f && lGameLevel->plPlayer->fMovementSpeed < lGameLevel->plPlayer->fMaxMovementSpeed)
	{
		lGameLevel->plPlayer->fMoney -= 50.0f;
		lGameLevel->plPlayer->fMovementSpeed += 10.0f;
	}
}

void ShopScreen::BuyEnergyPowerup()
{
	if (lGameLevel->plPlayer->fMoney >= 200.0f)
	{
		lGameLevel->plPlayer->fMoney -= 200.0f;
		lGameLevel->vItems.push_back(std::make_shared<EnergyPowerupItem>());
	}
}

void ShopScreen::BuyRegenerationPowerup()
{
	if (lGameLevel->plPlayer->fMoney >= 250.0f)
	{
		lGameLevel->plPlayer->fMoney -= 250.0f;
		lGameLevel->vItems.push_back(std::make_shared<RegenerationPowerupItem>());
	}
}

void ShopScreen::BuyLaserUpgrade()
{
	if (lGameLevel->plPlayer->fMoney >= 400.0f && ((LaserWeapon*)lGameLevel->vItems[0].get())->nLaserLevel != LaserWeapon::DoubleShot)
	{
		lGameLevel->plPlayer->fMoney -= 400.0f;
		lGameLevel->vItems[0] = std::make_shared<LaserWeapon>(LaserWeapon::LaserLevel::DoubleShot);;
	}
}

void ShopScreen::BuyBombUpgrade()
{
	if (lGameLevel->plPlayer->fMoney >= 500.0f)
	{
		lGameLevel->plPlayer->fMoney -= 500.0f;
		int nOldLevel = ((BombWeapon*)lGameLevel->vItems[2].get())->nLevel;
		lGameLevel->vItems[2] = std::make_shared<BombWeapon>(++nOldLevel);
	}
}

void ShopScreen::UpgradeRegeneration()
{
	if (lGameLevel->plPlayer->fMoney >= 100.0f && lGameLevel->plPlayer->fHealthRegeneration < lGameLevel->plPlayer->fMaxHealthRegeneration)
	{
		lGameLevel->plPlayer->fMoney -= 100.0f;
		lGameLevel->plPlayer->fHealthRegeneration += 1.0f;
	}
}

void ShopScreen::KeyDown(int key)
{
	if (key == 'E' || key == VK_ESCAPE)
	{
		Resume();
	}
}