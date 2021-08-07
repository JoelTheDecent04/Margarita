#pragma once
#include "Texture.h"

class Animation
{
public:
	int nTexture;
	int nNumFrames;
	int* pnFrame;
	float fSecondsUntilNextFrame;
	float fFrameRate;
	bool bAnimationRunning;
	Animation(int nTexture, int nFrames, float fFrameRate, int* pnFrame);
	void Update(float deltatime);
	void Start() { bAnimationRunning = true; };
	void Stop() { bAnimationRunning = false; };
};