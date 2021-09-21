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
	if (nPlayers == 0)
		return false;
	//std::cout << "Delta time: " << deltatime << "\n";

	//Entity updates
	for (auto entity = vEntities.begin(); entity != vEntities.end(); ) 
	{
		bool entity_exists = (*entity)->Update(deltatime);
		if (entity_exists == false)
			entity = vEntities.erase(entity);
		else
			entity++; //To prevent deleting current iterator
	}

	//Player updates
	for (auto player = vPlayers.begin(); player != vPlayers.end(); player++)
	{
		if (player->second->alive)
			player->second->Update(deltatime);
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
			auto crab = std::make_shared<Crab>(randomf() > 0.5f ? -100.0f : 5120.0f + 100.0f);
			if (crab->bLegalPosition)
			{
				vEntities.push_back(crab);
				nEnemies++;
			}
		}
		fNextEnemySpawn = (randomf() * 10.0f) / (fDifficulty / 60.0f);
	}

	//Wave
	if (!bWaveFinished)
	{
		fSecondsUntilNextWave -= deltatime;
		if (fSecondsUntilNextWave <= 0.0f)
		{
			fSecondsUntilNextWave = 0.0f;
			if (nEnemies == 0)
			{
				bWaveFinished = true;
				std::cout << "Wave " << nWave << " finished\n";
			}
		}
	}
	else
	{
		int nReadyPlayers = 0;
		for (auto& player : vPlayers)
			if (player.second->alive && player.second->ready)
				nReadyPlayers++;

		if ((float)nReadyPlayers / nPlayers > 0.5f)
			NextWave();
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
	vPlayers[new_player->id] = new_player;

	return new_player->id;
}
