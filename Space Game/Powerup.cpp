#include "Powerup.h"
#include "Player.h"

void Powerup::Remove()
{
	sgGame->plPlayer->puCurrentPowerup = nullptr;
}
