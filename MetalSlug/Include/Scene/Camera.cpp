#include "Camera.h"

CCamera::CCamera()	:
	m_Resolution(1024.f, 768.f),
	m_WorldResolution(WORLD_WIDTH, WORLD_HEIGHT),
	m_CamFixX(false),
	m_CamFixY(false)
{
}

CCamera::~CCamera()
{
}

bool CCamera::Init()
{
	m_CamFixY = true;
	m_Pos.y = WORLD_HEIGHT - m_Resolution.y;
	return false;
}

void CCamera::Update(float DeltaTime)
{
	if (m_Target)
	{
		if (!m_Target->IsActive())
		{
			m_Target = nullptr;
		}

		else
		{
			if (!m_CamFixX)
			{
				m_Pos.x = m_Target->GetPos().x - m_TargetPivot.x * m_Resolution.x +
					m_TargetOffset.x;
			}

			if (!m_CamFixY)
			{
				m_Pos.y = m_Target->GetPos().y - m_TargetPivot.y * m_Resolution.y +
					m_TargetOffset.y;
			}
		}
	}
	
	// 카메라가 World를 벗어날 수 없도록 한다
	if (m_Pos.x < 0.f)
		m_Pos.x = 0.f;

	else if (m_Pos.x + m_Resolution.x > m_WorldResolution.x)
		m_Pos.x = m_WorldResolution.x - m_Resolution.x;

	if (m_Pos.y < 0.f)
		m_Pos.y = 0.f;

	else if (m_Pos.y + m_Resolution.y > m_WorldResolution.y)
		m_Pos.y = m_WorldResolution.y - m_Resolution.y;
}
