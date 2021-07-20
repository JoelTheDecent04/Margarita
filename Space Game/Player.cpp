#include "Player.h"
#include "Space.h"
#include "DeathScreen.h"
#include "Bomb.h"

static uint32_t nLastCharacterId = 0;

int keyJump1 = SDL_SCANCODE_W;
int keyJump2 = SDL_SCANCODE_SPACE;
int keyMoveLeft1 = SDL_SCANCODE_A;
int keyMoveLeft2 = 0;
int keyMoveRight1 = SDL_SCANCODE_D;
int keyMoveRight2 = 0;
int keyMoveDown1 = SDL_SCANCODE_S;
int keyMoveDown2 = 0;

Player::Player(float fX, float fY)
	: Entity(tCharacterTexture, fX, fY)
{
	fHealth = 200.0f;
	fMaxHealth = 200.0f;
	nEnergy = 150.0f;
	nMaxEnergy = 150.0f;
	fMoney = 0.0f;
	fEnergyRechargeSpeed = 3.0f;
	fMaxEnergyRechargeSpeed = 9.0f;
	fMaxHealthUpgrade = 450.0f;
	fWidth = 50.0f;
	fHeight = 58.0f;
	fMovementSpeed = 150.0f;
	fMaxMovementSpeed = 190.0f;
	puCurrentPowerup = nullptr;
	fHealthRegeneration = 2.0f;
	fMaxHealthRegeneration = 5.0f;

	nType = Type::Player;
}

bool Player::Update(float deltatime)
{
	if (fHealth <= 0.0f)
	{
		Destroy();
		return false;
	}

	if (GetKeyState(keyMoveRight1) || GetKeyState(keyMoveRight2))
	{
		fSpeedX += fPlayerAcceleration * deltatime;
		if (fSpeedX > fMovementSpeed)
			fSpeedX = fMovementSpeed;
	}
	if (GetKeyState(keyMoveLeft1) || GetKeyState(keyMoveLeft2))
	{
		fSpeedX -= fPlayerAcceleration * deltatime;
		if (fSpeedX < -fMovementSpeed)
			fSpeedX = -fMovementSpeed;
	}
	if (GetKeyState(keyMoveDown1) || GetKeyState(keyMoveDown2))
		fSpeedY += fPlayerMoveDownSpeed * deltatime;
	if (GetKeyState(keyJump1) || GetKeyState(keyJump2))
	{
		fSpeedY = -180.0f;
		bOnGround = false;
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

	nEnergy += (bOnGround ? fEnergyRechargeSpeed : fEnergyRechargeSpeed / 2.0f) * deltatime;
	if (nEnergy > nMaxEnergy) nEnergy = nMaxEnergy;
	fHealth += fHealthRegeneration * deltatime;
	if (fHealth > fMaxHealth) fHealth = fMaxHealth;

	if (fMoney >= 150.0f && Game::sgSpaceGame->vItems.size() <= 2) Game::sgSpaceGame->vItems.push_back(std::make_shared<BombWeapon>(1)); //TODO improve

	if (puCurrentPowerup)
	{
		puCurrentPowerup->Update(deltatime);
		nFrame = 1;
	}
	else
		nFrame = 0;

	return true;
}

nlohmann::json Player::Save()
{
	nlohmann::json j = Entity::Save();

	j["money"] = fMoney;
	j["energy"] = nEnergy;
	j["max_energy"] = nMaxEnergy;
	j["energy_recharge_speed"] = fEnergyRechargeSpeed;
	j["max_energy_recharge_speed"] = fMaxEnergyRechargeSpeed;
	j["max_health_upgrade"] = fMaxHealthUpgrade;
	j["movement_speed"] = fMovementSpeed;
	j["max_movement_speed"] = fMaxMovementSpeed;
	j["health_regeneration"] = fHealthRegeneration;
	j["max_health_regeneration"] = fMaxHealthRegeneration;

	return j;
}

void Player::Load(nlohmann::json& j)
{
	Entity::Load(j);
	fMoney = j["money"].get<float>();
	nEnergy = j["energy"].get<float>();
	nMaxEnergy = j["max_energy"].get<float>();
	fEnergyRechargeSpeed = j["energy_recharge_speed"].get<float>();
	fMaxEnergyRechargeSpeed = j["max_energy_recharge_speed"].get<float>();
	fMaxHealthUpgrade = j["max_health_upgrade"].get<float>();
	fMovementSpeed = j["movement_speed"].get<float>();
	fMaxMovementSpeed = j["max_movement_speed"].get<float>();
	fHealthRegeneration = j["health_regeneration"].get<float>();
	fMaxHealthRegeneration = j["max_health_regeneration"].get<float>();
}

void Player::Destroy()
{
	Game::sgSpaceGame->nPlayers--;
}
