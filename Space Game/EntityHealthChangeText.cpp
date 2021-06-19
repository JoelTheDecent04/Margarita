#include "EntityHealthChangeText.h"
#include "Colours.h"
#include <stdio.h>

EntityHealthChangeText::EntityHealthChangeText(Entity* entity, int nChange)
	: TextObject()
{
	bool healing = nChange > 0 ? true : false;
	wchar_t strTextBuffer[24];
	swprintf_s(strTextBuffer, 24, healing ? L"+%d" : L"%d", nChange);
	TextSize textsize;
	Graphics::TextMetrics(strTextBuffer, 12.0f, textsize);

	fOpacity = 1.0f;
	fOpacityChangePerSecond = -4.0f;
	fX = entity->fX - (textsize.width / 2);
	fY = entity->fY - (entity->tTexture->fTextureDrawnHeight / 2) - 4 - 12;
	fSpeedX = 0.0f;
	fSpeedY = -100.0f;
	fSize = 12.0f;
	clrColour = healing ? &clrGreen : &clrRed;
	strText = std::wstring(strTextBuffer);
}