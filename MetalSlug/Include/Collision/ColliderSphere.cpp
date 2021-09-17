
#include "ColliderSphere.h"
#include "ColliderBox.h"
#include "../Object/GameObject.h"
#include "../GameManager.h"
#include "Collision.h"
#include "../Scene/Scene.h"
#include "../Scene/Camera.h"

CColliderSphere::CColliderSphere()
{
	m_Info.Radius = 50.f;
	m_Type = ECollider_Type::Sphere;
}

CColliderSphere::CColliderSphere(const CColliderSphere& collider)	:
	CCollider(collider)
{
	m_Info = collider.m_Info;
}



CColliderSphere::~CColliderSphere()
{
}

bool CColliderSphere::Init()
{
	if (!CCollider::Init())
		return false;

	return true;
}

void CColliderSphere::Update(float DeltaTime)
{
	CCollider::Update(DeltaTime);
}

void CColliderSphere::PostUpdate(float DeltaTime)
{
	CCollider::PostUpdate(DeltaTime);

	m_Info.Center = m_Owner->GetPos() + m_Offset;

}

void CColliderSphere::Render(HDC hDC)
{
	CCollider::Render(hDC);

#ifdef _DEBUG
	HPEN Pen = CGameManager::GetInst()->GetGreenPen();

	if (!m_CollisionList.empty() || m_MouseCollision)
		Pen = CGameManager::GetInst()->GetRedPen();

	CCamera* Camera = m_Scene->GetCamera();

	SphereInfo	RenderInfo = m_Info;

	RenderInfo.Center -= Camera->GetPos();

	// 현재 object를 일단 Prev에 담아두고 다 그리면
	// SelectObject로원래 object로 돌려준다.
	HGDIOBJ Prev = SelectObject(hDC, Pen);
	
	// FrameRect같은 안이 비어 있는 원을 그릴 수 있는
	// API가 제공되지 않으므로
	// Sphere충돌체를 LineTo로 최대한 원처럼 그려준다

	// 원의 시작점은 3시 방향
	MoveToEx(hDC, (int)(RenderInfo.Center.x + RenderInfo.Radius),
		(int)(RenderInfo.Center.y), nullptr);

	// 30도 마다 하나의 선을 그어줌
	for (int i = 0; i < 12; ++i)
	{
		float Radian = DegreeToRadian((i + 1) * (360.f / 12.f));
		
		float x = RenderInfo.Center.x + cosf(Radian) * RenderInfo.Radius;
		float y = RenderInfo.Center.y + sinf(Radian) * RenderInfo.Radius;

		LineTo(hDC, (int)x, (int)y);
	}

	SelectObject(hDC, Prev);

#endif // _DEBUG

}

CColliderSphere* CColliderSphere::Clone()
{
	return new CColliderSphere(*this);
}

bool CColliderSphere::Collision(CCollider* Dest)
{
	switch (Dest->GetColliderType())
	{
	case ECollider_Type::Box:
		return CCollision::CollisionBoxToSphere((CColliderBox*)Dest, this);
		break;
	case ECollider_Type::Sphere:
		return CCollision::CollisionSphereToSphere(this, (CColliderSphere*)Dest);
	case ECollider_Type::Point:
		break;
	default:
		break;
	}

	return false;
}

bool CColliderSphere::CollisionMouse(const Vector2& MousePos)
{
	float Dist = Distance(m_Info.Center, MousePos);

	return Dist <= m_Info.Radius;
}
