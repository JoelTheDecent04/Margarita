#include "JoinServerScreen.h"
#include "Graphics.h"
#include "SpaceOnline.h"

JoinServerScreen::JoinServerScreen()
	: gui (Rect(0, 0, 1280, 720))
{
	gui.children.push_back(std::make_shared<WidgetLabel>(Rect(540, 50, 740, 100), "Connect to Server", TextAlign::Centered, &Graphics::pFont44Relative));
	gui.children.push_back(std::make_shared<WidgetLabel>(Rect(400, 125, 450, 175), "IP: "));
	gui.children.push_back(std::make_shared<WidgetButton>(Rect(590, 200, 690, 250), [](void* p) { ((JoinServerScreen*)p)->Connect(); }, "Connect"));

	//Input textbox
	ip_input_textbox = std::make_shared<WidgetTextbox>(Rect(450, 125, 600, 175), TextAlign::Left, WidgetTextbox::StartActive);
	gui.children.push_back(ip_input_textbox);
}

JoinServerScreen::~JoinServerScreen()
{

}

void JoinServerScreen::Render()
{
	Graphics::Clear(clrBlack);
	gui.Draw(0, 0);
}

void JoinServerScreen::Update(float deltatime)
{
	gui.Update(deltatime);
}

void JoinServerScreen::LeftClick()
{
	int nCursorX, nCursorY;
	GetRelativeMousePos(&nCursorX, &nCursorY);
	gui.ClickOn(nCursorX, nCursorY, this);
}

void JoinServerScreen::KeyDown(int key)
{
	gui.KeyDown(key);
}

void JoinServerScreen::KeyPress(const char* keys)
{
	gui.TextInput(keys);
}

void JoinServerScreen::Connect()
{
	Game::LoadLevel(std::make_shared<SpaceOnline>(ip_input_textbox->Text()));
}