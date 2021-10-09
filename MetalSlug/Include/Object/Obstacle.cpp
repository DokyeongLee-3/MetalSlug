
#include "Obstacle.h"
#include "../Scene/Camera.h"
#include "../Scene/Scene.h"
#include "../GameManager.h"

CObstacle::CObstacle()
{
}

CObstacle::CObstacle(const CObstacle& obj)	:
	CGameObject(obj)
{
}

CObstacle::~CObstacle()
{
}

void CObstacle::Start()
{
}

bool CObstacle::Init()
{
	m_ObjType = EObject_Type::Obstacle;
	SetPivot(0.f, 0.f);
	m_PhysicsSimulate = false;

	// Player의 ZOrder가 6이다
	if (m_Name == "FrontObstacle")
	{
		SetZOrder(7);
		SetTexture("FrontObstacle");
	}

	else if (m_Name == "BackObstacle")
	{
		SetZOrder(5);
		SetTexture("BackObstacle");
	}

	SetTextureColorKey(255, 255, 255);
	return true;
}

void CObstacle::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CObstacle::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CObstacle::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CObstacle::PrevRender()
{
	CGameObject::PrevRender();
}

void CObstacle::Render(HDC hDC)
{
	//Vector2 Pos = m_Pos;
	//Vector2 RenderPos = m_RenderPos;
	//Vector2 CamPos = m_Scene->GetCamera()->GetPos();

	//int a = 3; 

	CGameObject::Render(hDC);
}

CObstacle* CObstacle::Clone()
{
	return new CObstacle(*this);
}
