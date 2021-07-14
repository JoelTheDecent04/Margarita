#include "DeathScreen.h"
#include "Game.h"
#include "Space.h"
#include "Colours.h"
#include <fstream>

using namespace Game;

void DeathScreen::Render()
{
	Game::sgSpaceGame->Render();

	Graphics::FillRectangle(0.0f, 0.0f,nScreenWidth, nScreenHeight, clrBlack, 0.7f);

	TextSize textsize;
	Graphics::TextMetrics("You Suck!", Graphics::pFont36Relative, textsize);
	Graphics::WriteText("You Suck!", fScaleH * 640 - (textsize.width / 2), 100, Graphics::pFont36Relative);

	for (int i = 0; i < vButtons.size(); i++)
		vButtons[i].Draw(i == nButtonHover);
}

void DeathScreen::Update(float deltatime)
{
	int nCursorX, nCursorY;
	GetRelativeMousePos(&nCursorX, &nCursorY);

	bool bMouseOverButton = false;
	int i = 0;
	for (Button& button : vButtons)
	{
		if (PointInRect(button.rect, nCursorX, nCursorY))
		{
			nButtonHover = i;
			bMouseOverButton = true;
			break;
		}
		i++;
	}
	if (!bMouseOverButton) nButtonHover = -1;
}

void DeathScreen::LeftClick()
{
	if (nButtonHover != -1)
		vButtons[nButtonHover].function(this);
}

DeathScreen::DeathScreen()
{
	nButtonHover = -1;
	vButtons.push_back(Button(500, 200, 780, 300, [](void*) { LoadLevel(std::make_shared<SpaceGame>()); }, "New Game"));
	vButtons.push_back(Button(500, 320, 780, 420, [](void*) { Quit(); }, "Quit"));
}

DeathScreen::~DeathScreen()
{
	
}