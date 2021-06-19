#pragma once
#include <Windows.h>

struct Rect
{
	int left, top, right, bottom;
};

float distance(float ax, float ay, float bx, float by);
float random();
float random_off_screen();
void GetRelativeMousePos(int* x, int* y);
void DebugOut(const wchar_t* fmt, ...);
bool PointInRect(Rect& rect, int x, int y);