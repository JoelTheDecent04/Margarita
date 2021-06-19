#pragma once
#include <Windows.h>
float distance(float ax, float ay, float bx, float by);
float random();
float random_off_screen();
void GetRelativeMousePos(int* x, int* y);
void DebugOut(const wchar_t* fmt, ...);
bool PointInRect(RECT& rect, int x, int y);