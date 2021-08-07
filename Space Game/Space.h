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

	int nCurrentItem;
	std::vector<std::shared_ptr<Item>> vItems;

	std::shared_ptr<Player> plPlayer;

	std::vector<std::shared_ptr<BackgroundObject>> vBackgroundObjects;
	
	float fFps;

	float fNextEnemySpawn;
	float fDifficulty;

	int nWave;
	float fSecondsUntilNextWave;
	int nEnemies;
	int nPlayers;
	bool bWaveFinished;

	float fSecondsUntilNextComet;
	float fLightingLoopTime;

	std::shared_ptr<EventHandler> pEventHandler;

	static void LoadResources();
	static void UnloadResources();

	void Render() override;
	void Update(float deltatime) override;
	void LeftClick() override;
	void KeyDown(int key) override;
	void NextWave();
	SpaceGame();
	~SpaceGame();
};

extern float fBackgroundPosition;

extern Texture* textures[];
namespace TextureID
{
	enum TextureID
	{
		Character, Orb, Background, Laser, Laserbeam, Enemy, Bomb, Crab, Comet, None, BombAnimation, EnergyPowerup, RegenerationPowerup, Light, Foreground
	};
};
//extern Texture* tCharacterTexture, * tOrbTexture, * tBackground, * tLaserTexture, * tLaserBeamTexture, * tEnemyTexture, * tBombTexture, * tCrabTexture;
//extern Texture* tCometTexture, * tNoTexture, * tBombAnimationTexture, *tEnergyPowerupTexture, * tRegenerationPowerupTexture, * tLight;

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