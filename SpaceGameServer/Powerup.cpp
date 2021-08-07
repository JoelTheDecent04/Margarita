#include "Powerup.h"
#include "Player.h"


void Powerup::Remove()
{
	Game::sgSpaceGame->plPlayer->puCurrentPowerup = nullptr;
}
