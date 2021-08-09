#include "Player.h"
#include "OnlineSpaceGame.h"
#include "Bomb.h"
#include "Laser.h"
#include "Orb.h"
#include "ServerUtilities.h"

Player::Player(float fX, float fY, const std::string& str)
	: Entity(fX, fY, 83, 58)
{
	fHealth = 200.0f;
	fMaxHealth = 200.0f;
	fEnergy = 150.0f;
	fMaxEnergy = 150.0f;
	fMoney = 0.0f;
	fEnergyRechargeSpeed = 3.0f;
	fMaxEnergyRechargeSpeed = 9.0f;
	fMaxHealthUpgrade = 450.0f;
	fWidth = 50.0f;
	fHeight = 58.0f;
	fMovementSpeed = 150.0f;
	fMaxMovementSpeed = 190.0f;
	//powerup = null
	fHealthRegeneration = 2.0f;
	fMaxHealthRegeneration = 5.0f;
	bOnGround = true;

	nType = Type::Player;
	nTexture = TextureID::Character;

	sgSpaceGame->nPlayers++;

	static int last_id = 0;
	id = last_id++;
	alive = true;
	ready = false;
}
bool Player::Update(float deltatime) 
{
	if (fHealth <= 0.0f)
	{
		Destroy(nullptr);
		return true; //Players stay loaded even after dying
	}

	fEnergy += (bOnGround ? fEnergyRechargeSpeed : fEnergyRechargeSpeed / 2.0f) * deltatime;
	if (fEnergy > fMaxEnergy) fEnergy = fMaxEnergy;
	fHealth += fHealthRegeneration * deltatime;
	if (fHealth > fMaxHealth) fHealth = fMaxHealth;

	/*if (puCurrentPowerup)
		puCurrentPowerup->Update(deltatime);*/

	return true;
}
void Player::Destroy(Entity*) 
{
	alive = false;
	sgSpaceGame->nPlayers--;
}

flatbuffers::Offset<NetPlayer> Player::SerialisePlayer(flatbuffers::FlatBufferBuilder& packet)
{
	return CreateNetPlayer(packet, id, fX - fWidth / 2.0f, fY - fHeight / 2.0f, nTexture, nFrame, fHealth, fEnergy, alive);
}