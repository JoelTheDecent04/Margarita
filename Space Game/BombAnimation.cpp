#include "BombAnimation.h"

BombAnimation::BombAnimation(Bomb* bomb)
	: animation(nullptr, 21, 60, &nFrame)
{
	tTexture = tBombAnimationTexture;
	tTexture->fTextureDrawnWidth = 1.5f * 1.778f * (75.0f + 25.0f * bomb->nLevel);
	tTexture->fTextureDrawnHeight = 1.5f * (75.0f + 25.0f * bomb->nLevel);
	animation.tTexture = tBombAnimationTexture;
	fX = bomb->fX - tTexture->fTextureDrawnWidth / 2;
	fY = bomb->fY - tTexture->fTextureDrawnHeight / 2;
	nFrame = 0;
}

void BombAnimation::Draw()
{
	tBombAnimationTexture->Draw(nFrame, fX - fBackgroundPosition, fY);
}

bool BombAnimation::Update(float deltatime)
{
	animation.Update(deltatime);
	if (nFrame == animation.nNumFrames - 1) 
		return false;

	return true;
}
