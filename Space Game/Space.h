#pragma once
#include "Game.h"
#include "Texture.h"
#include "BackgroundObject.h"
#include <concurrent_vector.h>

class Item;
class Entity;
class Player;

class SpaceGame : public Level
{	
public:
	concurrency::concurrent_vector<Entity*> vEntities;

	int nCurrentItem;
	concurrency::concurrent_vector<Item*> vItems;

	Player* plPlayer;

	concurrency::concurrent_vector<BackgroundObject*> vBackgroundObjects;
	
	float fFps;
	bool bGameRunning;

	double fNextEnemySpawn;
	float fDifficulty;

	int nWave;
	float fSecondsUntilNextWave;
	int nEnemies;

	float fSecondsUntilNextComet;

	void Load() override;
	void Unload() override;
	void Render() override;
	void Update(double deltatime) override;
	void LeftClick() override;
	void KeyDown(int key) override;
	SpaceGame() {};
};

extern float fBackgroundPosition;
extern Texture* tCharacterTexture, * tOrbTexture, * tBackground, * tLaserTexture, * tLaserBeamTexture, * tEnemyTexture, * tBombTexture, * tCrabTexture;
extern Texture* tCometTexture;

static const float fPlayerAcceleration = 3000.0f;
static const float fPlayerDecceleration = 1500.0f;
static const float fPlayerMaxSpeed = 150.0f;
static const float fGravity = 250.0f;
static const float fPlayerMoveDownSpeed = 500.0f;
static const float fFriction = 100.0f;