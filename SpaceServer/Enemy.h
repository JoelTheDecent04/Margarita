#pragma once
#include "Entity.h"

class Enemy : public Entity
{
	float fSecondsUntilNextAttack;
public:
	bool bLegalPosition;
	Enemy(float fX, float fY);
	bool Update(float deltatime) override;
	void Destroy(Entity* entity) override;
};