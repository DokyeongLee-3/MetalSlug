#pragma once

#include "GameInfo.h"
#include <list>

class CGameManager
{
private:
	CGameManager();
	~CGameManager();

public:
	bool Init(HINSTANCE hInst);
	int Run();

private:
	void Logic();
	void Input();
	bool Update(float DeltaTime);
	bool PostUpdate(float DeltaTime);
	bool Collision(float DeltaTime);
	void Render(float DeltaTime);

private:
	static bool m_Loop;
	HINSTANCE m_hInst;
	HWND m_hWnd;
	HDC m_hDC;
	HDC m_hBackDC;
	HBITMAP	m_hBackBmp;
	HBITMAP	m_hPrevBackBmp;
	Resolution m_RS;
	class CTimer* m_Timer;
	float m_TimeScale;
	HBRUSH m_hGreenBrush;
	HBRUSH m_hRedBrush;
	HPEN m_hGreenPen;
	HPEN m_hRedPen;
	int m_PlayerCredit;

public:
	int GetPlayerCredit()	const
	{
		return m_PlayerCredit;
	}

	void SetPlayerCredit(int Credit)
	{
		m_PlayerCredit = Credit;
	}

	Resolution GetResolution()	const
	{
		return m_RS;
	}

	HBRUSH GetGreenBrush()	const
	{
		return m_hGreenBrush;
	}

	HBRUSH GetRedBrush()	const
	{
		return m_hRedBrush;
	}

	HPEN GetGreenPen()	const
	{
		return m_hGreenPen;
	}

	HPEN GetRedPen()	const
	{
		return m_hRedPen;
	}

	HDC GetWindowDC()	const
	{
		return m_hDC;
	}

	HWND GetWindowHandle()	const
	{
		return m_hWnd;
	}

	HINSTANCE GetWindowInstance()	const
	{
		return m_hInst;
	}

public:
	void SetTimeScale(float TimeScale)
	{
		m_TimeScale = TimeScale;
	}

	float GetDeltaTime()	const;
	void Exit();

private:
	ATOM Register();
	BOOL Create();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	static CGameManager* m_Inst;

public:
	static CGameManager* GetInst()
	{
		if (!m_Inst)
		{
			m_Inst = new CGameManager;
		}
		return m_Inst;
	}

	static void DestoryInst()
	{
		SAFE_DELETE(m_Inst);
	}
};

