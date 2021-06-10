#include "Game.h"
#include "Graphics.h"
#include "TitleScreen.h"
#include "Space.h"
#include <stdint.h>
#include <string.h>
#include <queue>
#include <mutex>

int32_t nScreenWidth = 1600;
int32_t nScreenHeight = 900;

int32_t nRenderTargetWidth;
int32_t nRenderTargetHeight;

float fScaleH = (float)nScreenWidth / 1280;
float fScaleV = (float)nScreenHeight / 720;

namespace Game {
	Level* lCurrentLevel = nullptr;
	SpaceGame* sgSpaceGame = nullptr;
	static bool loading = false;
	static bool resize = false;

	bool bHighDetail = true;

	int nClicks;
	std::queue<int> qKeys;
	std::mutex m_qKeys;

	void GameMain()
	{
		CoInitializeEx(nullptr, COINIT_MULTITHREADED);

		LoadLevel(new TitleScreen());

		uint64_t nPerformanceFrequency;
		QueryPerformanceFrequency((LARGE_INTEGER*)&nPerformanceFrequency);
		double dSecondsPerCount = 1.0 / (double)nPerformanceFrequency;

		uint64_t nStartTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&nStartTime);

		while (1)
		{
			if (!loading)
			{
				while (nClicks > 0)
				{
					lCurrentLevel->LeftClick();
					nClicks--;
				}
				m_qKeys.lock();
				while (!qKeys.empty())
				{
					int key = qKeys.front();
					qKeys.pop();
					lCurrentLevel->KeyDown(key);
				}
				m_qKeys.unlock();
				if (resize)
				{
					resize = false;
					Graphics::Resize();
				}
				
				uint64_t nCurrentTime;
				QueryPerformanceCounter((LARGE_INTEGER*)&nCurrentTime);
				double dDeltaTime = (nCurrentTime - nStartTime) * dSecondsPerCount;
				nStartTime = nCurrentTime;

				lCurrentLevel->Update(dDeltaTime);

				Graphics::BeginDraw();
				lCurrentLevel->Render();
				Graphics::EndDraw();
			}
		}
	}

	void LoadLevel(Level* lNewLevel, bool bUnloadPrevious, bool bLoadNext)
	{
		if (lCurrentLevel) //Render last frame of previous level
			lCurrentLevel->Render();

		loading = true;
		wchar_t txtWindowTitle[100];
		wchar_t txtNewWindowTitle[128];
		
		GetWindowText(Graphics::hWindow, txtWindowTitle, 100);
		wcscpy_s(txtNewWindowTitle, 128, txtWindowTitle);
		wcscat_s(txtNewWindowTitle, 128, L" - Loading");
		SetWindowText(Graphics::hWindow, txtNewWindowTitle);
		SetCursor(LoadCursor(nullptr, IDC_WAIT));

		if (lCurrentLevel && bUnloadPrevious)
		{
			Level* lLastLevel = lCurrentLevel;
			lCurrentLevel = nullptr;
			lLastLevel->Unload();
		}
		
		
		if (bLoadNext) lNewLevel->Load();
		lCurrentLevel = lNewLevel;

		loading = false;
		SetCursor(LoadCursor(nullptr, IDC_ARROW));
		SetWindowText(Graphics::hWindow, txtWindowTitle);
	}

	void LeftClick()
	{
		nClicks++;
	}

	void KeyDown(int key)
	{
		m_qKeys.lock();
		qKeys.push(key);
		m_qKeys.unlock();
	}

	void Resize()
	{
		resize = true;
	}
	void Quit()
	{
		if (sgSpaceGame)
			sgSpaceGame->Save();
		ExitProcess(0);
	}
}