#include "Entity.h"
#include "OnlineSpaceGame.h"
#include "ServerUtilities.h"

Entity::Entity(float fX, float fY, float fWidth, float fHeight)
{
	this->fX = fX;
	this->fY = fY;
	bAffectedByGravity = false;
	fHealth = -1.0f;
	fMaxHealth = 100.0f;
	this->fHeight = fHeight;
	this->fWidth = fWidth;
	bCanCollide = true;
	bCanCollideWithPlayer = false;
	nType = Type::None;
	fSpeedX = 0.0f;
	fSpeedY = 0.0f;

	nFrame = 0;
	nTexture = TextureID::None;
}

bool Entity::Update(float deltatime)
{
	if (fHealth == 0.0f)
	{
		Destroy(nullptr);
		return false;
	}

	if (bAffectedByGravity) fSpeedY += fGravity * deltatime; //Gravity force

	float fNewX = fX + fSpeedX * deltatime; //Get new position
	float fNewY = fY + fSpeedY * deltatime;

	if (bCanCollide) //If this entity can collide
	{
		bool bCollided = false;
		for (auto entity : sgSpaceGame->vEntities) //Check for collisions
		{
			if (entity.get() == this || !entity->bCanCollide) continue;
			if (entity->nType == Type::Player && !bCanCollideWithPlayer) continue;
			if (entity->WillOverlap(this, fNewX, fNewY))
			{
				bCollided = true;
				if (Collide(entity.get()) == false)
				{
					return false;
				}
				break;
			}
		}

		if (bCollided)
		{
			bool bCollidedVertically = false;
			for (auto& entity : sgSpaceGame->vEntities) //Check if it can move vertically
			{
				if (!entity) continue;
				if (entity.get() == this || !entity->bCanCollide) continue;
				if (entity->nType == Type::Player && !bCanCollideWithPlayer) continue;
				if (entity->WillOverlap(this, fX, fNewY))
				{
					bCollidedVertically = true;
					break;
				}
			}

			if (bCollidedVertically)
			{
				bool bCollidedHorizontally = false;
				for (auto& entity : sgSpaceGame->vEntities) //Check if it can move horizontally
				{
					if (!entity) continue;
					if (entity.get() == this || !entity->bCanCollide) continue;
					if (entity->nType == Type::Player && !bCanCollideWithPlayer) continue;
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

	if (fX < -200.0f || fX > 5320.0f || fY < -250.0f)
	{
		return false;
	}

	if (fSpeedX < 5.0f && fSpeedX > -5.0f) fSpeedX = 0.0f; //If speed is low set to 0

	return true;
}
bool Entity::Overlapping(Entity* e)
{
	return (fX - fWidth / 2 < e->fX + e->fWidth / 2 &&
		fX + fWidth / 2 > e->fX - e->fWidth / 2 &&
		fY - fHeight / 2 < e->fY + e->fHeight / 2 &&
		fY + fHeight / 2 > e->fY - e->fHeight / 2);
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
void Entity::ChangeHealth(float fChange, Entity* e)
{
	fHealth += fChange;
	if (fHealth <= 0.0f)
	{
		//sgSpaceGame->pEventHandler->Event(EventHandler::Type::Kill, 0, e, this);
	}
	else if (fChange < 0.0f)
	{
		//sgSpaceGame->pEventHandler->Event(EventHandler::Type::Hit, 0, e, this);
	}
}

std::shared_ptr<Player> Entity::NearestPlayer(float* nearest_dist)
{
	std::shared_ptr<Player> nearest_player;
	float distance = 10000.0f; //Large number

	for (auto& player : sgSpaceGame->vPlayers)
	{
		float dist;
		if ((dist = Distance( (Entity*)(player.get()) )) < distance)
		{
			nearest_player = player;
			distance = dist;
		}
	}

	if (nearest_dist)
		*nearest_dist = distance;
	return nearest_player;
}

flatbuffers::Offset<NetEntity> Entity::Serialise(flatbuffers::FlatBufferBuilder& packet)
{
	return CreateNetEntity(packet, fX - fWidth / 2.0f, fY - fHeight / 2.0f, nTexture, 0.0f, nFrame); //Client draws from top left corner
}

