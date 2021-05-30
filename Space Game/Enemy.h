#pragma once
#include "Entity.h"

class Enemy : public Entity
{
	float fSecondsUntilNextAttack;
public:
	bool bLegalPosition;
	Enemy(SpaceGame* game, float fX, float fY);
	void Update(double deltatime) override;
	void Destroy() override;
};