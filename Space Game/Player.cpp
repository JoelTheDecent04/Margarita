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

Player::Player(SpaceGame* game, float fX, float fY)
	: Entity(game, tCharacterTexture, fX, fY)
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
	if (fHealth == 0.0f)
	{
		sgGame->bGameRunning = false;
		return true; //So the player object is not deleted immediately
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

	if (fMoney >= 150.0f && sgGame->vItems.size() <= 2) sgGame->vItems.push_back(std::make_shared<BombWeapon>(1)); //TODO improve

	if (puCurrentPowerup)
	{
		puCurrentPowerup->Update(deltatime);
		nFrame = 1;
	}
	else
		nFrame = 0;

	return true;
}

void Player::Save(std::fstream& f)
{
	Entity::Save(f);
	f << fMoney << " " << nEnergy << " " << nMaxEnergy << " " << fEnergyRechargeSpeed << " " << fMaxEnergyRechargeSpeed << " " << fMaxHealthUpgrade << " " << fMovementSpeed << " " << fMaxMovementSpeed << " " << fHealthRegeneration << " " << fMaxHealthRegeneration << " ";
}

void Player::Load(std::fstream& f)
{
	Entity::Load(f);
	f >> fMoney >> nEnergy >> nMaxEnergy >> fEnergyRechargeSpeed >> fMaxEnergyRechargeSpeed >> fMaxHealthUpgrade >> fMovementSpeed >> fMaxMovementSpeed >> fHealthRegeneration >> fMaxHealthRegeneration;
}
