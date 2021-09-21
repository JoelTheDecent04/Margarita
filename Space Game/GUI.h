#pragma once
#include "Utilities.h"
#include "Graphics.h"
#include "Texture.h"
#include <SDL.h>
#include <SDL_ttf.h>

#include <vector>

class Bar //TODO add horizontal / vertical
{
public:
	Rect rect;
	SDL_Color clrColour;
	float* fValue;
	float* fMaxValue;
	Bar(int left, int top, int right, int bottom, SDL_Color clrColour, float* fValue, float* fMaxValue);
	void Draw();
};

class Button
{
public:
	Rect rect;
	void (*function)(void*);
	const char* text;
	TTF_Font** font;
	bool bClickable;
	Button(int left, int top, int right, int bottom, void (*function)(void*), const char* text, TTF_Font** font = &Graphics::pFont24Relative);
	void Click(void* p);
	void Draw(bool bHover);
};

class Widget
{
public:
	Rect region;
	virtual void ClickOn(int x, int y, void*) {}; //Relative coordinates
	virtual void ClickOff(int x, int y, void*) {};
	virtual void OnMouseEnter() {};
	virtual void OnMouseLeave() {};
	virtual void Update(float deltatime) {};
	virtual void TextInput(const char* text) { };
	virtual void KeyDown(int key) { };
	virtual void Draw(int x, int y) {}; //Real screen coordinates
};

class WidgetGroup : public Widget
{
	int nCurrentHover;
public:
	std::vector<std::shared_ptr<Widget>> children;
	WidgetGroup(const Rect& r);
	void ClickOn(int x, int y, void*) override;
	void ClickOff(int x, int y, void*) override;
	void OnMouseEnter() override;
	void OnMouseLeave() override;
	void Update(float deltatime) override;
	void TextInput(const char* text) override;
	void KeyDown(int key) override;
	void Draw(int x, int y) override;
};

class WidgetBitmapButton : public Widget
{
	bool hovering;
public:
	void (*function)(void*);
	Texture* tTexture;
	int nFrame;
	WidgetBitmapButton(const Rect& r, void (*function)(void*), Texture* tTexture, int nFrame);
	void Draw(int x, int y) override;
	void ClickOn(int x, int y, void* arg) override;
	void OnMouseEnter() override;
	void OnMouseLeave() override;
};

class WidgetButton : public Widget
{
	bool hovering;
	const char* text;
	TTF_Font** ppFont;
public:
	void (*function)(void*);
	WidgetButton(const Rect& r, void (*function)(void*), const char* text, TTF_Font** ppFont = &Graphics::pFont24Relative);
	void Draw(int x, int y) override;
	void ClickOn(int x, int y, void* arg) override;
	void OnMouseEnter() override;
	void OnMouseLeave() override;
};

enum class TextAlign
{
	Left, Top, Right, Bottom, Centered
};

class WidgetLabel : public Widget
{
	const char* text;
	TTF_Font** ppFont;
	TextAlign align;
public:
	WidgetLabel(const Rect& r, const char* text, TextAlign align = TextAlign::Centered, TTF_Font** ppFont = &Graphics::pFont24Relative);
	void SetText(const char* text) { this->text = text; };
	void Draw(int x, int y) override;
};

class WidgetTextbox : public Widget
{
	std::string text;
	TTF_Font** ppFont;
	TextAlign align;
	bool active;
public:
	enum
	{
		StartInactive = 0,
		StartActive = 1
	};
	WidgetTextbox(const Rect& r, TextAlign align = TextAlign::Centered, bool bStartActive = false, TTF_Font** ppFont = &Graphics::pFont24Relative);
	void ClickOn(int x, int y, void*) override;
	void TextInput(const char* text) override;
	void KeyDown(int key) override;
	void Draw(int x, int y) override;
	std::string& Text() { return text; }
};

class WidgetBitmap : public Widget
{
	Texture* tTexture;
	int nFrame;
public:
	WidgetBitmap(const Rect& r, Texture* tTexture, int nFrame);
	void Draw(int x, int y) override;
};