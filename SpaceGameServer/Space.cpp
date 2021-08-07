#include "Space.h"
#include "Player.h"
#include "Orb.h"
#include "Laser.h"
#include "Enemy.h"
#include "Bomb.h"
#include "Crab.h"
#include "EnergyPowerup.h"
#include "RegenerationPowerup.h"
#include "nlohmann/json.hpp"
#include "Utilities.h"
#include <math.h>
#include <random>
#include <time.h>
#include <fstream>
#include <algorithm>


float fPhysicsUpdatesPerSeconds;

int nCurrentVersion = 16;

SpaceGameServer::SpaceGameServer()
	: pEventHandler(std::make_shared<EventHandler>())
{	
	nPlayers = 0;

	fNextEnemySpawn = 0.0;

	fDifficulty = 80.0f;
	nWave = 0;
	fSecondsUntilNextWave = 0.0f;
	nEnemies = 0;

	NextWave();

	//LoadFromFile(); //Load save game if it exists

	//Game::sgSpaceGame = this;
};

SpaceGameServer::~SpaceGameServer()
{
	
}

void SpaceGameServer::Update(float deltatime)
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
			assert(0);
			return;
		}
	}

	fNextEnemySpawn -= deltatime; //Enemy spawning
	if (fNextEnemySpawn <= 0 && fSecondsUntilNextWave > 0.0f)
	{
		if (randomf() >= 0.15f)
		{
			auto enemy = std::make_shared<Enemy>(randomf() * 5120.0f, 0 - randomf() * 100.0f);
			if (enemy->bLegalPosition)
			{
				vEntities.push_back(enemy);
				nEnemies++;
			}
		}
		else
		{
			auto crab = std::make_shared<Crab>(randomf() > 0.5f ? -100.0 : 5120 + 100);
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
}

void SpaceGameServer::NextWave()
{
	nWave++;
	fSecondsUntilNextWave = 60.0f;
	fDifficulty *= 1.5f;

	for (auto& plPlayer : vPlayers)
	{
		plPlayer->fMaxHealthUpgrade += 50.0f;
		plPlayer->fMaxEnergyRechargeSpeed += 1.0f;
		plPlayer->fMaxMovementSpeed += 10.0f;
		plPlayer->fMaxHealthRegeneration += 1.0f;
	}
	bWaveFinished = false;
}

nlohmann::json SpaceGameServer::GetState()
{
	return nlohmann::json();
}

void SpaceGameServer::SetState(nlohmann::json& j)
{
}
