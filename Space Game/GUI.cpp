#include "GUI.h"
#include "Graphics.h"
#include "Game.h"
#include "Colours.h"


Bar::Bar(int left, int top, int right, int bottom, SDL_Color clrColour, float* fValue, float* fMaxValue)
	: clrColour(clrColour)
{
	this->rect = { left, top, right, bottom }; this->fValue = fValue; this->fMaxValue = fMaxValue;
}

void Bar::Draw()
{
	Graphics::FillRectangle(fScaleH * rect.left, fScaleV * rect.top, fScaleH * ((*fValue / *fMaxValue) * (rect.right - rect.left)), fScaleV * (rect.bottom - rect.top), clrColour);
	Graphics::DrawRectangle(fScaleH * rect.left, fScaleV * rect.top, fScaleH * (rect.right - rect.left), fScaleV * (rect.bottom - rect.top), clrDarkGrey);
}

Button::Button(int left, int top, int right, int bottom, void (*function)(void*), const char* text, TTF_Font** font)
{
	this->rect = { left, top, right, bottom };
	this->function = function;
	this->text = text;
	this->font = font;
	bClickable = true;
};

void Button::Click(void* p)
{
	function(p);
}

void Button::Draw(bool bHover)
{
	Graphics::FillRectangle(fScaleH * rect.left, fScaleV * rect.top, fScaleH * (rect.right - rect.left), fScaleV * (rect.bottom - rect.top), bClickable ? (bHover ? clrDarkGrey : clrBlack) : clrDarkerGrey);
	Graphics::DrawRectangle(fScaleH * rect.left, fScaleV * rect.top, fScaleH * (rect.right - rect.left), fScaleV * (rect.bottom - rect.top), clrDarkGrey);
	TextSize textsize;
	Graphics::TextMetrics(text, *font, textsize);
	Graphics::WriteText(text,
		fScaleH * ((rect.right + rect.left) / 2) - textsize.width / 2,
		fScaleV * ((rect.bottom + rect.top) / 2) - textsize.height / 2,
		*font);
}


//New GUI

WidgetGroup::WidgetGroup(const Rect& r)
{
	region = r;
	nCurrentHover = -1;
}
void WidgetGroup::ClickOn(int x, int y, void* arg)
{
	for (int i = 0; i < children.size(); i++)
	{
		auto& child = children[i];
		if (PointInRect(child->region, x, y))
			child->ClickOn(x - child->region.left, y - child->region.top, arg);
		else
			child->ClickOff(x - child->region.left, y - child->region.top, arg);
	}
}
void WidgetGroup::ClickOff(int x, int y, void* arg)
{
	for (auto& child : children)
		child->ClickOff(x, y, arg);
}
void WidgetGroup::OnMouseEnter()
{
}
void WidgetGroup::OnMouseLeave()
{
}
void WidgetGroup::Update(float deltatime)
{
	int nCursorX, nCursorY;
	GetRelativeMousePos(&nCursorX, &nCursorY);
	
	//Check if still hovering over previous widget
	if (nCurrentHover >= 0 && PointInRect(children[nCurrentHover]->region, nCursorX, nCursorY) == false)
	{
		children[nCurrentHover]->OnMouseLeave();
		nCurrentHover = -1;
	}
	
	//Check if hovering over chidren
	if (nCurrentHover == -1)
	{
		for (int i = 0; i < children.size(); i++)
		{
			auto& child = children[i];
			if (PointInRect(child->region, nCursorX, nCursorY))
			{
				child->OnMouseEnter();
				nCurrentHover = i;
				break;
			}
		}
	}

	//Update children
	for (auto& child : children)
		child->Update(deltatime);
}
void WidgetGroup::TextInput(const char* text)
{
	for (auto& child : children)
		child->TextInput(text);
}
void WidgetGroup::KeyDown(int key)
{
	for (auto& child : children)
		child->KeyDown(key);
}
void WidgetGroup::Draw(int x, int y)
{
	for (auto& child : children)
		child->Draw(x + child->region.left, y + child->region.top);
}

WidgetBitmapButton::WidgetBitmapButton(const Rect& r, void(*function)(void*), Texture* tTexture, int nFrame)
{
	region = r;
	this->function = function;
	this->tTexture = tTexture;
	this->nFrame = nFrame;
	hovering = false;
}

void WidgetBitmapButton::Draw(int x, int y)
{
	tTexture->Draw(nFrame, x, y);
	if (hovering)
		Graphics::FillRectangle(fScaleH * region.left, fScaleV * region.top, fScaleH * (region.right - region.left), fScaleV * (region.bottom - region.top), clrBlack, 0.35f);
}
void WidgetBitmapButton::ClickOn(int x, int y, void* arg)
{
	function(arg);
}
void WidgetBitmapButton::OnMouseEnter()
{
	hovering = true;
}
void WidgetBitmapButton::OnMouseLeave()
{
	hovering = false;
}


WidgetButton::WidgetButton(const Rect& r, void (*function)(void*), const char* text, TTF_Font** ppFont)
{
	region = r;
	this->function = function;
	this->text = text;
	this->ppFont = ppFont;
	hovering = false;
}
void WidgetButton::Draw(int x, int y)
{
	Graphics::FillRectangle(fScaleH * region.left, fScaleV * region.top, fScaleH * (region.right - region.left), fScaleV * (region.bottom - region.top), hovering ? clrDarkGrey : clrBlack);
	Graphics::DrawRectangle(fScaleH * region.left, fScaleV * region.top, fScaleH * (region.right - region.left), fScaleV * (region.bottom - region.top), clrDarkGrey);

	TextSize textsize;
	Graphics::TextMetrics(text, *ppFont, textsize);
	Graphics::WriteText(text,
		fScaleH * ((region.right + region.left) / 2) - textsize.width / 2,
		fScaleV * ((region.bottom + region.top) / 2) - textsize.height / 2,
		*ppFont);
}
void WidgetButton::ClickOn(int x, int y, void* arg)
{
	function(arg);
}
void WidgetButton::OnMouseEnter()
{
	hovering = true;
}
void WidgetButton::OnMouseLeave()
{
	hovering = false;
}

WidgetLabel::WidgetLabel(const Rect& r, const char* text, TextAlign align, TTF_Font** ppFont)
{
	this->region = r;
	this->text = text;
	this->ppFont = ppFont;
	this->align = align;
}

void WidgetLabel::Draw(int x, int y)
{
	TextSize textsize;
	Graphics::TextMetrics(text, *ppFont, textsize);

	switch (align)
	{
	case TextAlign::Centered:
		Graphics::WriteText(text, fScaleH * ((region.right + region.left) / 2) - textsize.width / 2, fScaleV * ((region.bottom + region.top) / 2) - textsize.height / 2, *ppFont);
		break;
	case TextAlign::Left:
		Graphics::WriteText(text, fScaleH * region.left, fScaleV * ((region.bottom + region.top) / 2) - textsize.height / 2, *ppFont);
		break;
	case TextAlign::Right:
		Graphics::WriteText(text, fScaleH * region.right - textsize.width, fScaleV * ((region.bottom + region.top) / 2) - textsize.height / 2, *ppFont);
		break;
	case TextAlign::Bottom:
		Graphics::WriteText(text, fScaleH * ((region.right + region.left) / 2) - textsize.width / 2, fScaleV * region.bottom - textsize.height, *ppFont);
		break;
	case TextAlign::Top:
		Graphics::WriteText(text, fScaleH * ((region.right + region.left) / 2) - textsize.width / 2, fScaleV * region.top, *ppFont);
		break;
	}
	
}

WidgetTextbox::WidgetTextbox(const Rect& r, TextAlign align, bool bStartActive, TTF_Font** ppFont)
{
	region = r;
	active = bStartActive;
	this->ppFont = ppFont;
	this->align = align;
}

void WidgetTextbox::ClickOn(int x, int y, void*)
{
	active = true;
}

void WidgetTextbox::TextInput(const char* input)
{
	if (active)
		text.append(input);
}

void WidgetTextbox::KeyDown(int key)
{
	if (active && key == SDL_SCANCODE_BACKSPACE && text.size() > 0)
		text.pop_back();
}

void WidgetTextbox::Draw(int x, int y)
{
	const char* txt = text.c_str();

	TextSize textsize;
	Graphics::TextMetrics(txt, *ppFont, textsize);

	switch (align)
	{
	case TextAlign::Centered:
		Graphics::WriteText(txt, fScaleH * ((region.right + region.left) / 2) - textsize.width / 2, fScaleV * ((region.bottom + region.top) / 2) - textsize.height / 2, *ppFont);
		break;
	case TextAlign::Left:
		Graphics::WriteText(txt, fScaleH * region.left, fScaleV * ((region.bottom + region.top) / 2) - textsize.height / 2, *ppFont);
		break;
	case TextAlign::Right:
		Graphics::WriteText(txt, fScaleH * region.right - textsize.width, fScaleV * ((region.bottom + region.top) / 2) - textsize.height / 2, *ppFont);
		break;
	case TextAlign::Bottom:
		Graphics::WriteText(txt, fScaleH * ((region.right + region.left) / 2) - textsize.width / 2, fScaleV * region.bottom - textsize.height, *ppFont);
		break;
	case TextAlign::Top:
		Graphics::WriteText(txt, fScaleH * ((region.right + region.left) / 2) - textsize.width / 2, fScaleV * region.top, *ppFont);
		break;
	}
}

WidgetBitmap::WidgetBitmap(const Rect& r, Texture* tTexture, int nFrame)
{
	region = r;
	this->tTexture = tTexture;
	this->nFrame = nFrame;
}
void WidgetBitmap::Draw(int x, int y)
{
	tTexture->DrawDifferentSize(
		nFrame, region.left, region.top,
		region.right - region.left, region.bottom - region.top);
}