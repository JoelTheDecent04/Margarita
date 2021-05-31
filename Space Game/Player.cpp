#include "Player.h"
#include "Space.h"
#include "DeathScreen.h"
#include "Bomb.h"

static uint32_t nLastCharacterId = 0;

int keyJump1 = 'W';
int keyJump2 = VK_SPACE;
int keyMoveLeft1 = 'A';
int keyMoveLeft2 = 0;
int keyMoveRight1 = 'D';
int keyMoveRight2 = 0;
int keyMoveDown1 = 'S';
int keyMoveDown2 = 0;

Player::Player(SpaceGame* game, float fX, float fY, Texture* tTexture, std::wstring sName)
	: Entity(game, tTexture, fX, fY)
{
	this->sName = sName;
	fHealth = 200.0f;
	fMaxHealth = 200.0f;
	nEnergy = 100.0f;
	nMaxEnergy = 100;
	fMoney = 0.0f;
	fEnergyRechargeSpeed = 3.0f;
	fMaxEnergyRechargeSpeed = 10.0f;
	fMaxHealthUpgrade = 500.0f;
	fWidth = 72.0f;
	fHeight = 58.0f;
	fMovementSpeed = 150.0f;
	fMaxMovementSpeed = 200.0f;
}

void Player::Update(double deltatime)
{
	if (GetForegroundWindow() == Graphics::hWindow) //Controls
	{
		if (GetAsyncKeyState(keyMoveRight1) || GetAsyncKeyState(keyMoveRight2))
		{
			fSpeedX += fPlayerAcceleration * deltatime;
			if (fSpeedX > fMovementSpeed)
				fSpeedX = fMovementSpeed;
		}
		if (GetAsyncKeyState(keyMoveLeft1) || GetAsyncKeyState(keyMoveLeft2))
		{
			fSpeedX -= fPlayerAcceleration * deltatime;
			if (fSpeedX < -fMovementSpeed)
				fSpeedX = -fMovementSpeed;
		}
		if (GetAsyncKeyState(keyMoveDown1) || GetAsyncKeyState(keyMoveDown2))
			fSpeedY += fPlayerMoveDownSpeed * deltatime;
		if (GetAsyncKeyState(keyJump1) || GetAsyncKeyState(keyJump2))
		{
			fSpeedY = -180.0f;
			bOnGround = false;
		}
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
	fHealth += 3.0f * deltatime;
	if (fHealth > fMaxHealth) fHealth = fMaxHealth;

	if (fMoney >= 150.0f && sgGame->vItems.size() <= 2) sgGame->vItems.push_back(new BombWeapon()); //TODO improve
}

void Player::Destroy()
{
	sgGame->bGameRunning = false;
}
