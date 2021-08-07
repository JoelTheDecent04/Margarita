#pragma once
#include "Space.h"
#include <enet/enet.h>

class OnlineSpaceGame : public SpaceGame
{
	ENetHost* client;
public:
	OnlineSpaceGame();
	void Update(float deltatime) override;
};

void SpaceServerMain();