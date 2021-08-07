#include "BombAnimation.h"

BombAnimation::BombAnimation(Bomb* bomb)
	: animation(TextureID::None, 21, 60, &nFrame)
{
	nTexture = TextureID::BombAnimation;
	textures[nTexture]->fTextureDrawnWidth = 1.5f * 1.778f * (75.0f + 25.0f * bomb->nLevel);
	textures[nTexture]->fTextureDrawnHeight = 1.5f * (75.0f + 25.0f * bomb->nLevel);
	animation.nTexture = TextureID::BombAnimation;
	fX = bomb->fX - textures[nTexture]->fTextureDrawnWidth / 2;
	fY = bomb->fY - textures[nTexture]->fTextureDrawnHeight / 2;
	nFrame = 0;
}

void BombAnimation::Draw()
{
	textures[TextureID::BombAnimation]->Draw(nFrame, fX - fBackgroundPosition, fY);
}

bool BombAnimation::Update(float deltatime)
{
	animation.Update(deltatime);
	if (nFrame == animation.nNumFrames - 1) 
		return false;

	return true;
}
