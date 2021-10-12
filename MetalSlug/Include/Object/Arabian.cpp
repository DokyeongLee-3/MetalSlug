
#include "Arabian.h"
#include "../Scene/Scene.h"
#include "../Collision/ColliderBox.h"

CArabian::CArabian()	:
	m_ThrowDistance(300.f),
	m_SwingDistance(80.f),
	m_State(EArabian_State::Shuffle),
	m_Target(nullptr),
	m_AttackTime(0),
	m_Changing(false)
{
	m_ObjType = EObject_Type::Character;
}

CArabian::CArabian(const CArabian& obj)	:
	CMonster(obj)
{
	m_ThrowDistance = obj.m_ThrowDistance;
	m_State = EArabian_State::Shuffle;
	m_Target = obj.m_Target;

	
}

CArabian::~CArabian()
{
}

void CArabian::Start()
{
	CMonster::Start();
}

bool CArabian::Init()
{
	if(!CMonster::Init())
		return false;

	m_ObjType = EObject_Type::Character;
	m_State = EArabian_State::Shuffle;

	m_PhysicsSimulate = true;
	m_IsGround = false;
	m_ZOrder = 6;
	m_GravityAccel = 10.f;
	m_MoveSpeed = 50.f;
	m_Pivot = Vector2(0.5f, 1.f);

	CreateAnimation();
	AddArabianAnimation();
	

	CColliderBox* Body = AddCollider<CColliderBox>("Body");
	Body->SetExtent(80.f, 120.f);
	Body->SetOffset(0.f, -60.f);
	Body->SetCollisionProfile("Monster");
	Body->SetCollisionBeginFunction<CArabian>(this, &CArabian::CollisionBegin);
	Body->SetCollisionStayFunction<CArabian>(this, &CArabian::CollisionStay);
	Body->SetCollisionEndFunction<CArabian>(this, &CArabian::CollisionEnd);

	return true;
}

void CArabian::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);

	// 도중에 끊으면 안되는 애니메이션이 있다면 FiniteState를 호출하지
	// 않음으로써 다른 애니메이션으로 전환하지 않는다
	if(!m_Changing)
		FiniteState();

	std::string CurAnim = m_Animation->GetCurrentAnimation()->Sequence->GetName();

	if (CurAnim.find("RunLeft") != std::string::npos)
	{
		CGameObject::Move(Vector2(-1.f, 0.f));

		// 위의 Move로 인해서 X방향으로만 이동하기 전에는 바닥과
		// 충돌중이었으나 X방향으로 이동함으로써 내리막길인 
		// 바닥과 순간적으로 충돌하지 않으나 아직 바닥과의 충돌함수가
		// 호출되기 전이라서 m_IsGround는 true로 되어 있어서
		// Update에서 중력이 작용하는 코드도 이번 프레임에서는
		// 건너뛴다. 따라서 여기서 추가적으로 중력을 작용하는 코드를 작성해준다
		if (m_IsGround)
		{
			CCollider* Body = FindCollider("Body");

			if (Body->CheckCollisionList("StagePixel"))
			{
				Vector2 HitPoint = Body->GetHitPoint();
				Vector2 Offset = Body->GetOffset();

				float Width = ((CColliderBox*)Body)->GetWidth();
				float Height = ((CColliderBox*)Body)->GetHeight();

				RectInfo Rect;
				Rect.Left = m_Pos.x - Width / 2.f + Offset.x;
				Rect.Top = m_Pos.y - Height / 2.f + Offset.y;
				Rect.Right = m_Pos.x + Width / 2.f + Offset.x;
				Rect.Bottom = m_Pos.y + Height / 2.f + Offset.y;

				((CColliderBox*)Body)->SetInfo(Rect);

				if (Rect.Left > HitPoint.x || Rect.Right < HitPoint.x)
				{
					m_FallTime += DeltaTime * m_GravityAccel * 30;

					m_Pos.y += 0.5f * GRAVITY * m_FallTime;
				}
			}
		}
	}

	else if (CurAnim.find("RunRight") != std::string::npos)
	{
		CGameObject::Move(Vector2(1.f, 0.f));

		if (m_IsGround)
		{
			CCollider* Body = FindCollider("Body");

			if (Body->CheckCollisionList("StagePixel"))
			{
				Vector2 HitPoint = Body->GetHitPoint();
				Vector2 Offset = Body->GetOffset();

				float Width = ((CColliderBox*)Body)->GetWidth();
				float Height = ((CColliderBox*)Body)->GetHeight();

				RectInfo Rect;
				Rect.Left = m_Pos.x - Width / 2.f + Offset.x;
				Rect.Top = m_Pos.y - Height / 2.f + Offset.y;
				Rect.Right = m_Pos.x + Width / 2.f + Offset.x;
				Rect.Bottom = m_Pos.y + Height / 2.f + Offset.y;

				((CColliderBox*)Body)->SetInfo(Rect);

				float MidPoint = (Rect.Left + Rect.Right) / 2;

				if (MidPoint - ((CColliderBox*)Body)->GetWidth() / 2 > HitPoint.x)
				{
					m_FallTime += DeltaTime * m_GravityAccel * 30;

					m_Pos.y += 0.5f * GRAVITY * m_FallTime;
				}
			}
		}
	}


}

void CArabian::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

void CArabian::Collision(float DeltaTime)
{
	CMonster::Collision(DeltaTime);
}

void CArabian::Render(HDC hDC)
{
	CMonster::Render(hDC);

	wchar_t StrPos_x[32] = {};
	wchar_t StrPos_y[32] = {};
	wsprintf(StrPos_x, L"x : %d", (int)m_Pos.x);
	wsprintf(StrPos_y, L"y : %d", (int)m_Pos.y);
	//m_Pos -= m_Scene->GetCamera()->GetPos();
	TextOut(hDC, (int)m_RenderPos.x - 10, (int)m_RenderPos.y - 160, StrPos_x, lstrlen(StrPos_x));
	TextOut(hDC, (int)m_RenderPos.x - 10, (int)m_RenderPos.y - 140, StrPos_y, lstrlen(StrPos_y));
}

CArabian* CArabian::Clone()
{
	return new CArabian(*this);
}

void CArabian::FiniteState()
{
	// 애니메이션 동작중에 있으면 다른 애니메이션으로 전환 금지
	if (m_Changing)
		return;

	std::string CurAnim = m_Animation->GetCurrentAnimation()->Sequence->GetName();

	if (m_Pos.x - m_Target->GetPos().x > 0 &&
		CurAnim.find("Right") != std::string::npos)
	{
		ChangeAnimation("ArabianTurnRight");
		m_Changing = true;
		return;
	}

	if (m_Pos.x - m_Target->GetPos().x < 0 &&
		CurAnim.find("Left") != std::string::npos)
	{
		ChangeAnimation("ArabianTurnLeft");
		m_Changing = true;
		return;
	}

	if (m_Target)
	{
		float Dist = abs(m_Pos.x - m_Target->GetPos().x);

		if (Dist <= m_ThrowDistance)
		{
			m_State = EArabian_State::Shuffle;

			if (Dist <= m_SwingDistance)
				m_State = EArabian_State::Attack;
		}

		else
		{
			m_State = EArabian_State::Trace;
		}
	}

	switch (m_State)
	{
	case EArabian_State::Shuffle:
		if (CurAnim.find("Shuffle") == std::string::npos)
		{
			Shuffle();
		}
		break;
	case EArabian_State::Trace:
		if (CurAnim.find("Run") == std::string::npos)
		{
			Running();
		}
		break;
	case EArabian_State::Attack:
		if (CurAnim.find("Attack") == std::string::npos)
		{
			Swing();
		}
		break;
	}
}

void CArabian::AddArabianAnimation()
{
	AddAnimation("ArabianRunLeft", true, 1.f);
	AddAnimation("ArabianRunRight", true, 1.f, 1.f, true);
	AddAnimation("ArabianThrowLeft", false, 1.6f);
	AddAnimation("ArabianThrowRight", false, 1.6f, 1.f, true);
	AddAnimation("ArabianShuffleLeft", false, 0.8f);
	AddAnimation("ArabianShuffleRight", false, 0.8f, 1.f, true);
	AddAnimation("ArabianPrepareLeft", false, 0.6f);
	AddAnimation("ArabianPrepareRight", false, 0.6f, 1.f, true);
	AddAnimation("ArabianAttackLeft", false, 0.8f);
	AddAnimation("ArabianAttackRight", false, 0.8f, 1.f, true);
	AddAnimation("ArabianDeathAnimationLeft1", false, 1.f);
	AddAnimation("ArabianDeathAnimationRight1", false, 1.f, 1.f, true);
	AddAnimation("ArabianDeathAnimationLeft2", false, 1.f);
	AddAnimation("ArabianDeathAnimationRight2", false, 1.f, 1.f, true);
	AddAnimation("ArabianDeathAnimationLeft3", false, 1.f);
	AddAnimation("ArabianDeathAnimationRight3", false, 1.f, 1.f, true);
	AddAnimation("ArabianJumpLeft", false, 1.f);
	AddAnimation("ArabianJumpRight", false, 1.f, 1.f, true);
	AddAnimation("ArabianTurnLeft", false, 0.2f);
	AddAnimation("ArabianTurnRight", false, 0.2f, 1.f, true);

	SetAnimationEndNotify<CArabian>("ArabianThrowLeft",
		this, &CArabian::FiniteState);
	SetAnimationEndNotify<CArabian>("ArabianThrowRight",
		this, &CArabian::FiniteState);
	SetAnimationEndNotify<CArabian>("ArabianAttackLeft",
		this, &CArabian::SwingEnd);
	SetAnimationEndNotify<CArabian>("ArabianAttackRight",
		this, &CArabian::SwingEnd);
	SetAnimationEndNotify<CArabian>("ArabianJumpLeft",
		this, &CArabian::FiniteState);
	SetAnimationEndNotify<CArabian>("ArabianJumpRight",
		this, &CArabian::FiniteState);
	SetAnimationEndNotify<CArabian>("ArabianPrepareLeft",
		this, &CArabian::PrepareEnd);
	SetAnimationEndNotify<CArabian>("ArabianPrepareRight",
		this, &CArabian::PrepareEnd);
	SetAnimationEndNotify<CArabian>("ArabianTurnLeft",
		this, &CArabian::ChangingEnd);
	SetAnimationEndNotify<CArabian>("ArabianTurnRight",
		this, &CArabian::ChangingEnd);

	SetAnimationEndNotify<CArabian>("ArabianShuffleLeft",
		this, &CArabian::Prepare);
	SetAnimationEndNotify<CArabian>("ArabianShuffleRight",
		this, &CArabian::Prepare);

}

void CArabian::CollisionBegin(CCollider* Src, CCollider* Dest, float DeltaTime)
{
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

void CArabian::CollisionStay(CCollider* Src, CCollider* Dest, float DeltaTime)
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

void CArabian::CollisionEnd(CCollider* Src, CCollider* Dest, float DeltaTime)
{
}

void CArabian::Swing()
{
	std::string CurAnim = m_Animation->GetCurrentAnimation()->Sequence->GetName();

	if (CurAnim.find("Right") != std::string::npos)
	{
		ChangeAnimation("ArabianAttackRight");
	}

	else if (CurAnim.find("Left") != std::string::npos)
	{
		ChangeAnimation("ArabianAttackLeft");
	}

	m_Changing = true;
}

void CArabian::SwingEnd()
{
	m_Changing = false;

	FiniteState();
}

void CArabian::Shuffle()
{
	std::string CurAnim = m_Animation->GetCurrentAnimation()->Sequence->GetName();

	if (CurAnim.find("Right") != std::string::npos)
	{
		ChangeAnimation("ArabianShuffleRight");
	}

	else if (CurAnim.find("Left") != std::string::npos)
	{
		ChangeAnimation("ArabianShuffleLeft");
	}


}

void CArabian::Prepare()
{
	std::string CurAnim = m_Animation->GetCurrentAnimation()->Sequence->GetName();

	if (CurAnim.find("Right") != std::string::npos)
	{
		ChangeAnimation("ArabianPrepareRight");
	}

	else if (CurAnim.find("Left") != std::string::npos)
	{
		ChangeAnimation("ArabianPrepareLeft");
	}

	m_Changing = true;
}

void CArabian::PrepareEnd()
{
	m_Changing = false;
	FiniteState();
}

void CArabian::Throw()
{

}

void CArabian::Running()
{
	std::string CurAnim = m_Animation->GetCurrentAnimation()->Sequence->GetName();

	if (CurAnim.find("Right") != std::string::npos)
	{
		ChangeAnimation("ArabianRunRight");
	}

	else if (CurAnim.find("Left") != std::string::npos)
	{
		ChangeAnimation("ArabianRunLeft");
	}
}

void CArabian::ChangingEnd()
{
	m_Changing = false;
	m_State = EArabian_State::Shuffle;

	std::string CurAnim = m_Animation->GetCurrentAnimation()->Sequence->GetName();

	if (CurAnim.find("Left") != std::string::npos)
	{
		ChangeAnimation("ArabianShuffleRight");
	}

	else if (CurAnim.find("Right") != std::string::npos)
	{
		ChangeAnimation("ArabianShuffleLeft");
	}
}
