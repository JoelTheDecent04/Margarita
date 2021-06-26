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

ShopScreen::ShopScreen()
{
	nButtonHover = -1;
	vButtons.push_back(Button(520, 600, 760, 680, [](void* s) { ((ShopScreen*)s)->Resume(); }, "Back", &Graphics::pFont24Relative));

	vButtons.push_back(Button(130, 150, 490, 190, [](void* s) { ((ShopScreen*)s)->UpgradeRegeneration(); }, "Health Regeneration ($100)\n", &Graphics::pFont12)); //TODO should be relative
	vBars.push_back(Bar(210, 173, 410, 185, clrDodgerBlue, &lGameLevel->plPlayer->fHealthRegeneration, &lGameLevel->plPlayer->fMaxHealthRegeneration));
	vButtons.push_back(Button(130, 200, 490, 240, [](void* s) { ((ShopScreen*)s)->UpgradeHealth(); }, "Max Health ($150)\n", &Graphics::pFont12));
	vBars.push_back(Bar(210, 223, 410, 235, clrRed, &lGameLevel->plPlayer->fMaxHealth, &lGameLevel->plPlayer->fMaxHealthUpgrade));
	vButtons.push_back(Button(130, 250, 490, 290, [](void* s) { ((ShopScreen*)s)->UpgradeEnergyRecharge(); }, "Recharge Speed ($150)\n", &Graphics::pFont12));
	vBars.push_back(Bar(210, 273, 410, 285, clrGreen, &lGameLevel->plPlayer->fEnergyRechargeSpeed, &lGameLevel->plPlayer->fMaxEnergyRechargeSpeed));
	vButtons.push_back(Button(130, 300, 490, 340, [](void* s) { ((ShopScreen*)s)->UpgradeMovementSpeed(); }, "Movement Speed ($50)\n", &Graphics::pFont12));
	vBars.push_back(Bar(210, 323, 410, 335, clrDarkBlue, &lGameLevel->plPlayer->fMovementSpeed, &lGameLevel->plPlayer->fMaxMovementSpeed));

	vButtons.push_back(Button(610, 150, 800, 190, [](void* s) { ((ShopScreen*)s)->BuyEnergyPowerup(); }, "Energy Powerup ($200)", &Graphics::pFont16Relative));
	vButtons.push_back(Button(610, 200, 800, 240, [](void* s) { ((ShopScreen*)s)->BuyRegenerationPowerup(); }, "Regeneration Powerup ($250)", &Graphics::pFont14Relative));

	vButtons.push_back(Button(130, 430, 320, 470, [](void* s) { ((ShopScreen*)s)->BuyLaserUpgrade(); }, "Laser ($400)", &Graphics::pFont16Relative));
	vButtons.push_back(Button(130, 480, 320, 520, [](void* s) { ((ShopScreen*)s)->BuyBombUpgrade(); }, "Bomb ($500)", &Graphics::pFont16Relative));

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
	Graphics::WriteText("Upgrades", fScaleH * 130, fScaleV * 115, Graphics::pFont20Relative);

	for (int i = 0; i < vButtons.size(); i++)
		vButtons[i].Draw(i == nButtonHover);

	for (Bar& bar : vBars)
		bar.Draw();

	char txtBuf[64];
	snprintf(txtBuf, sizeof(txtBuf), "%d / %d", (int)lGameLevel->plPlayer->fHealthRegeneration, (int)lGameLevel->plPlayer->fMaxHealthRegeneration); //Max health
	Graphics::TextMetrics(txtBuf, Graphics::pFont9Relative, textsize);
	Graphics::WriteText(txtBuf, fScaleH * 310 - textsize.width / 2, fScaleV * 179.0f - textsize.height / 2, Graphics::pFont9Relative);

	snprintf(txtBuf, sizeof(txtBuf), "%d / %d", (int)lGameLevel->plPlayer->fMaxHealth, (int)lGameLevel->plPlayer->fMaxHealthUpgrade); //Max health
	Graphics::TextMetrics(txtBuf, Graphics::pFont9Relative, textsize);
	Graphics::WriteText(txtBuf, fScaleH * 310 - textsize.width / 2, fScaleV * 229.0f - textsize.height / 2, Graphics::pFont9Relative);

	snprintf(txtBuf, sizeof(txtBuf), "%d / %d", (int)lGameLevel->plPlayer->fEnergyRechargeSpeed, (int)lGameLevel->plPlayer->fMaxEnergyRechargeSpeed); //Energy recharge
	Graphics::TextMetrics(txtBuf, Graphics::pFont9Relative, textsize);
	Graphics::WriteText(txtBuf, fScaleH * 310 - textsize.width / 2, fScaleV * 279.0f - textsize.height / 2, Graphics::pFont9Relative);

	snprintf(txtBuf, sizeof(txtBuf), "%d / %d", (int)lGameLevel->plPlayer->fMovementSpeed, (int)lGameLevel->plPlayer->fMaxMovementSpeed); //Energy recharge
	Graphics::TextMetrics(txtBuf, Graphics::pFont9Relative, textsize);
	Graphics::WriteText(txtBuf, fScaleH * 310 - textsize.width / 2, fScaleV * 329.0f - textsize.height / 2, Graphics::pFont9Relative);

	snprintf(txtBuf, sizeof(txtBuf), "$%d", (int)lGameLevel->plPlayer->fMoney); //Money (corner)
	Graphics::TextMetrics(txtBuf, Graphics::pFont16, textsize);
	Graphics::WriteText(txtBuf, fScaleH * (1280 - 5) - textsize.width, 5, Graphics::pFont16);

	Graphics::WriteText("Powerups", fScaleH * 610, fScaleV * 115, Graphics::pFont20Relative);

	Graphics::WriteText("Weapon Upgrades", fScaleH * 130.0f, fScaleV * 395.0f, Graphics::pFont20Relative);
}

void ShopScreen::Update(float deltatime)
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
	vButtons[7].bClickable = (lGameLevel->plPlayer->fMoney >= 400.0f && (((LaserWeapon*)lGameLevel->vItems[0].get())->nLaserLevel != LaserWeapon::floatShot));
	vButtons[8].bClickable = (lGameLevel->plPlayer->fMoney >= 500.0f);
}

void ShopScreen::LeftClick()
{
	if (nButtonHover != -1)
		vButtons[nButtonHover].function(this);
}

void ShopScreen::Resume()
{
	Game::LoadLevel(Game::sgSpaceGame);
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
	if (lGameLevel->plPlayer->fMoney >= 400.0f && ((LaserWeapon*)lGameLevel->vItems[0].get())->nLaserLevel != LaserWeapon::floatShot)
	{
		lGameLevel->plPlayer->fMoney -= 400.0f;
		lGameLevel->vItems[0] = std::make_shared<LaserWeapon>(LaserWeapon::LaserLevel::floatShot);;
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
	if (key == SDL_SCANCODE_E || key == SDL_SCANCODE_ESCAPE)
	{
		Resume();
	}
}