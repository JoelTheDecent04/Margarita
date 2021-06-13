#include "Space.h"
#include "Player.h"
#include "Orb.h"
#include "Utilities.h"
#include "Laser.h"
#include "EntityHealthChangeText.h"
#include "Enemy.h"
#include "DeathScreen.h"
#include "Bomb.h"
#include "PauseScreen.h"
#include "Shop.h"
#include "Crab.h"
#include "Comet.h"
#include "EnergyPowerup.h"
#include "RegenerationPowerup.h"
#include "ControlsScreen.h"
#include "Graphics.h"
#include "Light.h"
#include <math.h>
#include <random>
#include <time.h>
#include <fstream>

float fBackgroundPosition = 0.0f;
Texture* tCharacterTexture, * tOrbTexture, * tBackground, * tLaserTexture, * tLaserBeamTexture, * tEnemyTexture, * tBombTexture, * tCrabTexture;
Texture* tForegroundTexture, * tCometTexture, * tNoTexture, * tBombAnimationTexture, * tEnergyPowerupTexture, * tRegenerationPowerupTexture;

Texture* tLight;

int keyOpenShop1 = 'E';
int keyOpenShop2 = 0;
int keyChangeWeapon1[9] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };
int keyChangeWeapon2[9];

int keyNextWave1 = 'R';
int keyNextWave2 = 0;

double fPhysicsUpdatesPerSeconds;

bool bShowHitboxes = false;

D2D1::ColorF clrBlack		= D2D1::ColorF(0.0f, 0.0f, 0.0f);
D2D1::ColorF clrRed			= D2D1::ColorF(1.0f, 0.0f, 0.0f);
D2D1::ColorF clrDarkGrey	= D2D1::ColorF(0.3f, 0.3f, 0.3f);
D2D1::ColorF clrWhite		= D2D1::ColorF(1.0f, 1.0f, 1.0f);
D2D1::ColorF clrBlue		= D2D1::ColorF(0.0f, 0.0f, 1.0f);

int nCurrentVersion = 14;

void SpaceGame::Load()
{
	bGameRunning = true;

	tCharacterTexture = new Texture(L"player.png", 3375, 2098, 83, 58);
	tOrbTexture = new Texture(L"orb.png", 2497, 2497, 32, 32);
	tBackground = new Texture(L"background.png", 15360, 2160, 5120, 720);
	tLaserTexture = new Texture(L"Laser.png");
	tLaserBeamTexture = new Texture(L"LaserBeam.png", 2569, 765, 32, 10);
	tEnemyTexture = new Texture(L"enemy.png", 1856, 2646, 38, 55);
	tCrabTexture = new Texture(L"crab.png", 700, 350, 96, 48);
	tBombTexture = new Texture(L"bomb.png", 2218, 2223, 32.0f, 32.0f);
	tForegroundTexture = new Texture(L"foreground.png", 15360, 382, 5120, 127.33f);
	tCometTexture = new Texture(L"comet.png", 640, 360, 100, 50);
	tNoTexture = new Texture(L"notexture.png");
	tBombAnimationTexture = new Texture(L"bomb_animation.png", 1280, 720, 100.0f, 100.0f);
	tEnergyPowerupTexture = new Texture(L"energy_powerup.png", 2600, 2600, 32.0f, 32.0f);
	tRegenerationPowerupTexture = new Texture(L"regen_powerup.png", 2415, 2415, 32.0f, 32.0f);
	tLight = new Texture(L"inverted_light.png", 512, 512, 384, 384);
	tLight->light = true;


	plPlayer = std::make_shared<Player>(this, 384.0f, 380.f);
	vEntities.push_back(plPlayer);
	vEntities.push_back(std::make_shared<Light>(plPlayer));
	

	vItems.push_back(std::make_shared<LaserWeapon>(LaserWeapon::Normal));
	vItems.push_back(std::make_shared<OrbWeapon>());

	nCurrentItem = 0;

	fNextEnemySpawn = 0.0;

	fDifficulty = 80.0f;
	nWave = 0;
	fSecondsUntilNextWave = 0.0f;
	nEnemies = 0;
	fSecondsUntilNextComet = 40 + random() * 40;
	NextWave();

	LoadFromFile(); //Load save game if it exists

	Game::sgSpaceGame = this;

	fLightingLoopTime = 0.0f;
	fBrightness = 1.0f;
}
void SpaceGame::Unload()
{
	delete tCharacterTexture;
	delete tOrbTexture;
	delete tBackground;
	delete tLaserTexture;
	delete tLaserBeamTexture;
	delete tEnemyTexture;
	delete tBombTexture;
	delete tCrabTexture;
	delete tForegroundTexture;
	delete tCometTexture;
	delete tNoTexture;
	delete tBombAnimationTexture;

	//TODO finish

	Game::sgSpaceGame = nullptr;
}
void SpaceGame::Render()
{
	fBackgroundPosition = plPlayer->fX - (1280 / 2);
	if (plPlayer->fX < (1280 / 2))
		fBackgroundPosition = 0;
	if (plPlayer->fX > 5120 - (1280 / 2))
		fBackgroundPosition = 5120 - 1280;

	tBackground->DrawPanorama(fBackgroundPosition);

	Graphics::iLightingDeviceContext->BeginDraw();

	D2D1_COLOR_F col = { 1.0f, 1.0f, 1.0f, fBrightness };
	Graphics::iLightingDeviceContext->Clear(col);

	for (auto& entity : vEntities)
		entity->Draw();

	tForegroundTexture->Draw(0, -fBackgroundPosition - 65.0f, 475.0f);
	
	Graphics::DrawLighting();

	for (auto& backgroundobject : vBackgroundObjects)
		backgroundobject->Draw();

	wchar_t txtBuf[64];

	swprintf_s(txtBuf, 64, L"HP %u / %u", (int)plPlayer->fHealth, (int)plPlayer->fMaxHealth);
	Graphics::WriteText(txtBuf, 5, 2, 14); //2
	Graphics::FillRectangle(5, 2 + 14 + 4, 100.0f * (plPlayer->fHealth / plPlayer->fMaxHealth), 20, clrRed); //18
	Graphics::DrawRectangle(5, 2 + 14 + 4, 100, 20, clrDarkGrey);

	swprintf_s(txtBuf, 64, L"Energy %u / %u", (int)plPlayer->nEnergy, (int)plPlayer->nMaxEnergy);
	Graphics::WriteText(txtBuf, 5, 2 + 14 + 2 + 20 + 2, 14); //40
	Graphics::FillRectangle(5, 2 + 14 + 2 + 20 + 2 + 14 + 5, 100.0f * (plPlayer->nEnergy / plPlayer->nMaxEnergy), 20, clrBlue); //56
	Graphics::DrawRectangle(5, 2 + 14 + 2 + 20 + 2 + 14 + 5, 100, 20, clrDarkGrey);

	DWRITE_TEXT_METRICS tmTextMetrics;
	Graphics::TextMetrics(vItems[nCurrentItem]->strName, 14.0f, tmTextMetrics);
	Graphics::FillRectangle(0, nScreenHeight - 4 - 32 - 4 - 14, max(32 * vItems.size(), tmTextMetrics.width) + 10, 14 + 4 + 32 + 4, clrBlack);
	Graphics::WriteText(vItems[nCurrentItem]->strName, 5, nScreenHeight - 4 - 32 - 4 - 14, 14);

	int nItem = 0;
	for (auto& item : vItems)
	{
		item->tTexture->Draw(0, 4 + nItem * 32, nScreenHeight - 4 - 32, true);
		Graphics::DrawRectangle(4 + nItem * 32, nScreenHeight - 4 - 32, 32, 32, nItem == nCurrentItem ? clrWhite : clrDarkGrey);
		if (item->nCount > 1)
		{
			swprintf_s(txtBuf, 64, L"%d", item->nCount);
			Graphics::WriteText(txtBuf, 6 + nItem * 32, nScreenHeight - 4 - 16, 14.0f);
		}
		nItem++;
	}

	swprintf_s(txtBuf, 64, L"$%d", (int)plPlayer->fMoney);
	Graphics::TextMetrics(txtBuf, 16.0f, tmTextMetrics);
	Graphics::WriteText(txtBuf, nScreenWidth - 5 - tmTextMetrics.width, 5, 16.0f);

	swprintf_s(txtBuf, 64, L"Wave %d (%d seconds left)", nWave, (int)fSecondsUntilNextWave);
	Graphics::TextMetrics(txtBuf, 16.0f, tmTextMetrics);
	Graphics::FillRectangle(nScreenWidth - 5 - tmTextMetrics.width - 5, nScreenHeight - 5 - tmTextMetrics.height - 5, 5 + tmTextMetrics.width + 3, 5 + 16 + 5, clrBlack);
	Graphics::WriteText(txtBuf, nScreenWidth - 5 - tmTextMetrics.width, nScreenHeight - 5 - tmTextMetrics.height, 16.0f);

	if (bWaveFinished)
	{
		swprintf_s(txtBuf, 64, L"Wave Completed. Press '%s' To Continue.", ControlsScreen::KeyText(keyNextWave1).c_str());
		Graphics::TextMetrics(txtBuf, 16.0f, tmTextMetrics);
		Graphics::WriteText(txtBuf, nScreenWidth / 2 - tmTextMetrics.width / 2, 4, 16.0f);
	}

	
}
void SpaceGame::Update(double deltatime)
{
	fPhysicsUpdatesPerSeconds = 1.0 / deltatime;

	for (int i = 0; i < vEntities.size(); i++) //Entity updates //TODO optimise
	{
		auto entity = vEntities[i];
		bool bEntityExists = entity->Update(deltatime);
		if (!bEntityExists)
		{
			vEntities.erase(vEntities.begin() + i);
			i--;
		}
		if (!bGameRunning) //Game could end after any entity update
		{
			Game::LoadLevel(new DeathScreen());
			return;
		}
	}

	for (int i = 0; i < vBackgroundObjects.size(); i++)
	{
		auto backgroundobjects = vBackgroundObjects[i];
		if (backgroundobjects->Update(deltatime) == false)
		{
			vBackgroundObjects.erase(vBackgroundObjects.begin() + i);
			i--;
		}
	}

	fNextEnemySpawn -= deltatime; //Enemy spawning
	if (fNextEnemySpawn <= 0 && fSecondsUntilNextWave > 0.0f)
	{
		if (random() >= 0.15f)
		{
			auto enemy = std::make_shared<Enemy>(this, random_off_screen(), random() * 500);
			if (enemy->bLegalPosition)
			{
				vEntities.push_back(enemy);
				nEnemies++;
			}
		}
		else
		{
			auto crab = std::make_shared<Crab>(this, random_off_screen());
			if (crab->bLegalPosition)
			{
				vEntities.push_back(crab);
				nEnemies++;
			}
		}

		fNextEnemySpawn = (random() * 10.0f) / (fDifficulty / 60.0f);
	}

	fSecondsUntilNextWave -= deltatime; //Waves
	if (fSecondsUntilNextWave < 0.0f) fSecondsUntilNextWave = 0.0f;
	if (fSecondsUntilNextWave <= 0.0f && nEnemies == 0)
		bWaveFinished = true;

	fSecondsUntilNextComet -= deltatime; //Comet
	if (fSecondsUntilNextComet < 0.0f)
	{
		vBackgroundObjects.push_back(std::make_shared<Comet>());
		fSecondsUntilNextComet = 40 + random() * 40;
	}

	fLightingLoopTime += deltatime; //Lighting
	if (fLightingLoopTime > 90.0f)
		fLightingLoopTime = fLightingLoopTime - 90.0f;

	if (fLightingLoopTime >= 40.0f && fLightingLoopTime < 50.0f)
		fBrightness = cosf((fLightingLoopTime - 40.0f) / 10 * 3.141592f) / 2.0f + 0.5f;
	else if (fLightingLoopTime >= 50.0f && fLightingLoopTime < 80.0f)
		fBrightness = 0.0f;
	else if (fLightingLoopTime >= 80.0f && fLightingLoopTime < 90.0f)
		fBrightness = cosf((90.0f - fLightingLoopTime) / 10 * 3.141592f) / 2.0f + 0.5f;
	else
		fBrightness = 1.0f;
}
void SpaceGame::LeftClick()
{
	POINT pntCursorPosition;
	GetCursorPos(&pntCursorPosition);
	ScreenToClient(Graphics::hWindow, &pntCursorPosition); //Adjust cursor position so it is relative to the window

	pntCursorPosition.x /= fScaleH;
	pntCursorPosition.y /= fScaleV;

	pntCursorPosition.x += fBackgroundPosition;	

	float fGradient = (pntCursorPosition.y - plPlayer->fY) / (pntCursorPosition.x - plPlayer->fX);
	float fAngle = atan(fGradient);
	if (pntCursorPosition.x < plPlayer->fX) fAngle += 3.1415926f;
	
	vItems[nCurrentItem]->Use(this, plPlayer->fX, plPlayer->fY, fAngle);
	if (vItems[nCurrentItem]->nCount == 0)
	{
		vItems.erase(vItems.begin() + nCurrentItem);
		if (nCurrentItem >= vItems.size())
			nCurrentItem = vItems.size() - 1;
	}

}

void SpaceGame::KeyDown(int key)
{
	if (key == VK_ESCAPE)
		Game::LoadLevel(new PauseScreen(this), false, true);
	if (key == keyOpenShop1 || key == keyOpenShop2)
		Game::LoadLevel(new ShopScreen(this), false, true);
	if (key == 'K')
		plPlayer->ChangeHealth(-20.0f);
	if (key == 'J')
		plPlayer->ChangeHealth(20.0f);
	if (key == 'M')
		plPlayer->fMoney += 200.0f;
	if (key == 'H')
		bShowHitboxes = !bShowHitboxes;
	if (key == keyNextWave1 || key == keyNextWave2)
		if (bWaveFinished) NextWave();
	if (key == 'O')
		Save();
	if (key == 'L')
		LoadFromFile();
	for (int i = 0; i < 9; i++)
	{
		if (key == keyChangeWeapon1[i] || key == keyChangeWeapon2[i])
			if (vItems.size() >= i + 1)
				nCurrentItem = i;
	}

	if (key == VK_OEM_6) //[
	{
		nCurrentItem++;
		if (nCurrentItem >= vItems.size()) nCurrentItem = 0;
	}
	if (key == VK_OEM_4) //]
	{
		nCurrentItem--;
		if (nCurrentItem < 0) nCurrentItem = vItems.size() - 1;
	}
}

void SpaceGame::Save()
{
	std::fstream f;
	f.open("savegame.txt", std::fstream::out);

	f << nCurrentVersion << " " << fDifficulty << " " << nWave << " " << fSecondsUntilNextWave << " " << nEnemies << " " << fLightingLoopTime << " ";

	f << vEntities.size() << " ";
	for (auto& entity : vEntities)
		entity->Save(f);

	f << vItems.size() << " ";
	for (auto& item : vItems)
		item->Save(f);

	f.close();
}

void SpaceGame::LoadFromFile()
{
	std::fstream f;
	f.open("savegame.txt", std::fstream::in);

	if (!f.good()) return;

	int nVersion;
	f >> nVersion >> fDifficulty >> nWave >> fSecondsUntilNextWave >> nEnemies >> fLightingLoopTime;
	if (nVersion != nCurrentVersion) return;


	vEntities.clear(); //TODO free
	int nEntities;
	f >> nEntities;

	for (int i = 0; i < nEntities; i++)
	{
		int t;
		f >> t;
		Entity::Type type = (Entity::Type)t;

		switch (type)
		{
		case Entity::Type::None:
			abort();
			break;
		case Entity::Type::Player:
		{
			auto e = std::make_shared<Player>(this, 0.0f, 0.0f);
			e->Load(f);
			vEntities.push_back(e);
			vEntities.push_back(std::make_shared<Light>(e));
			plPlayer = e;
			break;
		}
		case Entity::Type::Bomb:
		{
			auto e = std::make_shared<Bomb>(this, 0.0f, 0.0f, 0.0f, 0.0f, 0);
			e->Load(f);
			vEntities.push_back(e);
			break;
		}
		case Entity::Type::Crab:
		{
			auto e = std::make_shared<Crab>(this, 0.0f);
			e->Load(f);
			vEntities.push_back(e);
			break;
		}
		case Entity::Type::Enemy:
		{
			auto e = std::make_shared<Enemy>(this, 0.0f, 0.0f);
			e->Load(f);
			vEntities.push_back(e);
			break;
		}
		case Entity::Type::Laser:
		{
			auto e = std::make_shared<LaserBeam>(this, nullptr, 0.0f, 0.0f, 0.0f, 0.0f);
			e->Load(f);
			vEntities.push_back(e);
			break;
		}
		case Entity::Type::Orb:
		{
			auto e = std::make_shared<Orb>(this, 0.0f, 0.0f, 0.0f, 0.0f);
			e->Load(f);
			vEntities.push_back(e);
			break;
		}
		case Entity::Type::Light:
			break;
		default:
			abort();
		}
	}
	
	vItems.clear();
	int nItems;
	f >> nItems;
	for (int i = 0; i < nItems; i++)
	{
		int t;
		f >> t;
		Item::Type type = (Item::Type)t;

		switch (type)
		{
		case Item::Type::None:
			abort();
			break;
		case Item::Type::Bomb:
		{
			auto i = std::make_shared<BombWeapon>(0);
			i->Load(f);
			vItems.push_back(i);
			break;
		}
		case Item::Type::Laser:
		{
			auto i = std::make_shared<LaserWeapon>(LaserWeapon::LaserLevel::Normal);
			i->Load(f);
			vItems.push_back(i);
			break;
		}
		case Item::Type::Orb:
		{
			auto i = std::make_shared<OrbWeapon>();
			i->Load(f);
			vItems.push_back(i);
			break;
		}
		case Item::Type::EnergyPowerup:
		{
			auto i = std::make_shared<EnergyPowerupItem>();
			i->Load(f);
			vItems.push_back(i);
			break;
		}
		case Item::Type::RegenerationPowerup:
		{
			auto i = std::make_shared<RegenerationPowerupItem>();
			i->Load(f);
			vItems.push_back(i);
			break;
		}

		default:
			abort();
		}
	}

	f.close();
}

void SpaceGame::NextWave()
{
	nWave++;
	fSecondsUntilNextWave = 60.0f;
	fDifficulty *= 1.5f;

	plPlayer->fMaxHealthUpgrade += 50.0f;
	plPlayer->fMaxEnergyRechargeSpeed += 1.0f;
	plPlayer->fMaxMovementSpeed += 10.0f;
	plPlayer->fMaxHealthRegeneration += 1.0f;
	bWaveFinished = false;
}