#include "Entity.h"
#include "Space.h"
#include "EntityHealthChangeText.h"
#include "Utilities.h"

Entity::Entity(SpaceGame* sgGame, Texture* tTexture, float fX, float fY)
{
	this->sgGame = sgGame;
	this->tTexture = tTexture;
	this->fX = fX;
	this->fY = fY;
	nFrame = 0;
	fSpeedX = 0.0f;
	fSpeedY = 0.0f;
	bOnGround = false;
	bAffectedByGravity = true;
	fHealth = -1.0f;
	fMaxHealth = 100.0f;
	fHeight = tTexture->fTextureDrawnHeight;
	fWidth = tTexture->fTextureDrawnWidth;
	bCanCollide = true;
	bCanCollideWithPlayer = false;
}
void Entity::Draw()
{
	tTexture->Draw(nFrame, (fX - fBackgroundPosition - (tTexture->fTextureDrawnWidth / 2)), fY - tTexture->fTextureDrawnHeight / 2);
}

bool Entity::Update(double deltatime)
{
	if (fHealth == 0.0f)
	{
		Destroy();
		return false;
	}

	if (bAffectedByGravity) fSpeedY += fGravity * deltatime; //Gravity force

	float fNewX = fX + fSpeedX * deltatime; //Get new position
	float fNewY = fY + fSpeedY * deltatime;

	if (bCanCollide) //If this entity can collide
	{
		bool bCollided = false;
		for (Entity* entity : sgGame->vEntities) //Check for collisions
		{
			if (!entity) continue;
			if (entity == this || !entity->bCanCollide) continue;
			if (entity == (Entity*)sgGame->plPlayer && !bCanCollideWithPlayer) continue;
			if (entity->WillOverlap(this, fNewX, fNewY))
			{
				bCollided = true;
				if (Collide(entity) == false) return false;
				break;
			}
		}

		if (bCollided)
		{
 			bool bCollidedVertically = false;
			for (Entity* entity : sgGame->vEntities) //Check if it can move vertically
			{
				if (!entity) continue;
				if (entity == this || !entity->bCanCollide) continue;
				if (entity == (Entity*)sgGame->plPlayer && !bCanCollideWithPlayer) continue;
				if (entity->WillOverlap(this, fX, fNewY))
				{
					bCollidedVertically = true;
					break;
				}
			}
			if (bCollidedVertically)
			{
				bool bCollidedHorizontally = false;
				for (Entity* entity : sgGame->vEntities) //Check if it can move horizontally
				{
					if (!entity) continue;
					if (entity == this || !entity->bCanCollide) continue;
					if (entity == (Entity*)sgGame->plPlayer && !bCanCollideWithPlayer) continue;
					if (entity->WillOverlap(this, fNewX, fY))
					{
						bCollidedHorizontally = true;
						break;
					}
				}
				if (bCollidedHorizontally)
				{
					fSpeedX = 0.0f;
					fSpeedY = 0.0f;
				}
				else
				{
					fX = fNewX;
					fSpeedY = 0.0f;
				}
			}
			else
			{
				fSpeedX = 0.0f;
				fY = fNewY;
			}
		}
		else
		{
			fX = fNewX;
			fY = fNewY;
		}

	}
	else
	{
		fX = fNewX;
		fY = fNewY;
	}

	if (fY >= 594.0f - fHeight / 2 && bAffectedByGravity) //If below ground
	{
		if (fSpeedY - fGravity * deltatime < 5.0f) //Set speed to 0 if it is already very low
			fSpeedY = 0.0f;

		fY = 594.0f - fHeight / 2;
		fSpeedY = -(fSpeedY) * 0.6f; //bounce

		if (fSpeedX > 0.0f)
		{
			fSpeedX -= fFriction * deltatime;
			if (fSpeedX < 0.0f) fSpeedX = 0.0f;
		}
		if (fSpeedX < 0.0f)
		{
			fSpeedX += fFriction * deltatime;
			if (fSpeedX > 0.0f) fSpeedX = 0.0f;
		}
	}
	if (fY >= 594.0f - fHeight / 2 && !bAffectedByGravity)
	{
		fY = (594.0f - fHeight / 2) - (fY - (594.0f - fHeight / 2));
		fSpeedY = -fSpeedY;
	}

	if (fX < 0.0f || fX > 5120.0f)
	{ 
		return false; 
	}

	if (fSpeedX < 5.0f && fSpeedX > -5.0f) fSpeedX = 0.0f; //If speed is low set to 0

	return true;
}

void Entity::ChangeHealth(float fChange)
{
	sgGame->vBackgroundObjects.push_back(new EntityHealthChangeText(this, fChange));
	fHealth += fChange;
	if (fHealth <= 0.0f)
		fHealth = 0.0f; //When Destroy() is called on the player, it won't get deleted straight away
}

bool Entity::Overlapping(Entity* e)
{
	return (fX - fWidth / 2 < e->fX + e->fWidth / 2 &&
		fX + fWidth / 2 > e->fX - e->fWidth / 2 &&
		fY - fHeight / 2 < e->fY + e->fHeight / 2 &&
		fY + fHeight / 2> e->fY - e->fHeight / 2);
}

bool Entity::WillOverlap(Entity* e, float fNewX, float fNewY)
{
	return (fX - fWidth / 2 < fNewX + e->fWidth / 2 &&
		fX + fWidth / 2 > fNewX - e->fWidth / 2 &&
		fY - fHeight / 2 < fNewY + e->fHeight / 2 &&
		fY + fHeight / 2 > fNewY - e->fHeight / 2);
}

float Entity::Distance(Entity* entity)
{
	return distance(fX, fY, entity->fX, entity->fY);
}