#pragma once
#include "Entity.h"

class Enemy : public Entity
{
	float fSecondsUntilNextAttack;
public:
	bool bLegalPosition;
	Enemy(SpaceGame* game, float fX, float fY);
	bool Update(double deltatime) override;
	void Destroy() override;
};