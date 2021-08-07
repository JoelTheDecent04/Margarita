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

	vItems.push_back(std::make_shared<LaserWeapon>(LaserWeapon::Normal));
	vItems.push_back(std::make_shared<OrbWeapon>());

	nType = Type::Player;
	nTexture = TextureID::Character;

	static int last_id = 0;
	id = last_id++;
}
bool Player::Update(float deltatime) 
{
	if (fHealth <= 0.0f)
	{
		Destroy(nullptr);
		return false;
	}

	if (fSpeedX > 0)
	{
		fSpeedX -= fPlayerDecceleration * deltatime;
		if (fSpeedX < 0)
			fSpeedX = 0;
	}
	if (fSpeedX < 0)
	{
		fSpeedX += fPlayerDecceleration * deltatime;
		if (fSpeedX > 0)
			fSpeedX = 0;
	}
	if (!bOnGround)
		fSpeedY += fGravity * deltatime;

	fX += fSpeedX * deltatime;
	fY += fSpeedY * deltatime;

	if (fX < 0)
		fX = 0;
	if (fX > 5120)
		fX = 5120;
	if (fY <= 0.0f)
	{
		fY = 0.0f;
		fSpeedY = 0.0f;
	}
	if (fY >= 594.0f - fHeight / 2)
	{
		fY = 594.0f - fHeight / 2;
		fSpeedY = 0.0f;
		bOnGround = true;
	}

	fEnergy += (bOnGround ? fEnergyRechargeSpeed : fEnergyRechargeSpeed / 2.0f) * deltatime;
	if (fEnergy > fMaxEnergy) fEnergy = fMaxEnergy;
	fHealth += fHealthRegeneration * deltatime;
	if (fHealth > fMaxHealth) fHealth = fMaxHealth;
	
	if (fMoney >= 150.0f && vItems.size() <= 2) vItems.push_back(std::make_shared<BombWeapon>(1)); //TODO improve

	/*if (puCurrentPowerup)
		puCurrentPowerup->Update(deltatime);*/

	return true;
}
void Player::Destroy(Entity*) 
{
	sgSpaceGame->nPlayers--;
}

flatbuffers::Offset<NetPlayer> Player::SerialisePlayer(flatbuffers::FlatBufferBuilder& packet)
{
	return CreateNetPlayer(packet, id, fX - fWidth / 2.0f, fY - fHeight / 2.0f, nTexture, nFrame, fHealth, fEnergy);
}