#include "EntityHealthChangeText.h"
#include "Colours.h"
#include "Space.h"
#include <stdio.h>

EntityHealthChangeText::EntityHealthChangeText(Entity* entity, int nChange)
	: TextObject()
{
	bool healing = nChange > 0 ? true : false;
	char strTextBuffer[64];
	snprintf(strTextBuffer, sizeof(strTextBuffer), healing ? "+%d" : "%d", nChange);
	TextSize textsize;
	Graphics::TextMetrics(strTextBuffer, Graphics::pFont12, textsize);

	fOpacity = 1.0f;
	fOpacityChangePerSecond = -4.0f;
	fX = entity->fX - (textsize.width / 2);
	fY = entity->fY - (textures[entity->nTexture]->fTextureDrawnHeight / 2) - 4 - 12;
	fSpeedX = 0.0f;
	fSpeedY = -100.0f;
	ppFont = &Graphics::pFont14Relative;
	this->clrColour = healing ? &clrGreen : &clrRed;
	strText = std::string(strTextBuffer);
}