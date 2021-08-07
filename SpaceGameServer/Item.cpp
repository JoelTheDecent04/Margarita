#include "Item.h"

Item::Item(std::shared_ptr<Player>& player)
{ 
	plPlayer = player;
	nCount = 0; 
	nType = Type::None; 
	strName = "Unknown";
}