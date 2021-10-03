
#include "Bomb.h"
#include "../Collision/ColliderBox.h"
#include "../GameManager.h"

CBomb::CBomb() :
	m_Dir(1.f, 0.f),
	m_CollisionCount(0),
	m_ForceXDir(70.f),
	m_ForceYDir(4000.f)
{
}

CBomb::CBomb(const CBomb& obj)	:
	CGameObject(obj)
{
	m_Dir = obj.m_Dir;
	m_ForceXDir = obj.m_ForceXDir;
	m_CollisionCount = obj.m_CollisionCount;
	m_ForceXDir = obj.m_ForceXDir;
	m_ForceYDir = obj.m_ForceYDir;
}

CBomb::~CBomb()
{
}

void CBomb::Start()
{
	CGameObject::Start();

	CCollider* Body = FindCollider("Body");

	Body->SetCollisionBeginFunction<CBomb>(this,
		&CBomb::CollisionBegin);
}

bool CBomb::Init()
{
	if (!CGameObject::Init())
		return false;

	m_GravityAccel = 8.f;
	m_IsGround = false;
	m_PhysicsSimulate = true;

	SetPivot(0.5f, 0.5f);

	AddAnimation("BombRight", true, 1.2f);
	AddAnimation("BombLeft", true, 1.2f);

	CColliderBox* Body = AddCollider<CColliderBox>("Body");
	Body->SetExtent(50.f, 60.f);
	Body->SetOffset(0.f, 0.f);

	return true;
}

void CBomb::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	Vector2 Dir = m_Dir;
	Dir.Normalize();

	Vector2 CurrentMove;
	CurrentMove.x = Dir.x * m_ForceXDir * CGameManager::GetInst()->GetDeltaTime() * m_TimeScale * m_GravityAccel;
	CurrentMove.y = Dir.y * m_ForceYDir * CGameManager::GetInst()->GetDeltaTime() * m_TimeScale * m_GravityAccel;
	m_Pos += CurrentMove;

	if (m_ForceYDir >= 0.f)
		m_ForceYDir -= GRAVITY * 10;
}

void CBomb::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CBomb::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CBomb::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CBomb* CBomb::Clone()
{
	return new CBomb(*this);
}

void CBomb::CollisionBegin(CCollider* Src, CCollider* Dest, float DeltaTime)
{
	std::string DestName = Dest->GetOwner()->GetName();

	if (DestName == "Stage")
	{
		if (m_CollisionCount == 0)
		{
			++m_CollisionCount;

			CGameObject::Jump();
			m_JumpVelocity = 40.f;
		}

		else if (m_CollisionCount == 1)
		{
			Destroy();
		}
	}
}
