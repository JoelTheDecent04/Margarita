#pragma once
#include "Entity.h"

class Crab : public Entity
{
	float fSecondsUntilNextAttack;
	
public:
	bool bLegalPosition;
	Crab(float fX);
	bool Update(float deltatime) override;
	void Destroy() override;
};