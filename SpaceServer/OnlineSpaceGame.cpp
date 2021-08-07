#include "OnlineSpaceGame.h"
#include "Player.h"
#include "ServerUtilities.h"
#include "Enemy.h"
#include "Crab.h"

#include <iostream>

std::shared_ptr<OnlineSpaceGame> sgSpaceGame;

OnlineSpaceGame::OnlineSpaceGame()
// : pEventHandler(std::make_unique<EventHandler>())
{
	nPlayers = 0;
	fNextEnemySpawn = 0.0f;
	fDifficulty = 80.0f;
	nWave = 0;
	fSecondsUntilNextWave = 0.0f;
	nEnemies = 0;
	bWaveFinished = false;
	
	NextWave();
}

bool OnlineSpaceGame::Update(float deltatime)
{
	//Entity updates
	for (auto entity = vEntities.begin(); entity != vEntities.end(); entity++) 
	{
		bool entity_exists = (*entity)->Update(deltatime);
		if (entity_exists == false)
		{
			vEntities.erase(entity);
		}
		if (nPlayers == 0)
		{
			//Game has ended
			//return false;
		}
	}

	//Enemy spawns
	fNextEnemySpawn -= deltatime;
	if (fNextEnemySpawn <= 0.0f && fSecondsUntilNextWave > 0.0f)
	{
		if (randomf() >= 0.15f)
		{
			auto enemy = std::make_shared<Enemy>(randomf() * 5120.0f, 0 - randomf() * 100);
			if (enemy->bLegalPosition)
			{
				vEntities.push_back(enemy);
				nEnemies++;
			}
		}
		else
		{
			auto crab = std::make_shared<Crab>(randomf() * 5120);
			if (crab->bLegalPosition)
			{
				vEntities.push_back(crab);
				nEnemies++;
			}
		}
		fNextEnemySpawn = (randomf() * 10.0f) / (fDifficulty / 60.0f);
	}

	//Wave
	fSecondsUntilNextWave -= deltatime;
	if (fSecondsUntilNextWave < 0.0f)
	{
		fSecondsUntilNextWave = 0.0f;
		if (fSecondsUntilNextWave < 0.0f && nEnemies == 0)
			bWaveFinished = true;
	}
	
	return true;
}
void OnlineSpaceGame::NextWave()
{
	nWave++;
	fSecondsUntilNextWave = 60.0f;
	fDifficulty *= 1.5f;

	for (auto& entity : vEntities)
	{
		if (entity->nType == Entity::Type::Player)
		{
			Player* player = (Player*)entity.get();
			player->fMaxHealthUpgrade += 50.0f;
			player->fMaxEnergyRechargeSpeed += 1.0f;
			player->fMaxMovementSpeed += 10.0f;
			player->fMaxHealthRegeneration += 1.0f;
		}
	}

	bWaveFinished = false;
}

int OnlineSpaceGame::AddPlayer()
{
	auto new_player = std::make_shared<Player>(100.0f, 100.0f, "Player");
	vPlayers.push_back(new_player);

	return new_player->id;
}
