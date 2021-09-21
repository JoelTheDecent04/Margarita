#pragma once
#include "../Margarita_generated.h"

class BackgroundObject
{
public:
	enum Type
	{
		None,
		EntityHealthChange
	};
	Type nType;
	short nData;
	float fX;
	float fY;
	flatbuffers::Offset<ServerObject> Serialise(flatbuffers::FlatBufferBuilder& packet);
	static int nObjects;
	static std::vector<BackgroundObject> vObjects;
};