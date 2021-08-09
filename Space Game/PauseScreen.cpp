#include "PauseScreen.h"
#include "Game.h"
#include "Space.h"
#include "TitleScreen.h"
#include "ControlsScreen.h"
#include "Utilities.h"
#include "Colours.h"

using namespace Game;

PauseScreen::PauseScreen()
	:gui (Rect(0, 0, 1280, 720))
{
	lPrevLevel = lCurrentLevel;

	gui.children.push_back(std::make_shared<WidgetButton>(Rect(500, 150, 780, 250), [](void* p) { LoadLevel(((PauseScreen*)p)->lPrevLevel); }, "Resume"));
	gui.children.push_back(std::make_shared<WidgetButton>(Rect(500, 275, 780, 375), [](void* p) { LoadLevel(std::make_shared<ControlsScreen>()); }, "Controls"));
	gui.children.push_back(std::make_shared<WidgetButton>(Rect(500, 400, 780, 500), [](void* p) { LoadLevel(std::static_pointer_cast<Level>(std::make_shared<TitleScreen>())); }, "Title Screen"));
	gui.children.push_back(std::make_shared<WidgetButton>(Rect(500, 525, 780, 625), [](void* p) { Quit(); }, "Quit"));
}

void PauseScreen::Render()
{

	if (lPrevLevel)
		lPrevLevel->Render();
	Graphics::FillRectangle(0.0f, 0.0f, fScaleH * 1280.0f, fScaleV * 720.0f, clrBlack, 0.7f);

	TextSize textsize;
	Graphics::TextMetrics("Game Paused", Graphics::pFont44Relative, textsize);
	Graphics::WriteText("Game Paused", fScaleH * 640 - textsize.width / 2, fScaleV * 70, Graphics::pFont44Relative);

	gui.Draw(0, 0);
}

void PauseScreen::Update(float deltatime)
{
	gui.Update(deltatime);
}

void PauseScreen::LeftClick()
{
	int nCursorX, nCursorY;
	GetRelativeMousePos(&nCursorX, &nCursorY);
	gui.ClickOn(nCursorX, nCursorY, this);
}

void PauseScreen::KeyDown(int key)
{
	if (key == SDL_SCANCODE_ESCAPE)
	{
		LoadLevel(lPrevLevel);
	}
}
