#include "Utilities.h"
#include <random>
#include <ctime>
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
	return (float)r() / r.max();
}

float random_off_screen()
{
	float fRandomLocation = random() * (5120 - 1280);
	if (fRandomLocation > fBackgroundPosition) fRandomLocation += 1280;
	return fRandomLocation;
}