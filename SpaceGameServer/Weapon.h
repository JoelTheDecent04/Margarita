#pragma once
#include "Space.h"
#include "Item.h"

class Weapon : public Item
{
public:
	Weapon(std::shared_ptr<Player>& player)
		: Item(player) {};
};