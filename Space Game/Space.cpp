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
#include <math.h>
#include <random>
#include <time.h>
#include <fstream>

float fBackgroundPosition = 0.0f;
Texture* tCharacterTexture, * tOrbTexture, * tBackground, * tLaserTexture, * tLaserBeamTexture, * tEnemyTexture, * tBombTexture, * tCrabTexture;
Texture* tForegroundTexture, * tCometTexture, * tNoTexture, * tBombAnimationTexture, * tEnergyPowerupTexture, * tRegenerationPowerupTexture;

int keyOpenShop1 = 'E';
int keyOpenShop2 = 0;
int keyChangeWeapon1[9] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };
int keyChangeWeapon2[9];

bool bShowDebugInfo = false;
double fPhysicsUpdatesPerSeconds;

bool bShowHitboxes = false;

D2D1::ColorF clrBlack		= D2D1::ColorF(0.0f, 0.0f, 0.0f);
D2D1::ColorF clrRed			= D2D1::ColorF(1.0f, 0.0f, 0.0f);
D2D1::ColorF clrDarkGrey	= D2D1::ColorF(0.3f, 0.3f, 0.3f);
D2D1::ColorF clrWhite		= D2D1::ColorF(1.0f, 1.0f, 1.0f);
D2D1::ColorF clrBlue		= D2D1::ColorF(0.0f, 0.0f, 1.0f);

int nCurrentVersion = 13;

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

	plPlayer = new Player(this, 384.0f, 384.0f);
	vEntities.push_back(plPlayer);

	vItems.push_back(new LaserWeapon(LaserWeapon::Normal));
	vItems.push_back(new OrbWeapon());

	nCurrentItem = 0;

	fNextEnemySpawn = 0.0;

	fDifficulty = 80.0f;
	nWave = 0;
	fSecondsUntilNextWave = 0.0f;
	nEnemies = 0;
	fSecondsUntilNextComet = 40 + random() * 40;

	LoadFromFile(); //Load save game if it exists

	Game::sgSpaceGame = this;
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

	OutputDebugString(L"SpaceGame::Unload locking\n");
	for (Entity* entity : vEntities)
		delete entity;

	for (Item* item : vItems)
		delete item;

	for (BackgroundObject* BackgroundObject : vBackgroundObjects)
		delete BackgroundObject;

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

	for (Entity* entity : vEntities)
		if (entity)
			entity->Draw();

	tForegroundTexture->Draw(0, -fBackgroundPosition - 65.0f, 475.0f);

	for (BackgroundObject* backgroundobject : vBackgroundObjects)
		if (backgroundobject)
			backgroundobject->Draw();

	wchar_t txtHealth[64];
	swprintf_s(txtHealth, 64, L"HP %u / %u", (int)plPlayer->fHealth, (int)plPlayer->fMaxHealth);
	Graphics::WriteText(txtHealth, 5, 2, 14); //2
	Graphics::FillRectangle(5, 2 + 14 + 4, 100.0f * (plPlayer->fHealth / plPlayer->fMaxHealth), 20, clrRed); //18
	Graphics::DrawRectangle(5, 2 + 14 + 4, 100, 20, clrDarkGrey);

	wchar_t txtEnergy[64];
	swprintf_s(txtEnergy, 64, L"Energy %u / %u", (int)plPlayer->nEnergy, (int)plPlayer->nMaxEnergy);
	Graphics::WriteText(txtEnergy, 5, 2 + 14 + 2 + 20 + 2, 14); //40
	Graphics::FillRectangle(5, 2 + 14 + 2 + 20 + 2 + 14 + 5, 100.0f * (plPlayer->nEnergy / plPlayer->nMaxEnergy), 20, clrBlue); //56
	Graphics::DrawRectangle(5, 2 + 14 + 2 + 20 + 2 + 14 + 5, 100, 20, clrDarkGrey);

	DWRITE_TEXT_METRICS tmTextMetrics;
	Graphics::TextMetrics(L"Items", 14.0f, tmTextMetrics);
	Graphics::FillRectangle(0, nScreenHeight - 4 - 32 - 4 - 14, max(32 * vItems.size(), tmTextMetrics.width) + 10, 14 + 4 + 32 + 4, clrBlack);
	Graphics::WriteText(L"Items", 5, nScreenHeight - 4 - 32 - 4 - 14, 14);

	int nItem = 0;
	for (Item* item : vItems)
	{
		item->tTexture->Draw(0, 4 + nItem * 32, nScreenHeight - 4 - 32, true);
		Graphics::DrawRectangle(4 + nItem * 32, nScreenHeight - 4 - 32, 32, 32, nItem == nCurrentItem ? clrWhite : clrDarkGrey);
		if (item->nCount > 1)
		{
			wchar_t txtItemCount[32];
			swprintf_s(txtItemCount, 32, L"%d", item->nCount);
			Graphics::WriteText(txtItemCount, 6 + nItem * 32, nScreenHeight - 4 - 16, 14.0f);
		}
		nItem++;
	}

	wchar_t txtMoney[64];
	swprintf_s(txtMoney, 64, L"$%d", (int)plPlayer->fMoney);
	Graphics::TextMetrics(txtMoney, 16.0f, tmTextMetrics);
	Graphics::WriteText(txtMoney, nScreenWidth - 5 - tmTextMetrics.width, 5, 16.0f);

	wchar_t txtWave[64];
	swprintf_s(txtWave, 64, L"Wave %d (%d seconds left)", nWave, (int)fSecondsUntilNextWave);
	Graphics::TextMetrics(txtWave, 16.0f, tmTextMetrics);
	Graphics::FillRectangle(nScreenWidth - 5 - tmTextMetrics.width - 5, nScreenHeight - 5 - tmTextMetrics.height - 5, 5 + tmTextMetrics.width + 3, 5 + 16 + 5, clrBlack);
	Graphics::WriteText(txtWave, nScreenWidth - 5 - tmTextMetrics.width, nScreenHeight - 5 - tmTextMetrics.height, 16.0f);

	if (bShowDebugInfo)
	{
		wchar_t txtDebug[64];
		swprintf_s(txtDebug, 64, L"%d physics updates per second", (int)(fPhysicsUpdatesPerSeconds / 10) * 10
		);
		Graphics::TextMetrics(txtDebug, 16.0f, tmTextMetrics);
		Graphics::WriteText(txtDebug, nScreenWidth / 2 - tmTextMetrics.width / 2, 4, 16.0f);
	}
}
void SpaceGame::Update(double deltatime)
{	 
	fPhysicsUpdatesPerSeconds = 1.0 / deltatime;

	for (int i = 0; i < vEntities.size(); i++) //Entity updates //TODO optimise
	{
		Entity* entity = vEntities[i];
		bool bEntityExists = entity->Update(deltatime);
		if (!bEntityExists)
		{
			vEntities.erase(vEntities.begin() + i);
			delete entity;
			i--;
		}
		if (!bGameRunning) //Game could end after any entity update
		{
			Game::LoadLevel(new DeathScreen());
			return;
		}
	}

	for (BackgroundObject*& backgroundobjects : vBackgroundObjects) //Object updates
	{
		if (backgroundobjects)
			if (backgroundobjects->Update(deltatime) == false)
			{
				delete backgroundobjects; //TODO fix
				backgroundobjects = nullptr;
			}
	}

	fNextEnemySpawn -= deltatime; //Enemy spawning
	if (fNextEnemySpawn <= 0)
	{
		if (fSecondsUntilNextWave > 0.0f) //Don't spawn near end of wave
		{
			if (random() >= 0.15f)
			{
				Enemy* enemy = new Enemy(this, random_off_screen(), random() * 500);
				if (!enemy->bLegalPosition)
					delete enemy;
				else
				{
					vEntities.push_back(enemy);
					nEnemies++;
				}
			}
			else
			{
				Crab* crab = new Crab(this, random_off_screen());
				if (!crab->bLegalPosition)
					delete crab;
				else
				{
					vEntities.push_back(crab);
					nEnemies++;
				}
			}
		}

		fNextEnemySpawn = (random() * 10.0f) / (fDifficulty / 60.0f);
	}

	fSecondsUntilNextWave -= deltatime; //Waves
	if (fSecondsUntilNextWave < 0.0f) fSecondsUntilNextWave = 0.0f;
	if (fSecondsUntilNextWave <= 0.0f && nEnemies == 0)
	{
		nWave++;
		fSecondsUntilNextWave = 60.0f;
		fDifficulty *= 1.5f;

		plPlayer->fMaxHealthUpgrade += 50.0f;
		plPlayer->fMaxEnergyRechargeSpeed += 1.0f;
		plPlayer->fMaxMovementSpeed += 10.0f;
		plPlayer->fMaxHealthRegeneration += 1.0f;
	}

	fSecondsUntilNextComet -= deltatime; //Comet
	if (fSecondsUntilNextComet < 0.0f)
	{
		vBackgroundObjects.push_back(new Comet());
		fSecondsUntilNextComet = 40 + random() * 40;
	}
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
	if (key == 'P')
		if (plPlayer->puCurrentPowerup == nullptr)
			plPlayer->puCurrentPowerup = new EnergyPowerup(this);
	if (key == 'H')
		bShowHitboxes = !bShowHitboxes;
	if (key == VK_F2)
		bShowDebugInfo = !bShowDebugInfo;
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

	f << nCurrentVersion << " " << fDifficulty << " " << nWave << " " << fSecondsUntilNextWave << " " << nEnemies << " ";

	f << vEntities.size() << " ";
	for (Entity* entity : vEntities)
		entity->Save(f);

	f << vItems.size() << " ";
	for (Item* item : vItems)
		item->Save(f);

	f.close();
}

void SpaceGame::LoadFromFile()
{
	std::fstream f;
	f.open("savegame.txt", std::fstream::in);

	if (!f.good()) return;



	int nVersion;
	f >> nVersion >> fDifficulty >> nWave >> fSecondsUntilNextWave >> nEnemies;
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
			Player* e = new Player(this, 0.0f, 0.0f);
			e->Load(f);
			vEntities.push_back(e);
			plPlayer = e;
			break;
		}
		case Entity::Type::Bomb:
		{
			Bomb* e = new Bomb(this, 0.0f, 0.0f, 0.0f, 0.0f, 0);
			e->Load(f);
			vEntities.push_back(e);
			break;
		}
		case Entity::Type::Crab:
		{
			Crab* e = new Crab(this, 0.0f);
			e->Load(f);
			vEntities.push_back(e);
			break;
		}
		case Entity::Type::Enemy:
		{
			Enemy* e = new Enemy(this, 0.0f, 0.0f);
			e->Load(f);
			vEntities.push_back(e);
			break;
		}
		case Entity::Type::Laser:
		{
			LaserBeam* e = new LaserBeam(this, nullptr, 0.0f, 0.0f, 0.0f, 0.0f);
			e->Load(f);
			vEntities.push_back(e);
			break;
		}
		case Entity::Type::Orb:
		{
			Orb* e = new Orb(this, 0.0f, 0.0f, 0.0f, 0.0f);
			e->Load(f);
			vEntities.push_back(e);
			break;
		}
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
			BombWeapon* i = new BombWeapon(0);
			i->Load(f);
			vItems.push_back(i);
			break;
		}
		case Item::Type::Laser:
		{
			LaserWeapon* i = new LaserWeapon(LaserWeapon::LaserLevel::Normal);
			i->Load(f);
			vItems.push_back(i);
			break;
		}
		case Item::Type::Orb:
		{
			OrbWeapon* i = new OrbWeapon();
			i->Load(f);
			vItems.push_back(i);
			break;
		}
		case Item::Type::EnergyPowerup:
		{
			EnergyPowerupItem* i = new EnergyPowerupItem();
			i->Load(f);
			vItems.push_back(i);
			break;
		}
		case Item::Type::RegenerationPowerup:
		{
			RegenerationPowerupItem* i = new RegenerationPowerupItem();
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