#pragma once
#include "Game.h"

#ifdef SUPPORT_MULTIPLAYER
#include "Space.h"
#include "EntityOnline.h"
#include <enet/enet.h>

#include "../Margarita_generated.h"

struct OnlineItem
{
	int ID;
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
	float fMovementSpeed;
	bool bOnGround;
	bool alive;
	bool ready;
	std::vector<OnlineItem> vItems;
};

struct WeaponFire
{
	short weapon;
	float angle;
	short intensity;
};

class SpaceOnline : public Level
{
	int nPlayerID;
	OnlinePlayer player;
	int nCurrentItem;

	WeaponFire weapon_fire;
	bool bFiredWeapon;

	float fSecondsUntilNextWave;
	int nWave;

	ENetHost* client;
	ENetPeer* server_peer;

	const ServerPacket* last_packet;
	ENetPacket* last_enet_packet;

	bool bDidConnect;
	bool bGameStarted;

	std::vector<OnlineEntity> vEntities;

public:
	void Render() override;
	void Update(float deltatime) override;
	void LeftClick() override;
	void KeyDown(int key) override;
	SpaceOnline(const std::string& ip);
	~SpaceOnline();
};

#endif