
#include "Bomb.h"
#include "../Collision/ColliderBox.h"
#include "../GameManager.h"
#include "../Object/EffectHit.h"
#include "../Scene/Scene.h"

CBomb::CBomb() :
	m_Dir(1.f, 0.f),
	m_CollisionCount(0),
	m_ForceXDir(70.f),
	m_ForceYDir(150.f),
	m_StartFall(false)
{
}

CBomb::CBomb(const CBomb& obj)	:
	CGameObject(obj)
{
	m_Dir = obj.m_Dir;
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

	SetZOrder(6);

	m_GravityAccel = 9.f;
	m_IsGround = false;
	m_PhysicsSimulate = false;

	SetPivot(0.5f, 0.5f);

	AddAnimation("BombRight", true, 0.8f);
	AddAnimation("BombLeft", true, 0.8f);

	CColliderBox* Body = AddCollider<CColliderBox>("Body");
	Body->SetExtent(35.f, 55.f);
	Body->SetOffset(0.f, 0.f);

	return true;
}

void CBomb::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	Vector2 Dir = m_Dir;
	Dir.Normalize();


	Vector2 CurrentMove = {};

	CurrentMove.x = Dir.x * m_ForceXDir * CGameManager::GetInst()->GetDeltaTime() * m_TimeScale * m_GravityAccel;

	if (!m_PhysicsSimulate)
	{
		CurrentMove.y = Dir.y * m_ForceYDir * CGameManager::GetInst()->GetDeltaTime() * m_TimeScale * m_GravityAccel;
	}

	m_Pos += CurrentMove;

	if (m_ForceYDir > 0.f)
		m_ForceYDir -= GRAVITY / 9.f;
	
	else if (m_ForceYDir <= 0.f && !m_StartFall)
	{
		m_PhysicsSimulate = true;
		m_FallStartY = m_Pos.y;
		m_StartFall = true;
		m_FallTime = 0.f;
	}

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

	if (DestName.find("Arabian") != std::string::npos)
	{
		CEffectHit* Hit = m_Scene->CreateObject<CEffectHit>(
		"BombExplosionEffect", "BombExplosionEffect",
		m_Pos + Vector2(0.f, -130.f), Vector2(150.f, 311.f));

		Destroy();
	}

	else if (DestName == "Stage")
	{
		if (m_CollisionCount == 0)
		{
			++m_CollisionCount;

			m_PhysicsSimulate = false;
			m_StartFall = false;
			m_ForceXDir = 40.f;
			m_ForceYDir = 120.f;
			m_Pos.y = Src->GetHitPoint().y;
		}

		else if (m_CollisionCount == 1)
		{
			CEffectHit* Hit = m_Scene->CreateObject<CEffectHit>(
				"BombExplosionEffect", "BombExplosionEffect",
				m_Pos + Vector2(0.f, -135.f), Vector2(150.f,311.f));
			Destroy();
		}
	}

}
