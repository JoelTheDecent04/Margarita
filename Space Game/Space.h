#pragma once
#include "Game.h"
#include "Texture.h"
#include "BackgroundObject.h"
#include "EventHandler.h"
#include <vector>
#include <memory>

class Item;
class Entity;
class Player;

class SpaceGame : public Level
{	
public:
	std::vector<std::shared_ptr<Entity>> vEntities;

	int nCurrentItem{ 0 };
	std::vector<std::shared_ptr<Item>> vItems;

	std::shared_ptr<Player> plPlayer;

	std::vector<std::shared_ptr<BackgroundObject>> vBackgroundObjects;
	
	float fFps{ 0.0f };
	bool bGameRunning { false };

	float fNextEnemySpawn{ 0.0 };
	float fDifficulty{ 0.0f };

	int nWave{ 0 };
	float fSecondsUntilNextWave{ 0.0f };
	int nEnemies{ 0 };
	bool bWaveFinished{ false };

	float fSecondsUntilNextComet{ 0.0f };
	float fLightingLoopTime{ 0.0f };
	float fBrightness{ 0.0f };

	std::shared_ptr<EventHandler> pEventHandler;

	void Load() override;
	void Unload() override;
	void Render() override;
	void Update(float deltatime) override;
	void LeftClick() override;
	void KeyDown(int key) override;
	void Save();
	void LoadFromFile();
	void NextWave();
	SpaceGame() : pEventHandler(std::make_shared<EventHandler>(this)) {};
};

extern float fBackgroundPosition;
extern Texture* tCharacterTexture, * tOrbTexture, * tBackground, * tLaserTexture, * tLaserBeamTexture, * tEnemyTexture, * tBombTexture, * tCrabTexture;
extern Texture* tCometTexture, * tNoTexture, * tBombAnimationTexture, *tEnergyPowerupTexture, * tRegenerationPowerupTexture, * tLight;

static const float fPlayerAcceleration = 3000.0f;
static const float fPlayerDecceleration = 1500.0f;
static const float fPlayerMaxSpeed = 150.0f;
static const float fGravity = 250.0f;
static const float fPlayerMoveDownSpeed = 500.0f;
static const float fFriction = 100.0f;

extern int keyOpenShop1, keyOpenShop2;
extern int keyNextWave1, keyNextWave2;

extern int keyChangeWeapon1[9];
extern int keyChangeWeapon2[9];

extern bool bShowHitboxes;