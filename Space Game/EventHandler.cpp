#include "EventHandler.h"
#include "Utilities.h"
#include "Entity.h"

EventHandler::EventHandler(SpaceGame* game)
{
	this->game = game;
}

void EventHandler::Event(Type nType, int nCount, Entity* a, Entity* b)
{
	switch (nType)
	{
	case Type::None: break;
	case Type::Damage:
		DebugOut(L"Damaged %d enemies with weapon %d\n", nCount, a->nType);
		break;
	case Type::Hit:
		DebugOut(L"Hit entity %d with weapon %d\n", b->nType, a->nType);
		break;
	case Type::Kill:
		DebugOut(L"Killed entity %d with weapon %d\n", b->nType, a->nType);
		break;
	default:
		abort();
	}
}