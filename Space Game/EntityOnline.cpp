#include "EntityOnline.h"
#include "Space.h"

void OnlineEntity::Draw()
{
	textures[nTextureID]->Draw(nFrame, (fX - fBackgroundPosition - (textures[nTextureID]->fTextureDrawnWidth / 2)), fY - textures[nTextureID]->fTextureDrawnHeight / 2);
}