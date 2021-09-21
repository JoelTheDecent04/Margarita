#include "BackgroundObject.h"

int BackgroundObject::nObjects = 0;
std::vector<BackgroundObject> BackgroundObject::vObjects;

flatbuffers::Offset<ServerObject> BackgroundObject::Serialise(flatbuffers::FlatBufferBuilder& packet)
{
	return CreateServerObject(packet, nType, nData, fX, fY);
}