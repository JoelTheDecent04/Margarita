#include "EntityHealthChangeText.h"
#include <stdio.h>

static D2D1::ColorF clrRed = { 1.0f, 0.0f, 0.0f };
static D2D1::ColorF clrGreen = { 0.0f, 1.0f, 0.0f };

EntityHealthChangeText::EntityHealthChangeText(Entity* entity, int nChange)
	: TextObject()
{
	bool healing = nChange > 0 ? true : false;
	wchar_t strTextBuffer[24];
	swprintf_s(strTextBuffer, 24, healing ? L"+%d" : L"%d", nChange);
	DWRITE_TEXT_METRICS tmTextMetrics;
	Graphics::TextMetrics(strTextBuffer, 12.0f, tmTextMetrics);

	fOpacity = 1.0f;
	fOpacityChangePerSecond = -4.0f;
	fX = entity->fX - (tmTextMetrics.width / 2);
	fY = entity->fY - (entity->tTexture->fTextureDrawnHeight / 2) - 4 - 12;
	fSpeedX = 0.0f;
	fSpeedY = -100.0f;
	fSize = 12.0f;
	clrColour = healing ? &clrGreen : &clrRed;
	strText = std::wstring(strTextBuffer);
}