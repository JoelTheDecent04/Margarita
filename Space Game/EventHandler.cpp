#include "EventHandler.h"
#include "Utilities.h"
#include "Entity.h"
#include "Player.h"

Entity::Type anWeapons[3] = { Entity::Type::Bomb, Entity::Type::Laser, Entity::Type::Orb };
Entity::Type anEnemies[2] = { Entity::Type::Enemy, Entity::Type::Crab };

void EventHandler::MakeNewAchievement()
{
	int n = random() * 6;
	n = n * 5 + 10;
	nAchievementCount = n;
	nAchievementEntityType1 = anWeapons[(int)(random() * 2)];
	nAchievementEntityType2 = anEnemies[(int)(random() * 1)];
	nAchievementType = Type::Kill;
}

EventHandler::EventHandler(SpaceGame* game)
{
	this->game = game;
	MakeNewAchievement();
}

void EventHandler::Event(Type nType, int nCount, Entity* a, Entity* b)
{
	switch (nType)
	{
	case Type::None: break;
	case Type::Damage:
		DebugOut(L"Damaged %d enemies with weapon %d\n", nCount, a->nType);

		break;
	case Type::Hit:
		DebugOut(L"Hit entity %d with weapon %d\n", b->nType, a->nType);
		break;
	case Type::Kill:
		DebugOut(L"Killed entity %d with weapon %d\n", b->nType, a->nType);
		break;
	default:
		abort();
	}

	if (nType == nAchievementType)
	{
		switch (nType)
		{
		case Type::Kill:
		{
			if (a->nType == nAchievementEntityType1 && b->nType == nAchievementEntityType2)
			{
				nAchievementCount--;
 				if (nAchievementCount == 0)
				{
 					MakeNewAchievement();
					game->plPlayer->fMoney += 120.0f;
					game->plPlayer->ChangeHealth(100.0f, nullptr);
				}
			}
		}
		}
	}
}