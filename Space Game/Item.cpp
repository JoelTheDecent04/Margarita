#include "Item.h"

Item::Item()
{ 
	nCount = 0; 
	tTexture = nullptr; 
	nType = Type::None; 
	strName = L"Unknown";
}
void Item::Save(std::fstream& f)
{
	f << (int)nType << " ";
}
void Item::Load(std::fstream& f)
{
}