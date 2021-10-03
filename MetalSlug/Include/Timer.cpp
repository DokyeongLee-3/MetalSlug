#include "Timer.h"
#include "GameManager.h"

CTimer::CTimer()	:
	m_FPS(0.f), 
	m_FPSTime(0.f), 
	m_Tick(0)
{
	// �ʴ� ����� ƽ�� ��������
	QueryPerformanceFrequency(&m_Second);
	// �� �Լ��� ȣ����� ��� ����� ƽ�� ����������
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
		// 60�������� ���� �� �󸶳� �ð��� ����������
		// ��, 1�ʿ� �� �������� ����������
		m_FPS = m_Tick / m_FPSTime;
		m_Tick = 0;
		m_FPSTime = 0.f;
	}


#ifdef _DEBUG

	char strFPS[64] = {};
	// ���ڿ��� ������ִ� �Լ�
	sprintf_s(strFPS, "FPS : %.f\n", m_FPS);
	// AllocConsole�� ������ �ܼ�â�� ������ִ� �Լ�
	// <conio.h>�� ���ԵǾ� ����
	//_cprintf(strFPS);
	// ������ Ÿ��Ʋ�ٿ� ������ִ� �Լ�
	SetWindowTextA(CGameManager::GetInst()->GetWindowHandle(), strFPS);
	// visual studio ���â�� ������ִ� �Լ�
	OutputDebugStringA(strFPS);
#endif // _DEBUG

	return m_DeltaTime;
}
