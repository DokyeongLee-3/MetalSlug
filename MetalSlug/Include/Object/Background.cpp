
#include "Background.h"
#include "../Scene/Camera.h"
#include "../Scene/Scene.h"

CBackground::CBackground()
{
}

CBackground::CBackground(const CBackground& obj)	:
	CGameObject(obj)
{
}

CBackground::~CBackground()
{
}

void CBackground::Start()
{
}

bool CBackground::Init()
{
	m_Pos = Vector2(0.f, 0.f);
	SetPivot(0.f, 0.f);

	m_PhysicsSimulate = false;

	return true;
}

void CBackground::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	Vector2 CamPos = m_Scene->GetCamera()->GetPos();

	if (GetName() == "Desert")
	{
		SetPos((CamPos.x) / 8, this->GetPos().y);
	}

	else if (GetName() == "SandyWave1")
	{
		SetPos((CamPos.x) / 5, this->GetPos().y);
	}

	else if (GetName() == "SandyWave2")
	{
		SetPos(700.f + (CamPos.x) / 5, this->GetPos().y);
	}

	else if (GetName() == "SandyWave3")
	{
		SetPos(1400.f + (CamPos.x) / 5, this->GetPos().y);
	}

	else if (GetName() == "SandyWave4")
	{
		SetPos(2100.f + (CamPos.x) / 5, this->GetPos().y);
	}

	else if (GetName() == "Sky")
	{
		SetPos((CamPos.x) / 1.2f, this->GetPos().y);
	}
}

void CBackground::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CBackground::Collision(float DeltaTime)
{
}

void CBackground::PrevRender()
{
	CGameObject::PrevRender();
}

void CBackground::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CBackground* CBackground::Clone()
{
	return new CBackground(*this);
}
