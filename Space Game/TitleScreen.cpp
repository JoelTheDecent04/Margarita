#include "TitleScreen.h"
#include "Game.h"
#include "Space.h"
#include "Utilities.h"
#include "Colours.h"

static Texture* tButtons;
static Texture* tTitlescreenBackground;

TitleScreen::TitleScreen()
{
	nButtonHover = -1;
	tButtons = new Texture("buttons.png", 764, 343, 280, 125);

	vButtons.push_back(Button(500, 200, 780, 325, [](void*) {
		if (Game::sgSpaceGame.get()) //TODO check if Game::sgSpaceGame is sufficient
			Game::LoadLevel(Game::sgSpaceGame);
		else
		{
			auto pSpaceGame = std::make_shared<SpaceGame>();
			Game::sgSpaceGame = pSpaceGame;
			pSpaceGame->LoadFromFile();
			Game::LoadLevel(pSpaceGame);
			
		}
	}, "Play"));
	vButtons.push_back(Button(500, 500, 780, 625, [](void*) { Game::Quit(); }, "Quit"));
	vButtons.push_back(Button(500, 350, 780, 475, [](void*) {  }, "Info"));

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

	int nIndex = 0;
	for (Button& button : vButtons)
	{
		tButtons->Draw(nIndex, button.rect.left, button.rect.top);
		if (nButtonHover == nIndex)
		{
			Graphics::FillRectangle(fScaleH * button.rect.left, fScaleV * button.rect.top, fScaleH * (button.rect.right - button.rect.left), fScaleV * (button.rect.bottom - button.rect.top), clrBlack, 0.35f);
		}
		nIndex++;
	}
}

void TitleScreen::Update(float deltatime)
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

void TitleScreen::LeftClick()
{
	if (nButtonHover != -1)
		vButtons[nButtonHover].function(this);
}
