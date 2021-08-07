#include "ServerUtilities.h"
#include <sstream>
#include <random>

std::string ip_representation(uint32_t ip)
{
	std::stringstream ss;
	
	ss << (ip & 0xff) << '.' << ((ip & 0xff00) >> 8) << '.' << ((ip & 0xff0000) >> 16) << '.' << ((ip & 0xff000000) >> 24);
	
	return ss.str();
}


float distance(float ax, float ay, float bx, float by)
{
	float x = bx - ax;
	float y = by - ay;
	return sqrt(x * x + y * y);
}

float randomf()
{
	static std::mt19937 r = std::mt19937(time(0));
	return (float)r() / r._Max;
}