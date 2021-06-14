#pragma once
#include <stdint.h>
#include <memory>

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

class SpaceGame;

namespace Game {
	void GameMain(void*);
	void LoadLevel(Level* lNewLevel, bool bUnloadPrevious = true, bool bLoadNext = true);
	void LeftClick();
	void KeyDown(int key);
	void Resize();
	void Quit();

	
	extern SpaceGame* sgSpaceGame;
}



extern int32_t nScreenWidth, nScreenHeight;
extern int32_t nRenderTargetWidth, nRenderTargetHeight;
extern float fScaleH, fScaleV;

