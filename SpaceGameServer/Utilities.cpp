#include "Utilities.h"
#include <random>
#include <ctime>
#include <stdarg.h>
extern float fBackgroundPosition;

float distance(float ax, float ay, float bx, float by)
{
	float x = bx - ax;
	float y = by - ay;
	return sqrt(x * x + y * y);
}

float randomf()
{
	static std::mt19937 r = std::mt19937(time(0));
	return (float)r() / r.max();
}


bool PointInRect(Rect& rect, int x, int y) //TODO make SDL_PointInRect
{
	return (x > rect.left && y > rect.top && x < rect.right && y < rect.bottom);
}

void DebugOut(const char* fmt, ...) {
	/*char s[1025];
	va_list args;
	va_start(args, fmt);
	wvsnprintf(s, fmt, args);
	va_end(args);
	OutputDebugString(s);*/
}