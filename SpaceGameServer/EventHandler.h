#pragma once
#include "Entity.h"

class EventHandler
{
	void MakeNewAchievement();
public:
	EventHandler();
	enum class Type
	{
		None,
		Damage,
		Kill,
		Hit
	};
	Type nAchievementType;
	Entity::Type nAchievementEntityType1;
	Entity::Type nAchievementEntityType2;
	int nAchievementCount;
	//Damage # enemies with $
	//Kill # $s

	
	void Event(Type nType, int nCount, Entity* a, Entity* b);

};