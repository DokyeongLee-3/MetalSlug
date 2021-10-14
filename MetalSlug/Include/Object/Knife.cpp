
#include "Knife.h"
#include "../Collision/ColliderBox.h"
#include "../GameManager.h"
#include "../Object/EffectHit.h"
#include "../Scene/Scene.h"

CKnife::CKnife()	:
	m_Dir(1.f, 0.f),
	m_ForceXDir(50.f),
	m_ForceYDir(300.f),
	m_StartFall(false),
	m_Stuck(false)
{
}

CKnife::CKnife(const CKnife& obj)	:
	CGameObject(obj)
{
	m_Dir = obj.m_Dir;
	m_ForceXDir = obj.m_ForceXDir;
	m_ForceYDir = obj.m_ForceYDir;
}

CKnife::~CKnife()
{
}

void CKnife::Start()
{
	CGameObject::Start();

	CCollider* Body = FindCollider("Body");

	Body->SetCollisionBeginFunction<CKnife>(this,
		&CKnife::CollisionBegin);

	// EndNotify를 Init에서 설정해주면 SetAnimationEndNotify의
	// 인자에 들어가는 this는 Clone하고자 하는 객체가 아니라
	// m_mapPrototype에 들어가있는 프로토타입을 가리키는 this이므로
	// Init에서 해주면 안되고 Start에서 해줘야한다
	SetAnimationEndNotify<CKnife>("KnifeRight", this,
		&CKnife::DestroyAfterStuck);
	SetAnimationEndNotify<CKnife>("KnifeLeft", this,
		&CKnife::DestroyAfterStuck);
}

bool CKnife::Init()
{
	if (!CGameObject::Init())
		return false;

	SetZOrder(6);

	m_GravityAccel = 6.f;
	m_IsGround = false;
	m_PhysicsSimulate = false;

	SetPivot(0.5f, 0.5f);

	AddAnimation("KnifeRight", true, 1.2f);
	AddAnimation("KnifeLeft", true, 1.2f);

	CColliderBox* Body = AddCollider<CColliderBox>("Body");
	Body->SetExtent(35.f, 55.f);
	Body->SetOffset(0.f, 0.f);
	Body->SetCollisionProfile("MonsterAttack");

	return true;
}

void CKnife::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	// 땅에 꼽히는 프레임은 땅이랑 충돌됐을 때부터 시작
	if (!m_Stuck && m_Animation->GetCurrentAnimation()->Frame >= 11)
		m_Animation->GetCurrentAnimation()->Frame = 0;

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
		m_ForceYDir = 0.f;
	}
}

void CKnife::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CKnife::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CKnife::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CKnife* CKnife::Clone()
{
	return new CKnife(*this);
}

void CKnife::CollisionBegin(CCollider* Src,
	CCollider* Dest, float DeltaTime)
{
	std::string DestName = Dest->GetOwner()->GetName();
	if (DestName == "Player")
	{
		Destroy();
	}

	else if (DestName == "Stage")
	{
		m_PhysicsSimulate = false;
		m_IsGround = true;
		m_Animation->GetCurrentAnimation()->Frame = 11;
		m_ForceXDir = 0.f;
		m_ForceYDir = 0.f;
		m_Stuck = true;

		CCollider* Body = FindCollider("Body");
		Body->Destroy();
	}
}

void CKnife::DestroyAfterStuck()
{
	Destroy();
}
