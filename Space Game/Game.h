#pragma once
#include <stdint.h>
#include <memory>

class Level
{
public:
	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Render() = 0;
	virtual void Update(float deltatime) = 0;
	virtual void LeftClick() {};
	virtual void KeyDown(int key) {};
};

class SpaceGame;

namespace Game {
	void GameMain();
	void LoadLevel(Level* lNewLevel, bool bUnloadPrevious = true, bool bLoadNext = true);
	void LeftClick();
	void KeyDown(int key);
	void Resize();
	void Quit();

	
	extern SpaceGame* sgSpaceGame;

	extern const uint8_t* pKeyStates;
	extern int pKeyStatesLength;
}



extern int32_t nScreenWidth, nScreenHeight;
extern int32_t nRenderTargetWidth, nRenderTargetHeight;
extern float fScaleH, fScaleV;

