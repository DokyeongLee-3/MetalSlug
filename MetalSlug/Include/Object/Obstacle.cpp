
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

	m_PhysicsSimulate = false;
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
	CGameObject::Render(hDC);
}

CObstacle* CObstacle::Clone()
{
	return new CObstacle(*this);
}
