
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
	return false;
}

void CObstacle::Update(float DeltaTime)
{
}

void CObstacle::PostUpdate(float DeltaTime)
{
}

void CObstacle::Collision(float DeltaTime)
{
}

void CObstacle::PrevRender()
{
}

void CObstacle::Render(HDC hDC)
{
}

CObstacle* CObstacle::Clone()
{
	return nullptr;
}
