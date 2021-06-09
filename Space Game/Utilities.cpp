#include "Utilities.h"
#include <random>
#include <ctime>
#include <varargs.h>
#include <Windows.h>
extern float fBackgroundPosition;

float distance(float ax, float ay, float bx, float by)
{
	float x = bx - ax;
	float y = by - ay;
	return sqrt(x * x + y * y);
}

float random()
{
	static std::mt19937 r = std::mt19937(time(0));
	return (float)r() / r._Max;
}

float random_off_screen()
{
	float fRandomLocation = random() * (5120 - 1280);
	if (fRandomLocation > fBackgroundPosition) fRandomLocation += 1280;
	return fRandomLocation;
}

void DebugOut(const wchar_t* fmt, ...) {
	wchar_t s[1025];
	va_list args;
	va_start(args, fmt);
	wvsprintf(s, fmt, args);
	va_end(args);
	OutputDebugString(s);
}