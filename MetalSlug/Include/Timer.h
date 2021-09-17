#pragma once

#include "GameInfo.h"

class CTimer
{
public:
	CTimer();
	~CTimer();

private:
	LARGE_INTEGER m_Time;
	LARGE_INTEGER m_Second;

	float		m_DeltaTime;

	float m_FPS;
	// (60프레임 기준) 걸린 시간
	float m_FPSTime;
	// CTimer::Update호출 될 때 마다 1씩 증가 -> 프레임수 의미
	int m_Tick;

public:
	float GetDeltaTime()	const
	{
		return m_DeltaTime;
	}

	float GetFPS()	const
	{
		return m_FPS;
	}

public:
	float Update();
};

