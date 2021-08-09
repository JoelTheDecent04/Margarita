#include "TitleScreen.h"
#include "Game.h"
#include "Space.h"
#include "Utilities.h"
#include "Colours.h"
#include "SpaceOnline.h"
#include "JoinServerScreen.h"

static Texture* tButtons;
static Texture* tTitlescreenBackground;

using namespace Game;

TitleScreen::TitleScreen()
	: gui(Rect(0, 0, 1280, 720))
{
	tButtons = new Texture("buttons.png", 764, 343, 280, 125);

	auto play_button_function = [](void*) {
		if (sgSpaceGame.get())
			LoadLevel(sgSpaceGame);
		else
		{
			auto pSpaceGame = std::make_shared<SpaceGame>();
			sgSpaceGame = pSpaceGame;
			LoadLevel(pSpaceGame);
		}
	};

	gui.children.push_back(std::make_shared<WidgetBitmapButton>(Rect(500, 200, 780, 325), play_button_function, tButtons, 0));
	gui.children.push_back(std::make_shared<WidgetBitmapButton>(Rect(500, 500, 780, 625), [](void*) {Quit(); }, tButtons, 1));
	gui.children.push_back(std::make_shared<WidgetBitmapButton>(Rect(500, 350, 780, 475), [](void*) { LoadLevel(std::make_shared<JoinServerScreen>()); /*std::make_shared<SpaceOnline>("localhost")*/ }, tButtons, 2));

	tTitlescreenBackground = new Texture("titlescreen_background.png", 3022, 1700, 1280.0f, 720.0f);
}

TitleScreen::~TitleScreen()
{
}

void TitleScreen::Render()
{
	tTitlescreenBackground->Draw(0, 0.0f, 0.0f);

	TextSize textsize;
	Graphics::TextMetrics("Margarita", Graphics::pFont125Relative, textsize); //TODO change font to chiller
	Graphics::WriteText("Margarita", fScaleH * 640 - textsize.width / 2, fScaleV * 15, Graphics::pFont125Relative, clrRed, 1.0f);

	gui.Draw(0, 0);
}

void TitleScreen::Update(float deltatime)
{
	gui.Update(deltatime);
}

void TitleScreen::LeftClick()
{
	int nCursorX, nCursorY;
	GetRelativeMousePos(&nCursorX, &nCursorY);
	gui.ClickOn(nCursorX, nCursorY, this);
}
