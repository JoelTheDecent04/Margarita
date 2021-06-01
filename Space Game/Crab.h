#pragma once
#include "Entity.h"
#include "Animation.h"

class Crab : public Entity
{
	float fSecondsUntilNextAttack;
	Animation animation;
	
public:
	bool bLegalPosition;
	Crab(SpaceGame* game, float fX);
	bool Update(double deltatime) override;
	void Destroy() override;
};