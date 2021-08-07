#pragma once

struct Rect
{
	int left, top, right, bottom;
};

float distance(float ax, float ay, float bx, float by);
float randomf();
void GetRelativeMousePos(int* x, int* y);
void DebugOut(const char* fmt, ...);
bool PointInRect(Rect& rect, int x, int y);
bool GetKeyState(int key);

#define ErrorAndQuit(n, ...) { printf(n, ##__VA_ARGS__); exit(); }

#define SAFE_DELETE(n) if (n) { delete n; n = nullptr; }