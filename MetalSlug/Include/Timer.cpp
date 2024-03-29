#include "Timer.h"
#include "GameManager.h"

CTimer::CTimer()	:
	m_FPS(0.f), 
	m_FPSTime(0.f), 
	m_Tick(0)
{
	// 초당 몇번의 틱이 지나는지
	QueryPerformanceFrequency(&m_Second);
	// 이 함수가 호출됐을 당시 몇번의 틱이 지나갔는지
	QueryPerformanceCounter(&m_Time);
}

CTimer::~CTimer()
{
}

float CTimer::Update()
{
	LARGE_INTEGER Time;

	QueryPerformanceCounter(&Time);

	m_DeltaTime = (Time.QuadPart - m_Time.QuadPart) / (float)m_Second.QuadPart;

	m_Time = Time;

	m_FPSTime += m_DeltaTime;
	++m_Tick;

	if (m_Tick == 60)
	{
		// 60프레임이 됐을 때 얼마나 시간이 지나갔느냐
		// 즉, 1초에 몇 프레임이 지나가느냐
		m_FPS = m_Tick / m_FPSTime;
		m_Tick = 0;
		m_FPSTime = 0.f;
	}


#ifdef _DEBUG

	char strFPS[64] = {};
	// 문자열을 만들어주는 함수
	sprintf_s(strFPS, "FPS : %.f\n", m_FPS);
	// AllocConsole로 열어준 콘솔창에 출력해주는 함수
	// <conio.h>에 포함되어 있음
	//_cprintf(strFPS);
	// 윈도우 타이틀바에 출력해주는 함수
	//SetWindowTextA(CGameManager::GetInst()->GetWindowHandle(), strFPS);
	// visual studio 출력창에 출력해주는 함수
	OutputDebugStringA(strFPS);
#endif // _DEBUG

	return m_DeltaTime;
}
