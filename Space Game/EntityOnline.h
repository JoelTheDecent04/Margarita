#pragma once

#ifdef SUPPORT_MULTIPLAYER
class OnlineEntity
{
public:
	float fX, fY;
	int nTextureID;
	int nFrame;
	void Draw();
};
#endif