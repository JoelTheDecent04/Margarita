#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include "Entity.h"

class OnlineSpaceGame
{
public:
	int nPlayers;

	float fNextEnemySpawn;
	float fDifficulty;
	int nWave;
	float fSecondsUntilNextWave;
	int nEnemies;
	bool bWaveFinished;

	//std::unique_ptr<EventHandler> pEventHandler;

	std::vector<std::shared_ptr<Entity>> vEntities;
	std::unordered_map<int, std::shared_ptr<Player>> vPlayers;
	
	//std::vector<std::shared_ptr<Player>> vPlayers; //Unordered map maybe

	OnlineSpaceGame();
	bool Update(float deltatime);
	void NextWave();
	int AddPlayer();
};

static const float fPlayerAcceleration = 3000.0f;
static const float fPlayerDecceleration = 1500.0f;
static const float fPlayerMaxSpeed = 150.0f;
static const float fGravity = 250.0f;
static const float fPlayerMoveDownSpeed = 500.0f;
static const float fFriction = 100.0f;

extern std::shared_ptr<OnlineSpaceGame> sgSpaceGame;