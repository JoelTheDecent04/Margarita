#pragma once
#include <Windows.h>
#include <d2d1.h>

class Bar //TODO add horizontal / vertical
{
public:
	RECT rect;
	D2D1::ColorF& clrColour;
	float* fValue;
	float* fMaxValue;
	Bar(int left, int top, int right, int bottom, D2D1::ColorF& clrColour, float* fValue, float* fMaxValue);
	void Draw();
};

class Button
{
public:
	RECT rect;
	void (*function)(void*);
	const wchar_t* text;
	float fSize;
	bool bClickable;
	Button(int left, int top, int right, int bottom, void (*function)(void*), const wchar_t* text, float fSize = 24.0f);
	void Click(void* p);
	void Draw(bool bHover);
};