#include "Game.h"
#include "Graphics.h"
#include <stdint.h>

int32_t nScreenWidth = 1600;
int32_t nScreenHeight = 900;

int32_t nRenderTargetWidth;
int32_t nRenderTargetHeight;

float fScaleH = (float)nScreenWidth / 1280;
float fScaleV = (float)nScreenHeight / 720;


namespace Game {
	Level* lCurrentLevel = nullptr;
	bool loading = false;
	HANDLE hRenderThread = 0;
	HANDLE hUpdateThread = 0;

	static void UpdateThread();
	static void RenderThread();

	void LoadLevel(Level* lNewLevel, bool bUnloadPrevious, bool bLoadNext)
	{
		if (lCurrentLevel) //Render last frame of previous level
			lCurrentLevel->Render();

		loading = true;
		if (lCurrentLevel && bUnloadPrevious)
		{
			Level* lLastLevel = lCurrentLevel;
			lCurrentLevel = nullptr;
			Sleep(50); //Give threads enough time to finish using level
			lLastLevel->Unload();
		}
		
		
		if (bLoadNext) lNewLevel->Load();
		lCurrentLevel = lNewLevel;

		if (!hUpdateThread)
		{
			hUpdateThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)UpdateThread, NULL, 0, NULL);
			assert(hUpdateThread);
		}
		if (!hRenderThread)
		{
			hRenderThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RenderThread, NULL, 0, NULL);
			assert(hRenderThread);
		}

		loading = false;
	}

	void Render()
	{
		if (!lCurrentLevel) return;
		Graphics::BeginDraw();
		lCurrentLevel->Render();
		DWRITE_TEXT_METRICS tmTextMetrics;
		Graphics::TextMetrics(L"Alpha 0.0.2", 16, tmTextMetrics);
		Graphics::WriteText(L"Alpha 0.0.2", 1280 - tmTextMetrics.width - 4, 4, 16);
		Graphics::EndDraw();
	}

	void Update(double dDeltaTime)
	{
		if (!lCurrentLevel) return;
		lCurrentLevel->Update(dDeltaTime);
	}

	void LeftClick()
	{
		if (!lCurrentLevel) return;
		lCurrentLevel->LeftClick();
	}

	void KeyDown(int key)
	{
		if (!lCurrentLevel) return;
		lCurrentLevel->KeyDown(key);
	}

	static void UpdateThread()
	{
		uint64_t nPerformanceFrequency;
		QueryPerformanceFrequency((LARGE_INTEGER*)&nPerformanceFrequency);
		double dSecondsPerCount = 1.0 / (double)nPerformanceFrequency;

		uint64_t nStartTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&nStartTime);

		while (1)
		{
			if (loading) continue;
			
			uint64_t nCurrentTime;
			QueryPerformanceCounter((LARGE_INTEGER*)&nCurrentTime);
			double dDeltaTime = (nCurrentTime - nStartTime) * dSecondsPerCount;

			nStartTime = nCurrentTime;

			lCurrentLevel->Update(dDeltaTime);
		}
	}

	static void RenderThread()
	{
		while (1)
		{
			if (loading) continue;
			if (nRenderTargetHeight != nScreenHeight || nRenderTargetWidth != nScreenWidth)
				Graphics::Resize();
			Graphics::BeginDraw();
			lCurrentLevel->Render();
			DWRITE_TEXT_METRICS tmTextMetrics;
			Graphics::EndDraw();
		}
	}
}