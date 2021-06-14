#pragma once

class Entity;
class SpaceGame;

class Objective
{

};

class EventHandler
{
	SpaceGame* game;
public:
	EventHandler(SpaceGame* game);
	enum class Type
	{
		None,
		Damage,
		Kill,
		Hit
	};
	
	void Event(Type nType, int nCount, Entity* a, Entity* b);
};