#include "Comet.h"
#include "Space.h"
#include "Utilities.h"

Comet::Comet()
	: animation(tCometTexture, 42, 60, &nFrame)
{
	nFrame = 0;
	fX = fBackgroundPosition + randomf() * 2560;
	fY = -100.0f;
	fSpeedX = -1200.0f;
	fSpeedY = 600.0f;
}

void Comet::Draw()
{
	tCometTexture->Draw(nFrame, fX, fY);
}

bool Comet::Update(float deltatime)
{
	fX += fSpeedX * deltatime;
	fY += fSpeedY * deltatime;
	animation.Update(deltatime);
	return true;
}
