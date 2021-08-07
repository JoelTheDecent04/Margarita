#pragma once
#include <stdint.h>
#include <string>
#include <nlohmann/json.hpp>
#include <enet/enet.h>

std::string ip_representation(uint32_t ip);

float distance(float ax, float ay, float bx, float by);
float randomf();

namespace TextureID
{
	enum TextureID
	{
		Character, Orb, Background, Laser, Laserbeam, Enemy, Bomb, Crab, Comet, None, BombAnimation, EnergyPowerup, RegenerationPowerup, Light, Foreground
	};
};