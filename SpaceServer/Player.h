#pragma once
#include "Entity.h"
#include "Item.h"

#include <string>
#include <vector>
#include <memory>

class Player : public Entity
{
public:
	//Powerup

	std::string strName;
	float fMoney;
	float fEnergy;
	float fMaxEnergy;
	float fEnergyRechargeSpeed;
	float fMaxEnergyRechargeSpeed;
	float fMaxHealthUpgrade;
	float fMovementSpeed;
	float fMaxMovementSpeed;
	float fHealthRegeneration;
	float fMaxHealthRegeneration;

	bool bOnGround;
	int id;
	bool alive;
	bool ready;

	//std::vector<std::shared_ptr<Item>> vItems;

	Player(float fX, float fY, const std::string& str);
	bool Update(float deltatime) override;
	void Destroy(Entity* entity) override;
	flatbuffers::Offset<NetPlayer> SerialisePlayer(flatbuffers::FlatBufferBuilder& packet);
};