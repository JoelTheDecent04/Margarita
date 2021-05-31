#include "Shop.h"
#include "Game.h"
#include "Space.h"
#include "Player.h"
#include "EnergyPowerup.h"

static D2D1::ColorF clrDarkGrey = D2D1::ColorF::DarkGray;
static D2D1::ColorF clrWhite = D2D1::ColorF::White;
static D2D1::ColorF clrBlack = D2D1::ColorF(0.0f, 0.0f, 0.0f);
static D2D1::ColorF clrRed = D2D1::ColorF::Red;
static D2D1::ColorF clrGreen = D2D1::ColorF::Green;
static D2D1::ColorF clrYellow = D2D1::ColorF::Yellow;

ShopScreen::ShopScreen(SpaceGame* lGameLevel)
{
	this->lGameLevel = lGameLevel;
	nButtonHover = -1;
	vButtons.push_back(Button(500, 500, 780, 600, [](void* s) { ((ShopScreen*)s)->Resume(); }, L"Back", 24.0f));

	vButtons.push_back(Button(130, 200, 310, 240, [](void* s) { ((ShopScreen*)s)->UpgradeHealth(); }, L"Max Health ($150)", 16.0f));
	vBars.push_back(Bar(330, 200, 530, 240, clrRed, &lGameLevel->plPlayer->fMaxHealth, &lGameLevel->plPlayer->fMaxHealthUpgrade));
	vButtons.push_back(Button(130, 250, 310, 290, [](void* s) { ((ShopScreen*)s)->UpgradeEnergyRecharge(); }, L"Recharge Speed ($150)", 16.0f));
	vBars.push_back(Bar(330, 250, 530, 290, clrGreen, &lGameLevel->plPlayer->fEnergyRechargeSpeed, &lGameLevel->plPlayer->fMaxEnergyRechargeSpeed));
	vButtons.push_back(Button(130, 300, 310, 340, [](void* s) { ((ShopScreen*)s)->UpgradeMovementSpeed(); }, L"Movement Speed ($50)", 16.0f));
	vBars.push_back(Bar(330, 300, 530, 340, clrYellow, &lGameLevel->plPlayer->fMovementSpeed, &lGameLevel->plPlayer->fMaxMovementSpeed));

	vButtons.push_back(Button(700, 200, 880, 240, [](void* s) { ((ShopScreen*)s)->BuyEnergyPowerup(); }, L"Energy Powerup ($200)", 16.0f));

	nUpgrades = 0;
}

void ShopScreen::Render()
{
	Graphics::Clear(clrBlack);
	DWRITE_TEXT_METRICS tmTextMetrics;
	Graphics::TextMetrics(L"Shop", fScaleV * 36.0f, tmTextMetrics);
	Graphics::WriteText(L"Shop", fScaleH * 640 - tmTextMetrics.width / 2, fScaleV * 60, fScaleV * 36.0f);

	Graphics::WriteText(L"Upgrades:", fScaleH * 130, fScaleV * 150, fScaleV * 20.0f);

	for (int i = 0; i < vButtons.size(); i++)
		vButtons[i].Draw(i == nButtonHover);

	for (Bar& bar : vBars)
		bar.Draw();

	wchar_t txtBuf[64];
	swprintf_s(txtBuf, 64, L"%d / %d", (int)lGameLevel->plPlayer->fMaxHealth, (int)lGameLevel->plPlayer->fMaxHealthUpgrade); //Max health
	Graphics::TextMetrics(txtBuf, fScaleV * 16.0f, tmTextMetrics);
	Graphics::WriteText(txtBuf, fScaleH * 550, fScaleV * (220 - 8), fScaleV * 16.0f);

	swprintf_s(txtBuf, 64, L"%d / %d", (int)lGameLevel->plPlayer->fEnergyRechargeSpeed, (int)lGameLevel->plPlayer->fMaxEnergyRechargeSpeed); //Energy recharge
	Graphics::TextMetrics(txtBuf, fScaleV * 16.0f, tmTextMetrics);
	Graphics::WriteText(txtBuf, fScaleH * 550, fScaleV * (270 - 8), fScaleV * 16.0f);

	swprintf_s(txtBuf, 64, L"%d / %d", (int)lGameLevel->plPlayer->fMovementSpeed, (int)lGameLevel->plPlayer->fMaxMovementSpeed); //Energy recharge
	Graphics::TextMetrics(txtBuf, fScaleV * 16.0f, tmTextMetrics);
	Graphics::WriteText(txtBuf, fScaleH * 550, fScaleV * (320 - 8), fScaleV * 16.0f);

	swprintf_s(txtBuf, 64, L"$%d", (int)lGameLevel->plPlayer->fMoney); //Money (corner)
	Graphics::TextMetrics(txtBuf, fScaleV * 16.0f, tmTextMetrics);
	Graphics::WriteText(txtBuf, fScaleH * (1280 - 5) - tmTextMetrics.width, 5, fScaleV * 16.0f);

	Graphics::WriteText(L"Powerups:", fScaleH * 700, fScaleV * 150, fScaleV * 20.0f);
}

void ShopScreen::Update(double deltatime)
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
	vButtons[1].bClickable = (lGameLevel->plPlayer->fMoney >= 150.0f);
	vButtons[2].bClickable = (lGameLevel->plPlayer->fMoney >= 150.0f);
	vButtons[3].bClickable = (lGameLevel->plPlayer->fMoney >= 50.0f);
	vButtons[4].bClickable = (lGameLevel->plPlayer->fMoney >= 200.0f);

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
		lGameLevel->plPlayer->fHealth += 50.0f;
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
		for (Item*& item : lGameLevel->vItems)
		{
			if (item == nullptr)
			{
				item = new EnergyPowerupItem();
				return;
			}
		}
		
		lGameLevel->vItems.push_back(new EnergyPowerupItem());
	}
}

void ShopScreen::KeyDown(int key)
{
	if (key == 'E' || key == VK_ESCAPE)
	{
		Resume();
	}
}