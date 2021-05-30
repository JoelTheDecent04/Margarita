#pragma once
#include <stdint.h>

class Level
{
public:
	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Render() = 0;
	virtual void Update(double deltatime) = 0;
	virtual void LeftClick() {};
	virtual void KeyDown(int key) {};
};

namespace Game {
	void LoadLevel(Level* lNewLevel, bool bUnloadPrevious = true, bool bLoadNext = true);
	void Render();
	void Update(double dDeltaTime);
	void LeftClick();
	void KeyDown(int key);
}

extern int32_t nScreenWidth, nScreenHeight;
extern int32_t nRenderTargetWidth, nRenderTargetHeight;
extern float fScaleH, fScaleV;