#include "Light.h"
#include "Space.h"

Light::Light(std::shared_ptr<Entity> parent)
	: Entity(parent->sgGame, tLight, parent->fX, parent->fY)
{
	this->parent = parent;
	bCanCollide = false;
	nType = Type::Light;
}

bool Light::Update(double deltatime)
{
	fX = parent->fX;
	fY = parent->fY;
	return true;
}