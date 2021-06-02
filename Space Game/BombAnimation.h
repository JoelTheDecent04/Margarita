#pragma once
#include "BackgroundObject.h"
#include "Texture.h"
#include "Bomb.h"
#include "Animation.h"

class BombAnimation : public BackgroundObject
{
	float fX, fY;
	int nFrame;
	Animation animation;
public:
	Texture* tTexture;
	BombAnimation(Bomb* bomb);
	void Draw() override;
	bool Update(double deltatime) override;
};