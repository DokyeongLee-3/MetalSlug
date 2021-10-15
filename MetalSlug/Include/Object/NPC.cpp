
#include "NPC.h"
#include "../Scene/Scene.h"
#include "Player.h"
#include "../Collision/ColliderBox.h"

CNPC::CNPC()	:
	m_Target(nullptr),
	m_Changing(false),
	m_State(ENPC_State::Idle)
{
}

CNPC::CNPC(const CNPC& obj)	:
	CGameObject(obj)
{
}

CNPC::~CNPC()
{
}

void CNPC::Start()
{
	CGameObject::Start();
}

bool CNPC::Init()
{
	if (!CGameObject::Init())
		return false;

	m_ZOrder = 6;
	m_GravityAccel = 10.f;
	m_MoveSpeed = 70.f;
	m_Pivot = Vector2(0.5f, 1.f);
	m_PhysicsSimulate = true;
	m_IsGround = false;
	m_Jump = false;

	SetTarget((CPlayer*)m_Scene->GetPlayer());

	AddNPCAnimation();

	CColliderBox* Body = AddCollider<CColliderBox>("Body");
	Body->SetExtent(80.f, 90.f);
	Body->SetOffset(0.f, -45.f);
	Body->SetCollisionProfile("NPC");
	Body->SetCollisionBeginFunction<CNPC>(this, &CNPC::CollisionBegin);
	Body->SetCollisionStayFunction<CNPC>(this, &CNPC::CollisionStay);
	Body->SetCollisionEndFunction<CNPC>(this, &CNPC::CollisionEnd);

	return true;
}

void CNPC::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

}

void CNPC::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CNPC::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CNPC::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CNPC* CNPC::Clone()
{
	return new CNPC(*this);
}

void CNPC::FiniteState()
{
	std::string CurAnim = m_Animation->GetCurrentAnimation()->Sequence->GetName();

	// 애니메이션 동작중에 있으면 다른 애니메이션으로 전환 금지
	if (m_Changing)
		return;
}

void CNPC::AddNPCAnimation()
{
	AddAnimation("HelperIdleLeft", true, 1.2f);
	AddAnimation("HelperEscapeLeft", false, 0.5f);
	AddAnimation("HelperGiveLeft", false, 0.7f);
	AddAnimation("HelperRunLeft", true, 1.f);
	AddAnimation("HelperSaluteLeft", false, 0.7f);
	AddAnimation("HelperWalkLeft", true, 1.f);
	AddAnimation("HelperWalkRight", true, 1.f, 1.f, true);
}

void CNPC::CollisionBegin(CCollider* Src, CCollider* Dest, float DeltaTime)
{
	if (m_Changing)
		m_Changing = false;

	if (Dest->GetName() == "StagePixel")
	{
		m_IsGround = true;
		m_Jump = false;

		m_FallTime = 0.f;

		m_Pos.y = Src->GetHitPoint().y;
		m_FallStartY = m_Pos.y;
		m_JumpVelocity = 0.f;
		m_MoveSpeed = 200.f;
	}

	else if ((Dest->GetName() == "BackObstaclePixel") ||
		(Dest->GetName() == "FrontObstaclePixel"))
	{
		m_IsGround = true;
		m_Jump = false;

		m_FallTime = 0.f;

		m_Pos.y = Src->GetHitPoint().y;
		m_FallStartY = m_Pos.y;
		m_JumpVelocity = 0.f;
	}
}

void CNPC::CollisionStay(CCollider* Src, CCollider* Dest, float DeltaTime)
{
	if (Dest->GetName() == "StagePixel")
	{
		m_IsGround = true;
		m_Jump = false;

		m_FallTime = 0.f;

		m_Pos.y = Src->GetHitPoint().y;
		m_FallStartY = m_Pos.y;
		m_JumpVelocity = 0.f;
	}

	else if ((Dest->GetName() == "BackObstaclePixel") ||
		(Dest->GetName() == "FrontObstaclePixel"))
	{
		m_IsGround = true;
		m_Jump = false;

		m_FallTime = 0.f;

		m_Pos.y = Src->GetHitPoint().y;
		m_FallStartY = m_Pos.y;
		m_JumpVelocity = 0.f;
	}
}

void CNPC::CollisionEnd(CCollider* Src, CCollider* Dest, float DeltaTime)
{
}
