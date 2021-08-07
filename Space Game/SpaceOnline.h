#pragma once
#include "Game.h"
#include "Space.h"
#include "EntityOnline.h"
#include <enet/enet.h>

#include "../Margarita_generated.h"

struct OnlineItem
{
	int nTexture;
	int nCount;
	std::string name;
};

struct OnlinePlayer
{
	float fX, fY, fSpeedX, fSpeedY;
	float fHealth, fMaxHealth;
	float fEnergy, fMaxEnergy;
	float fMoney;
	std::vector<OnlineItem> vItems;
};

class SpaceOnline : public Level
{
	int nPlayerID;
	OnlinePlayer player;
	int nCurrentItem;

	float fSecondsUntilNextWave;
	int nWave;

	ENetHost* client;
	ENetPeer* server_peer;

	const ServerPacket* last_packet;

	std::vector<OnlineEntity> vEntities;

public:
	void Render() override;
	void Update(float deltatime) override;
	void LeftClick() override;
	void KeyDown(int key) override;
	SpaceOnline(const std::string& ip);
	~SpaceOnline();
};