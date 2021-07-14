#include "Utilities.h"
#include "Graphics.h"
#include "Game.h"
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

float random_off_screen()
{
	float frandomfLocation = randomf() * (5120 - 1280);
	if (frandomfLocation > fBackgroundPosition) frandomfLocation += 1280;
	return frandomfLocation;
}

void GetRelativeMousePos(int* x, int* y)
{
	int nCursorX, nCursorY;
	SDL_GetMouseState(&nCursorX, &nCursorY);
	*x = nCursorX /= fScaleH;
	*y = nCursorY /= fScaleV;
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

bool GetKeyState(int key)
{
	return (/*key < pKeyStatesLength&&*/ Game::pKeyStates[key]);
}