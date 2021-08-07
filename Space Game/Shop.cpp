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

using namespace Game;

ShopScreen::ShopScreen()
{
	nButtonHover = -1;
	vButtons.push_back(Button(520, 600, 760, 680, [](void* s) { ((ShopScreen*)s)->Resume(); }, "Back", &Graphics::pFont24Relative));

	vButtons.push_back(Button(130, 150, 490, 190, [](void* s) { ((ShopScreen*)s)->UpgradeRegeneration(); }, "\nHealth Regeneration ($100)", &Graphics::pFont12)); //TODO should be relative
	vBars.push_back(Bar(210, 173, 410, 185, clrDodgerBlue, &Game::sgSpaceGame->plPlayer->fHealthRegeneration, &Game::sgSpaceGame->plPlayer->fMaxHealthRegeneration));
	vButtons.push_back(Button(130, 200, 490, 240, [](void* s) { ((ShopScreen*)s)->UpgradeHealth(); }, "\nMax Health ($150)", &Graphics::pFont12));
	vBars.push_back(Bar(210, 223, 410, 235, clrRed, &Game::sgSpaceGame->plPlayer->fMaxHealth, &Game::sgSpaceGame->plPlayer->fMaxHealthUpgrade));
	vButtons.push_back(Button(130, 250, 490, 290, [](void* s) { ((ShopScreen*)s)->UpgradeEnergyRecharge(); }, "\nRecharge Speed ($150)", &Graphics::pFont12));
	vBars.push_back(Bar(210, 273, 410, 285, clrGreen, &Game::sgSpaceGame->plPlayer->fEnergyRechargeSpeed, &Game::sgSpaceGame->plPlayer->fMaxEnergyRechargeSpeed));
	vButtons.push_back(Button(130, 300, 490, 340, [](void* s) { ((ShopScreen*)s)->UpgradeMovementSpeed(); }, "\nMovement Speed ($50)", &Graphics::pFont12));
	vBars.push_back(Bar(210, 323, 410, 335, clrDarkBlue, &Game::sgSpaceGame->plPlayer->fMovementSpeed, &Game::sgSpaceGame->plPlayer->fMaxMovementSpeed));

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
	textures[TextureID::Character]->DrawDifferentSize(1, 900.0f + 8, 140.0f, 1.43f * 200.0f, 200.0f);

	TextSize textsize;
	Graphics::WriteText("Upgrades", fScaleH * 130, fScaleV * 115, Graphics::pFont20Relative);

	for (int i = 0; i < vButtons.size(); i++)
		vButtons[i].Draw(i == nButtonHover);

	for (Bar& bar : vBars)
		bar.Draw();

	char txtBuf[64];
	snprintf(txtBuf, sizeof(txtBuf), "%d / %d", (int)Game::sgSpaceGame->plPlayer->fHealthRegeneration, (int)Game::sgSpaceGame->plPlayer->fMaxHealthRegeneration); //Max health
	Graphics::TextMetrics(txtBuf, Graphics::pFont9Relative, textsize);
	Graphics::WriteText(txtBuf, fScaleH * 310 - textsize.width / 2, fScaleV * 179.0f - textsize.height / 2, Graphics::pFont9Relative);

	snprintf(txtBuf, sizeof(txtBuf), "%d / %d", (int)Game::sgSpaceGame->plPlayer->fMaxHealth, (int)Game::sgSpaceGame->plPlayer->fMaxHealthUpgrade); //Max health
	Graphics::TextMetrics(txtBuf, Graphics::pFont9Relative, textsize);
	Graphics::WriteText(txtBuf, fScaleH * 310 - textsize.width / 2, fScaleV * 229.0f - textsize.height / 2, Graphics::pFont9Relative);

	snprintf(txtBuf, sizeof(txtBuf), "%d / %d", (int)Game::sgSpaceGame->plPlayer->fEnergyRechargeSpeed, (int)Game::sgSpaceGame->plPlayer->fMaxEnergyRechargeSpeed); //Energy recharge
	Graphics::TextMetrics(txtBuf, Graphics::pFont9Relative, textsize);
	Graphics::WriteText(txtBuf, fScaleH * 310 - textsize.width / 2, fScaleV * 279.0f - textsize.height / 2, Graphics::pFont9Relative);

	snprintf(txtBuf, sizeof(txtBuf), "%d / %d", (int)Game::sgSpaceGame->plPlayer->fMovementSpeed, (int)Game::sgSpaceGame->plPlayer->fMaxMovementSpeed); //Energy recharge
	Graphics::TextMetrics(txtBuf, Graphics::pFont9Relative, textsize);
	Graphics::WriteText(txtBuf, fScaleH * 310 - textsize.width / 2, fScaleV * 329.0f - textsize.height / 2, Graphics::pFont9Relative);

	snprintf(txtBuf, sizeof(txtBuf), "$%d", (int)Game::sgSpaceGame->plPlayer->fMoney); //Money (corner)
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
	vButtons[1].bClickable = (Game::sgSpaceGame->plPlayer->fMoney >= 100.0f);
	vButtons[2].bClickable = (Game::sgSpaceGame->plPlayer->fMoney >= 150.0f);
	vButtons[3].bClickable = (Game::sgSpaceGame->plPlayer->fMoney >= 150.0f);
	vButtons[4].bClickable = (Game::sgSpaceGame->plPlayer->fMoney >= 50.0f);
	vButtons[5].bClickable = (Game::sgSpaceGame->plPlayer->fMoney >= 200.0f);
	vButtons[6].bClickable = (Game::sgSpaceGame->plPlayer->fMoney >= 250.0f);
	vButtons[7].bClickable = (Game::sgSpaceGame->plPlayer->fMoney >= 400.0f && (((LaserWeapon*)Game::sgSpaceGame->vItems[0].get())->nLaserLevel != LaserWeapon::floatShot));
	vButtons[8].bClickable = (Game::sgSpaceGame->plPlayer->fMoney >= 500.0f);
}

void ShopScreen::LeftClick()
{
	if (nButtonHover != -1)
		vButtons[nButtonHover].function(this);
}

void ShopScreen::Resume()
{
	LoadLevel(Game::sgSpaceGame);
}

void ShopScreen::UpgradeHealth()
{
	if (Game::sgSpaceGame->plPlayer->fMoney >= 150.0f && Game::sgSpaceGame->plPlayer->fMaxHealth < Game::sgSpaceGame->plPlayer->fMaxHealthUpgrade)
	{
		Game::sgSpaceGame->plPlayer->fMoney -= 150.0f;
		Game::sgSpaceGame->plPlayer->fMaxHealth += 50.0f;
	}
}

void ShopScreen::UpgradeEnergyRecharge()
{
	if (Game::sgSpaceGame->plPlayer->fMoney >= 150.0f && Game::sgSpaceGame->plPlayer->fEnergyRechargeSpeed < Game::sgSpaceGame->plPlayer->fMaxEnergyRechargeSpeed)
	{
		Game::sgSpaceGame->plPlayer->fMoney -= 150.0f;
		Game::sgSpaceGame->plPlayer->fEnergyRechargeSpeed += 1.0f;
	}
}

void ShopScreen::UpgradeMovementSpeed()
{
	if (Game::sgSpaceGame->plPlayer->fMoney >= 50.0f && Game::sgSpaceGame->plPlayer->fMovementSpeed < Game::sgSpaceGame->plPlayer->fMaxMovementSpeed)
	{
		Game::sgSpaceGame->plPlayer->fMoney -= 50.0f;
		Game::sgSpaceGame->plPlayer->fMovementSpeed += 10.0f;
	}
}

void ShopScreen::BuyEnergyPowerup()
{
	if (Game::sgSpaceGame->plPlayer->fMoney >= 200.0f)
	{
		Game::sgSpaceGame->plPlayer->fMoney -= 200.0f;
		Game::sgSpaceGame->vItems.push_back(std::make_shared<EnergyPowerupItem>());
	}
}

void ShopScreen::BuyRegenerationPowerup()
{
	if (Game::sgSpaceGame->plPlayer->fMoney >= 250.0f)
	{
		Game::sgSpaceGame->plPlayer->fMoney -= 250.0f;
		Game::sgSpaceGame->vItems.push_back(std::make_shared<RegenerationPowerupItem>());
	}
}

void ShopScreen::BuyLaserUpgrade()
{
	if (Game::sgSpaceGame->plPlayer->fMoney >= 400.0f && ((LaserWeapon*)Game::sgSpaceGame->vItems[0].get())->nLaserLevel != LaserWeapon::floatShot)
	{
		Game::sgSpaceGame->plPlayer->fMoney -= 400.0f;
		Game::sgSpaceGame->vItems[0] = std::make_shared<LaserWeapon>(LaserWeapon::LaserLevel::floatShot);;
	}
}

void ShopScreen::BuyBombUpgrade()
{
	if (Game::sgSpaceGame->plPlayer->fMoney >= 500.0f)
	{
		Game::sgSpaceGame->plPlayer->fMoney -= 500.0f;
		int nOldLevel = ((BombWeapon*)Game::sgSpaceGame->vItems[2].get())->nLevel;
		Game::sgSpaceGame->vItems[2] = std::make_shared<BombWeapon>(++nOldLevel);
	}
}

void ShopScreen::UpgradeRegeneration()
{
	if (Game::sgSpaceGame->plPlayer->fMoney >= 100.0f && Game::sgSpaceGame->plPlayer->fHealthRegeneration < Game::sgSpaceGame->plPlayer->fMaxHealthRegeneration)
	{
		Game::sgSpaceGame->plPlayer->fMoney -= 100.0f;
		Game::sgSpaceGame->plPlayer->fHealthRegeneration += 1.0f;
	}
}

void ShopScreen::KeyDown(int key)
{
	if (key == SDL_SCANCODE_E || key == SDL_SCANCODE_ESCAPE)
	{
		Resume();
	}
}