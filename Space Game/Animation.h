#pragma once
#include "Texture.h"

class Animation
{
public:
	Texture* tTexture;
	int nNumFrames;
	int* pnFrame;
	float fSecondsUntilNextFrame;
	float fFrameRate;
	bool bAnimationRunning;
	Animation(Texture* tTexture, int nFrames, float fFrameRate, int* pnFrame);
	void Update(double deltatime);
	void Start() { bAnimationRunning = true; };
	void Stop() { bAnimationRunning = false; };
};