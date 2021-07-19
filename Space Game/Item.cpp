#include "Item.h"

Item::Item()
{ 
	nCount = 0; 
	tTexture = nullptr; 
	nType = Type::None; 
	strName = "Unknown";
}
nlohmann::json Item::Save()
{
	nlohmann::json j = { {"type", nType} };
	return j;
}
void Item::Load(nlohmann::json& j)
{
	nType = j["type"].get<Item::Type>();
}