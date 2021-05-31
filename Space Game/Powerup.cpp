#include "Powerup.h"
#include "Player.h"

void Powerup::Remove()
{
	for (Powerup*& p : sgGame->plPlayer->vPowerups)
	{
		if (p == this)
		{
			p = nullptr;
			delete this;
		}
	}
}
