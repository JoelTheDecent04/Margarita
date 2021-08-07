#pragma once
#include "Item.h"
#include "Player.h"

class Powerup
{
public:
	float fTimeLeft;
	std::shared_ptr<Player> plPlayer;

	Powerup(std::shared_ptr<Player>& player) : plPlayer(player) { fTimeLeft = 15.0f; };

	virtual void Update(float deltatime) = 0;
	void Remove();
};