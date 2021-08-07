#pragma once
#include "EventHandler.h"
#include "Item.h"
#include <vector>
#include <memory>

class Item;
class Entity;
class Player;

class SpaceGameServer
{	
public:
	std::vector<std::shared_ptr<Entity>> vEntities;
	std::vector<std::shared_ptr<Player>> vPlayers;

	float fNextEnemySpawn;
	float fDifficulty;

	int nWave;
	float fSecondsUntilNextWave;
	int nEnemies;
	int nPlayers;
	bool bWaveFinished;

	std::shared_ptr<EventHandler> pEventHandler;

	void Update(float deltatime);
	void KeyDown(int key);
	void NextWave();
	nlohmann::json GetState();
	void SetState(nlohmann::json& j);
	SpaceGameServer();
	~SpaceGameServer();
};

static const float fPlayerAcceleration = 3000.0f;
static const float fPlayerDecceleration = 1500.0f;
static const float fPlayerMaxSpeed = 150.0f;
static const float fGravity = 250.0f;
static const float fPlayerMoveDownSpeed = 500.0f;
static const float fFriction = 100.0f;

extern std::shared_ptr<SpaceGameServer> sgSpaceGame;