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
#include "Colours.h"
#include <math.h>
#include <random>
#include <time.h>
#include <fstream>
#include <algorithm>

using namespace Game;

float fBackgroundPosition = 0.0f;
Texture* textures[15];
//Texture* tCharacterTexture, * tOrbTexture, * tBackground, * tLaserTexture, * tLaserBeamTexture, * tEnemyTexture, * tBombTexture, * tCrabTexture;
//Texture* tForegroundTexture, * tCometTexture, * tNoTexture, * tBombAnimationTexture, * tEnergyPowerupTexture, * tRegenerationPowerupTexture;

int keyOpenShop1 = SDL_SCANCODE_E;
int keyOpenShop2 = 0;
int keyChangeWeapon1[9] = { SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3, SDL_SCANCODE_4, SDL_SCANCODE_5, 
SDL_SCANCODE_6, SDL_SCANCODE_7, SDL_SCANCODE_8, SDL_SCANCODE_9 };
int keyChangeWeapon2[9];

int keyNextWave1 = SDL_SCANCODE_R;
int keyNextWave2 = 0;

float fPhysicsUpdatesPerSeconds;

bool bShowHitboxes = false;

int nCurrentVersion = 16;

SpaceGame::SpaceGame() 
	: pEventHandler(std::make_shared<EventHandler>())
{
	plPlayer = std::make_shared<Player>(384.0f, 380.f);
	
	nPlayers = 0;

	vEntities.push_back(plPlayer);
	nPlayers++;

	vItems.push_back(std::make_shared<LaserWeapon>(LaserWeapon::Normal));
	vItems.push_back(std::make_shared<OrbWeapon>());

	nCurrentItem = 0;

	fNextEnemySpawn = 0.0;

	fDifficulty = 80.0f;
	nWave = 0;
	fSecondsUntilNextWave = 0.0f;
	nEnemies = 0;

	fSecondsUntilNextComet = 40 + randomf() * 40;
	NextWave();

	//LoadFromFile(); //Load save game if it exists

	//Game::sgSpaceGame = this;
};

SpaceGame::~SpaceGame()
{
	
}
void SpaceGame::LoadResources()
{
	textures[TextureID::Character] =		new Texture("player.png", 186, 116, 83, 58);
	textures[TextureID::Orb] =				new Texture("orb.png", 2497, 2497, 32, 32);
	textures[TextureID::Background] =		new Texture("background.png", 8192, 1152, 5120, 720);
	textures[TextureID::Laser] =			new Texture("Laser.png", 32, 32, 32.0f, 32.0f);
	textures[TextureID::Laserbeam] =		new Texture("LaserBeam.png", 2569, 765, 32, 10);
	textures[TextureID::Enemy] =			new Texture("enemy.png", 77, 110, 38, 55);
	textures[TextureID::Crab] =				new Texture("crab.png", 700, 350, 96, 48);
	textures[TextureID::Bomb] =				new Texture("bomb.png", 2218, 2223, 32.0f, 32.0f);
	textures[TextureID::Comet] =			new Texture("comet.png", 640, 360, 100, 50);
	textures[TextureID::None] =				new Texture("notexture.png", 32, 32, 32, 32);
	textures[TextureID::BombAnimation] =	new Texture("bomb_animation.png", 1280, 720, 100.0f, 100.0f);
	textures[TextureID::EnergyPowerup] =	new Texture("energy_powerup.png", 2600, 2600, 32.0f, 32.0f);
	textures[TextureID::RegenerationPowerup] = new Texture("regen_powerup.png", 2415, 2415, 32.0f, 32.0f);
	textures[TextureID::Foreground] =		new Texture("foreground.png", 8192, 204, 5120, 127.33f);
}
void SpaceGame::Render()
{
	fBackgroundPosition = plPlayer->fX - (1280 / 2);
	if (plPlayer->fX < (1280 / 2))
		fBackgroundPosition = 0;
	if (plPlayer->fX > 5120 - (1280 / 2))
		fBackgroundPosition = 5120 - 1280;

	textures[TextureID::Background]->DrawPanorama(fBackgroundPosition);

	//Graphics::iLightingDeviceContext->BeginDraw();

	//Graphics::iLightingDeviceContext->Clear(col);

	for (auto& entity : vEntities)
		entity->Draw();

	textures[TextureID::Foreground]->Draw(0, -fBackgroundPosition - 65.0f, 475.0f);
	
	Graphics::DrawLighting();

	for (auto& backgroundobject : vBackgroundObjects)
		backgroundobject->Draw();

	char txtBuf[64];

	snprintf(txtBuf, sizeof(txtBuf), "HP %u / %u", (int)plPlayer->fHealth, (int)plPlayer->fMaxHealth);
	Graphics::WriteText(txtBuf, 5, 2, Graphics::pFont14); //2
	Graphics::FillRectangle(5, 2 + 14 + 4, 100.0f * (plPlayer->fHealth / plPlayer->fMaxHealth), 20, clrRed); //18
	Graphics::DrawRectangle(5, 2 + 14 + 4, 100, 20, clrDarkGrey);

	snprintf(txtBuf, sizeof(txtBuf), "Energy %u / %u", (int)plPlayer->nEnergy, (int)plPlayer->nMaxEnergy);
	Graphics::WriteText(txtBuf, 5, 2 + 14 + 2 + 20 + 2, Graphics::pFont14); //40
	Graphics::FillRectangle(5, 2 + 14 + 2 + 20 + 2 + 14 + 5, 100.0f * (plPlayer->nEnergy / plPlayer->nMaxEnergy), 20, clrBlue); //56
	Graphics::DrawRectangle(5, 2 + 14 + 2 + 20 + 2 + 14 + 5, 100, 20, clrDarkGrey);

	TextSize textsize;
	Graphics::TextMetrics(vItems[nCurrentItem]->strName, Graphics::pFont14, textsize);
	Graphics::FillRectangle(0, nScreenHeight - 4 - 32 - 4 - 14, std::max((int)(32 * vItems.size()), textsize.width) + 10, 14 + 4 + 32 + 4, clrBlack);
	Graphics::WriteText(vItems[nCurrentItem]->strName, 5, nScreenHeight - 4 - 32 - 4 - 14, Graphics::pFont14);

	int nItem = 0;
	for (auto& item : vItems)
	{
		textures[item->nTexture]->Draw(0, 4 + nItem * 32, nScreenHeight - 4 - 32, true);
		Graphics::DrawRectangle(4 + nItem * 32, nScreenHeight - 4 - 32, 32, 32, nItem == nCurrentItem ? clrWhite : clrDarkGrey);
		if (item->nCount > 1)
		{
			snprintf(txtBuf, sizeof(txtBuf), "%d", item->nCount);
			Graphics::WriteText(txtBuf, 6 + nItem * 32, nScreenHeight - 4 - 16, Graphics::pFont14);
		}
		nItem++;
	}

	snprintf(txtBuf, 64, "$%d", (int)plPlayer->fMoney);
	Graphics::TextMetrics(txtBuf, Graphics::pFont16, textsize);
	Graphics::WriteText(txtBuf, nScreenWidth - 5 - textsize.width, 5, Graphics::pFont16);

	snprintf(txtBuf, 64, "Wave %d (%d seconds left)", nWave, (int)fSecondsUntilNextWave);
	Graphics::TextMetrics(txtBuf, Graphics::pFont16, textsize);
	Graphics::FillRectangle(nScreenWidth - 5 - textsize.width - 5, nScreenHeight  - textsize.height, 5 + textsize.width + 3, textsize.height, clrBlack);
	Graphics::WriteText(txtBuf, nScreenWidth - 5 - textsize.width, nScreenHeight - textsize.height, Graphics::pFont16);

	if (bWaveFinished)
	{
		snprintf(txtBuf, 64, "Wave Completed. Press '%s' To Continue.", ControlsScreen::KeyText(keyNextWave1));
		Graphics::TextMetrics(txtBuf, Graphics::pFont24, textsize);
		Graphics::WriteText(txtBuf, nScreenWidth / 2 - textsize.width / 2, 8, Graphics::pFont24);
	}

	snprintf(txtBuf, 64, "Objective: Kill %d x %s using %s", pEventHandler->nAchievementCount, astrEntityName[(int)pEventHandler->nAchievementEntityType2], astrEntityName[(int)pEventHandler->nAchievementEntityType1]);
	Graphics::TextMetrics(txtBuf, Graphics::pFont24, textsize);
	Graphics::WriteText(txtBuf, nScreenWidth / 2 - textsize.width / 2, nScreenHeight - 2 - textsize.height, Graphics::pFont24);

}
void SpaceGame::UnloadResources()
{

}
void SpaceGame::Update(float deltatime)
{
	fPhysicsUpdatesPerSeconds = 1.0 / deltatime;

	for (int i = 0; i < vEntities.size(); i++) //Entity updates
	{
		auto entity = vEntities[i];
		bool bEntityExists = entity->Update(deltatime);
		if (!bEntityExists)
		{
			vEntities.erase(vEntities.begin() + i);
			i--;
		}
		if (nPlayers == 0) //Game has ended
		{
			remove("save.json");
			LoadLevel(std::make_shared<DeathScreen>());
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
		if (randomf() >= 0.15f)
		{
			auto enemy = std::make_shared<Enemy>(random_off_screen(), randomf() * 500);
			if (enemy->bLegalPosition)
			{
				vEntities.push_back(enemy);
				nEnemies++;
			}
		}
		else
		{
			auto crab = std::make_shared<Crab>(random_off_screen());
			if (crab->bLegalPosition)
			{
				vEntities.push_back(crab);
				nEnemies++;
			}
		}

		fNextEnemySpawn = (randomf() * 10.0f) / (fDifficulty / 60.0f);
	}

	fSecondsUntilNextWave -= deltatime; //Waves
	if (fSecondsUntilNextWave < 0.0f) fSecondsUntilNextWave = 0.0f;
	if (fSecondsUntilNextWave <= 0.0f && nEnemies == 0)
		bWaveFinished = true;

	fSecondsUntilNextComet -= deltatime; //Comet
	if (fSecondsUntilNextComet < 0.0f)
	{
		vBackgroundObjects.push_back(std::make_shared<Comet>());
		fSecondsUntilNextComet = 40 + randomf() * 40;
	}

}
void SpaceGame::LeftClick()
{
	int nCursorX, nCursorY;
	GetRelativeMousePos(&nCursorX, &nCursorY);
	
	nCursorX += fBackgroundPosition;

	float fGradient = (nCursorY - plPlayer->fY) / (nCursorX - plPlayer->fX);
	float fAngle = atan(fGradient);
	if (nCursorX < plPlayer->fX) fAngle += 3.1415926f;
	
	vItems[nCurrentItem]->Use(plPlayer->fX, plPlayer->fY, fAngle);
	if (vItems[nCurrentItem]->nCount == 0)
	{
		vItems.erase(vItems.begin() + nCurrentItem);
		if (nCurrentItem >= vItems.size())
			nCurrentItem = vItems.size() - 1;
	}

}

void SpaceGame::KeyDown(int key)
{
	if (key == SDL_SCANCODE_ESCAPE)
		LoadLevel(std::static_pointer_cast<Level>(std::make_shared<PauseScreen>()));
	if (key == keyOpenShop1 || key == keyOpenShop2)
		LoadLevel(std::static_pointer_cast<Level>(std::make_shared<ShopScreen>()));
	if (key == SDL_SCANCODE_K)
		plPlayer->ChangeHealth(-20.0f, nullptr);
	if (key == SDL_SCANCODE_J)
		plPlayer->ChangeHealth(20.0f, nullptr);
	if (key == SDL_SCANCODE_M)
		plPlayer->fMoney += 200.0f;
	if (key == SDL_SCANCODE_H)
		bShowHitboxes = !bShowHitboxes;
	if (key == keyNextWave1 || key == keyNextWave2)
		if (bWaveFinished)
			NextWave();
	for (int i = 0; i < 9; i++)
	{
		if (key == keyChangeWeapon1[i] || key == keyChangeWeapon2[i])
			if (vItems.size() >= i + 1)
				nCurrentItem = i;
	}

	/*if (key == ) //[
	{
		nCurrentItem++;
		if (nCurrentItem >= vItems.size()) nCurrentItem = 0;
	}
	if (key == VK_OEM_4) //]
	{
		nCurrentItem--;
		if (nCurrentItem < 0) nCurrentItem = vItems.size() - 1;
	}*/
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