
#include "Player.h"
#include "../GameManager.h"
#include "../Scene/Scene.h"
#include "../Input.h"
#include "../Collision/ColliderBox.h"
#include "../Collision/ColliderSphere.h"
#include "../Scene/SceneCollision.h"
#include "../Scene/Camera.h"
#include "../Scene/SceneResource.h"

CPlayer::CPlayer() :
	m_TopAnimation(nullptr),
	m_BottomAnimation(nullptr),
	m_SitDown(false),
	m_BeforeRender(true),
	m_ObstacleFall(false)
{
}

CPlayer::CPlayer(const CPlayer& obj)	:
	CCharacter(obj)
{
}

CPlayer::~CPlayer()
{
	SAFE_DELETE(m_TopAnimation);
	SAFE_DELETE(m_BottomAnimation);
}

void CPlayer::Start()
{
	CCharacter::Start();

	// 실질적으로 key의 콜백함수들은 애니메이션을 바꿔주는
	// 용도이고, 애니메이션 시퀀스 내에서 총알나가고,
	// 원래 Idle로 돌아오고 이런 것들은 Notify로 설정해놓음
	CInput::GetInst()->SetCallback<CPlayer>("Jump",
		KeyState_Down, this, &CPlayer::Jump);

	CInput::GetInst()->SetCallback<CPlayer>("Down",
		KeyState_Push, this, &CPlayer::Down);

	CInput::GetInst()->SetCallback<CPlayer>("LookUp",
		KeyState_Push, this, &CPlayer::LookUp);

	CInput::GetInst()->SetCallback<CPlayer>("MoveLeft",
		KeyState_Push, this, &CPlayer::MoveLeft);

	CInput::GetInst()->SetCallback<CPlayer>("MoveRight",
		KeyState_Push, this, &CPlayer::MoveRight);

	CInput::GetInst()->SetCallback<CPlayer>("Fire",
		KeyState_Down, this, &CPlayer::BulletFire);

	CInput::GetInst()->SetCallback<CPlayer>("Bomb",
		KeyState_Down, this, &CPlayer::Bomb);

	CInput::GetInst()->SetCallback<CPlayer>("Pause",
		KeyState_Down, this, &CPlayer::Pause);

	CInput::GetInst()->SetCallback<CPlayer>("Resume",
		KeyState_Down, this, &CPlayer::Resume);

}

bool CPlayer::Init()
{
	if (!CCharacter::Init())
		return false;

	m_GravityAccel = 13.f;
	m_MoveSpeed = 400.f;
	m_ZOrder = 6;
	m_ObjType = EObject_Type::Player;

	// 피봇을 밭밑으로 보통 잡는다
	SetPivot(0.5f, 1.f);

	CreateTopAnimation();
	CreateBottomAnimation();
	AddTopAnimation("PlayerIdleRightTop", true, 0.8f);
	AddBottomAnimation("PlayerIdleRightBottom", true, 0.8f);
	AddTopAnimation("PlayerIdleLeftTop", true, 0.8f, 1.f, true);
	AddBottomAnimation("PlayerIdleLeftBottom", true, 0.8f, 1.f, true);

	AddTopAnimation("PlayerNormalFireRightTop", false, 0.15f);
	AddBottomAnimation("PlayerNormalFireRightBottom", false, 0.15f);
	AddTopAnimation("PlayerNormalFireLeftTop", false, 0.15f, 1.f, true);
	AddBottomAnimation("PlayerNormalFireLeftBottom", false, 0.15f, 1.f, true);
	AddTopAnimationNotify<CPlayer>("PlayerNormalFireRightTop",
		1, this, &CPlayer::CloneBullet);
	AddTopAnimationNotify<CPlayer>("PlayerNormalFireLeftTop",
		8, this, &CPlayer::CloneBullet);


	AddTopAnimation("PlayerRunRightTop", true, 1.f);
	AddBottomAnimation("PlayerRunRightBottom", true, 1.f);
	AddTopAnimation("PlayerRunLeftTop", true, 1.f, 1.f, true);
	AddBottomAnimation("PlayerRunLeftBottom", true, 1.f, 1.f, true);

	AddTopAnimation("PlayerVerticalJumpRightTop", false, 1.f);
	AddBottomAnimation("PlayerVerticalJumpRightBottom", false, 1.f);
	AddTopAnimation("PlayerVerticalJumpLeftTop", false, 1.f, 1.f, true);
	AddBottomAnimation("PlayerVerticalJumpLeftBottom", false, 1.f, 1.f, true);

	AddTopAnimation("PlayerJumpDownRightTop", false, 0.8f, 1.f, true);
	AddTopAnimation("PlayerJumpDownLeftTop", false, 0.8f, 1.f, true);

	AddTopAnimation("PlayerJumpAttackDownRightTop", false, 0.15f);
	AddTopAnimation("PlayerJumpAttackDownLeftTop", false, 0.15f, 1.f, true);

	AddTopAnimationNotify<CPlayer>("PlayerJumpAttackDownRightTop",
		1, this, &CPlayer::CloneBullet);
	AddTopAnimationNotify<CPlayer>("PlayerJumpAttackDownLeftTop",
		5, this, &CPlayer::CloneBullet);

	SetTopAnimationEndNotify<CPlayer>("PlayerJumpAttackDownRightTop",
		this, &CPlayer::TopAttackEnd);
	SetTopAnimationEndNotify<CPlayer>("PlayerJumpAttackDownLeftTop",
		this, &CPlayer::TopAttackEnd);

	AddTopAnimation("PlayerLookUpRightTop", true, 0.8f);
	AddTopAnimation("PlayerLookUpLeftTop", true, 0.8f, 1.f, true);

	AddTopAnimation("PlayerLookUpAttackRightTop", false, 0.15f);
	AddTopAnimation("PlayerLookUpAttackLeftTop", false, 0.15f, 1.f, true);

	AddTopAnimationNotify<CPlayer>("PlayerLookUpAttackRightTop",
		1, this, &CPlayer::CloneBullet);
	AddTopAnimationNotify<CPlayer>("PlayerLookUpAttackLeftTop",
		8, this, &CPlayer::CloneBullet);

	SetTopAnimationEndNotify<CPlayer>("PlayerLookUpAttackRightTop",
		this, &CPlayer::TopAttackEnd);
	SetTopAnimationEndNotify<CPlayer>("PlayerLookUpAttackLeftTop",
		this, &CPlayer::TopAttackEnd);


	SetTopAnimationEndNotify<CPlayer>("PlayerNormalFireRightTop",
		this, &CPlayer::TopAttackEnd);
	SetBottomAnimationEndNotify<CPlayer>("PlayerNormalFireRightBottom",
		this, &CPlayer::BottomAttackEnd);
	SetTopAnimationEndNotify<CPlayer>("PlayerNormalFireLeftTop",
		this, &CPlayer::TopAttackEnd);
	SetBottomAnimationEndNotify<CPlayer>("PlayerNormalFireLeftBottom",
		this, &CPlayer::BottomAttackEnd);

	AddTopAnimation("PlayerFrontJumpRightTop", false, 0.7f);
	AddBottomAnimation("PlayerFrontJumpRightBottom", false, 0.7f);
	AddTopAnimation("PlayerFrontJumpLeftTop", false, 0.7f, 1.f, true);
	AddBottomAnimation("PlayerFrontJumpLeftBottom", false, 0.7f, 1.f, true);

	AddTopAnimation("Blank", false, 0.8f);
	AddBottomAnimation("PlayerSitDownIdleRight", true, 0.8f);
	AddBottomAnimation("PlayerSitDownIdleLeft", true, 0.8f, 1.f, true);

	AddBottomAnimation("PlayerCrawlRight", true, 0.8f);
	AddBottomAnimation("PlayerCrawlLeft", true, 0.8f, 1.f, true);

	AddBottomAnimation("SitDownNormalAttackRight", false, 0.15f);
	AddBottomAnimation("SitDownNormalAttackLeft", false, 0.15f, 1.f, true);

	AddBottomAnimationNotify<CPlayer>("SitDownNormalAttackRight",
		0, this, &CPlayer::CloneBullet);
	AddBottomAnimationNotify<CPlayer>("SitDownNormalAttackLeft",
		8, this, &CPlayer::CloneBullet);

	AddTopAnimationNotify<CPlayer>("SitDownNormalAttackRight",
		0, this, &CPlayer::CloneBullet);
	AddTopAnimationNotify<CPlayer>("SitDownNormalAttackLeft",
		9, this, &CPlayer::CloneBullet);
	SetBottomAnimationEndNotify<CPlayer>("SitDownNormalAttackRight",
		this, &CPlayer::SitDownBottomAttackEnd);
	SetBottomAnimationEndNotify<CPlayer>("SitDownNormalAttackLeft",
		this, &CPlayer::SitDownBottomAttackEnd);

	AddTopAnimation("BombRightTop", false, 0.3f);
	AddTopAnimation("BombLeftTop", false, 0.3f, 1.f, true);
	AddTopAnimationNotify<CPlayer>("BombRightTop",
		2, this, &CPlayer::CloneBomb);
	AddTopAnimationNotify<CPlayer>("BombLeftTop",
		2, this, &CPlayer::CloneBomb);
	SetTopAnimationEndNotify<CPlayer>("BombRightTop",
		this, &CPlayer::BombEnd);
	SetTopAnimationEndNotify<CPlayer>("BombLeftTop",
		this, &CPlayer::BombEnd);

	AddBottomAnimation("PlayerSitDownBombRight", false, 0.4f);
	AddBottomAnimation("PlayerSitDownBombLeft", false, 0.4f, 1.f, true);
	AddBottomAnimationNotify<CPlayer>("PlayerSitDownBombRight",
		1, this, &CPlayer::CloneBomb);
	AddBottomAnimationNotify<CPlayer>("PlayerSitDownBombLeft",
		1, this, &CPlayer::CloneBomb);
	SetBottomAnimationEndNotify<CPlayer>("PlayerSitDownBombRight",
		this, &CPlayer::SitDownBombEnd);
	SetBottomAnimationEndNotify<CPlayer>("PlayerSitDownBombLeft",
		this, &CPlayer::SitDownBombEnd);

	CColliderSphere* Head = AddCollider<CColliderSphere>("Head");
	Head->SetRadius(25.f);
	Head->SetOffset(0.f, -75.f);
	Head->SetCollisionProfile("Player");

	CColliderBox* Body = AddCollider<CColliderBox>("Body");
	Body->SetExtent(65.f, 45.f);
	Body->SetOffset(0.f, -22.5f);
	Body->SetCollisionProfile("Player");
	Body->SetCollisionBeginFunction<CPlayer>(this, &CPlayer::CollisionBegin);
	Body->SetCollisionStayFunction<CPlayer>(this, &CPlayer::CollisionStay);
	Body->SetCollisionEndFunction<CPlayer>(this, &CPlayer::CollisionEnd);

	m_PhysicsSimulate = true;
	m_IsGround = false;
	SetJumpVelocity(50.f);

	return true;
}

void CPlayer::Update(float DeltaTime)
{

	if (!m_Start)
	{
		Start();
	}

	if (m_TopAnimation)
		m_TopAnimation->Update(DeltaTime);

	if (m_BottomAnimation)
		m_BottomAnimation->Update(DeltaTime);


	// 중력을 적용한다.
	if (!m_IsGround && m_PhysicsSimulate)
	{
		// SelectScene에서 MainScene으로 넘어오면서
		// 시간이 오래걸려서 Player가 이미 너무 많이
		// 추락하는 경우를 대비해서
		// 첫프레임에서는 m_FallTime을 0으로
		if (m_BeforeRender)
		{
			m_FallTime = 0.f;
			m_BeforeRender = false;
		}

		else
		{
			// 떨어지는 시간을 누적시켜준다.
			m_FallTime += DeltaTime * m_GravityAccel;
		}

		float	Velocity = 0.f;

		if (m_Jump)
			Velocity = m_JumpVelocity * m_FallTime;

		m_Pos.y = m_FallStartY - (Velocity - 0.5f * GRAVITY * m_FallTime * m_FallTime);

		// 점프에 의해서 아니고 지형지물에서 떨어지는 중이라면
		// 앞으로 조금씩 움직여준다
		if (m_ObstacleFall)
		{
			std::string Top = m_TopAnimation->m_CurrentAnimation->Sequence->GetName();

			if (Top.find("Right") != std::string::npos)
				m_Pos.x += 200.f * DeltaTime * m_TimeScale;

			else if(Top.find("Left") != std::string::npos)
				m_Pos.x -= 200.f * DeltaTime * m_TimeScale;
		}
	}

	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();
	
	for (; iter != iterEnd; )
	{
		if (!(*iter)->IsActive())
		{
			iter = m_ColliderList.erase(iter);
			iterEnd = m_ColliderList.end();
			continue;
		}

		// 비활성화 되어있는 충돌체는 Update하지 않는다
		else if ((*iter)->GetEnable())
		{
			(*iter)->Update(DeltaTime);
		}

		++iter;
	}
}

void CPlayer::PostUpdate(float DeltaTime)
{
	//CCharacter::PostUpdate(DeltaTime);

	std::string CurBottomAnimation = m_BottomAnimation->m_CurrentAnimation->Sequence->GetName();
	std::string CurTopAnimation = m_TopAnimation->m_CurrentAnimation->Sequence->GetName();

	if (m_IsGround && 
		CurBottomAnimation.find("Run") != std::string::npos)
	{
		if (m_Velocity.Length() == 0.f)
		{
			if (CurBottomAnimation.find("Right") != std::string::npos)
			{
				ChangeTopAnimation("PlayerIdleRightTop");
				ChangeBottomAnimation("PlayerIdleRightBottom");
			}

			else
			{
				ChangeTopAnimation("PlayerIdleLeftTop");
				ChangeBottomAnimation("PlayerIdleLeftBottom");
			}
		}
	}

	else if (CurTopAnimation.find("LookUp") != std::string::npos)
	{
		if (!(GetAsyncKeyState('W') & 0x8000))
		{
			if (CurBottomAnimation.find("Right") != std::string::npos)
			{
				ChangeTopAnimation("PlayerIdleRightTop");
				ChangeBottomAnimation("PlayerIdleRightBottom");
			}

			else
			{
				ChangeTopAnimation("PlayerIdleLeftTop");
				ChangeBottomAnimation("PlayerIdleLeftBottom");
			}
		}
	}

	else if (m_IsGround &&
		(CurBottomAnimation.find("Vertical") != std::string::npos ||
			CurBottomAnimation.find("FrontJump") != std::string::npos))
	{
		if (CurBottomAnimation.find("Right") != std::string::npos)
		{
			ChangeTopAnimation("PlayerIdleRightTop");
			ChangeBottomAnimation("PlayerIdleRightBottom");
		}

		else if (CurBottomAnimation.find("Left") != std::string::npos)
		{
			ChangeTopAnimation("PlayerIdleLeftTop");
			ChangeBottomAnimation("PlayerIdleLeftBottom");
		}
	}

	else if (CurBottomAnimation.find("SitDown") != std::string::npos)
	{
		if (!(GetAsyncKeyState('S') & 0x8000))
		{
			if (CurBottomAnimation.find("Right") != std::string::npos)
			{
				ChangeTopAnimation("PlayerIdleRightTop");
				ChangeBottomAnimation("PlayerIdleRightBottom");
			}

			else if(CurBottomAnimation.find("Left") != std::string::npos)
			{
				ChangeTopAnimation("PlayerIdleLeftTop");
				ChangeBottomAnimation("PlayerIdleLeftBottom");
			}
			m_SitDown = false;
			m_MoveSpeed = 400.f;
			CCollider* Head = FindCollider("Head");
			Head->SetEnable(true);
		}
	}

	else if (CurBottomAnimation.find("Crawl") != std::string::npos)
	{
		if (!(GetAsyncKeyState('S') & 0x8000))
		{
			if (CurBottomAnimation.find("Right") != std::string::npos)
			{
				ChangeTopAnimation("PlayerIdleRightTop");
				ChangeBottomAnimation("PlayerIdleRightBottom");
			}

			else if (CurBottomAnimation.find("Left") != std::string::npos)
			{
				ChangeTopAnimation("PlayerIdleLeftTop");
				ChangeBottomAnimation("PlayerIdleLeftBottom");
			}
			m_SitDown = false;
			m_MoveSpeed = 400.f;
			CCollider* Head = FindCollider("Head");
			Head->SetEnable(true);
		}

		else
		{
			if (m_Velocity.Length() == 0.f && 
				CurBottomAnimation.find("Attack") == std::string::npos &&
				CurBottomAnimation.find("Bomb") == std::string::npos)
			{
				if (CurBottomAnimation.find("Right") != std::string::npos)
				{
					ChangeBottomAnimation("PlayerSitDownIdleRight");
				}

				else if (CurBottomAnimation.find("Left") != std::string::npos)
				{
					ChangeBottomAnimation("PlayerSitDownIdleLeft");
				}

				m_SitDown = true;
			}
		}
	}

	m_Size = Vector2(0.f, 0.f);

	if (m_TopAnimation)
	{
		AnimationInfo* AnimInfo = m_TopAnimation->m_CurrentAnimation;

		const AnimationFrameData& FrameData =
			AnimInfo->Sequence->GetFrameData(AnimInfo->Frame);

		m_Size += FrameData.Size;
	}

	if (m_BottomAnimation)
	{
		AnimationInfo* AnimInfo = m_BottomAnimation->m_CurrentAnimation;

		const AnimationFrameData& FrameData =
			AnimInfo->Sequence->GetFrameData(AnimInfo->Frame);

		m_Size += FrameData.Size;
	}

	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; )
	{
		if (!(*iter)->IsActive())
		{
			iter = m_ColliderList.erase(iter);
			iterEnd = m_ColliderList.end();
			continue;
		}

		// 비활성화 되어있는 충돌체는 PostUpdate하지 않는다
		else if ((*iter)->GetEnable())
		{
			(*iter)->PostUpdate(DeltaTime);
		}

		++iter;
	}
}

void CPlayer::Collision(float DeltaTime)
{
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		m_Scene->GetSceneCollision()->AddCollider(*iter);
	}
}

void CPlayer::PrevRender()
{
	CCamera* Camera = m_Scene->GetCamera();

	m_RenderPos = m_Pos - Camera->GetPos();
}

void CPlayer::Render(HDC hDC)
{
	if (m_BottomAnimation)
	{
		AnimationInfo* AnimInfo = m_BottomAnimation->m_CurrentAnimation;

		// CAnimation::Update에서 m_CurrentAnimation의 Frame 번호를
		// 시간이 지남에 따라 계속 update해주므로, GetFrameData인자에는
		// 순서에 맞는 Frame번호를 넘겨줄 것이고, GetFrameData는
		// CAnimationSequence의 m_vecFrameData에서 Frame번호에 맞는
		// FrameData를 넘겨줄 것이다
		const AnimationFrameData& FrameData =
			AnimInfo->Sequence->GetFrameData(AnimInfo->Frame);

		Vector2 LT = m_RenderPos - m_Pivot * FrameData.Size + FrameData.Offset + m_Offset;

		if (AnimInfo->Sequence->GetTextureType() == ETexture_Type::Atlas)
		{
			AnimInfo->Sequence->GetTexture()->Render(hDC, LT,
				FrameData.StartPos, FrameData.Size);
		}

		// Frame방식
		else
		{
			AnimInfo->Sequence->GetTexture()->Render(hDC, LT,
				FrameData.StartPos, FrameData.Size, AnimInfo->Frame);
		}

	}

	else
	{
		Vector2 LT = m_RenderPos - m_Pivot * m_Size + m_Offset;

		if (m_Texture)
		{
			m_Texture->Render(hDC, LT, m_ImageStart, m_Size);
		}

		else
		{
			Rectangle(hDC, (int)m_Pos.x, (int)m_Pos.y,
				(int)(m_Pos.x + m_Size.x), (int)(m_Pos.y + m_Size.y));
		}
	}

	if (m_TopAnimation)
	{
		AnimationInfo* AnimInfo = m_TopAnimation->m_CurrentAnimation;

		// CAnimation::Update에서 m_CurrentAnimation의 Frame 번호를
		// 시간이 지남에 따라 계속 update해주므로, GetFrameData인자에는
		// 순서에 맞는 Frame번호를 넘겨줄 것이고, GetFrameData는
		// CAnimationSequence의 m_vecFrameData에서 Frame번호에 맞는
		// FrameData를 넘겨줄 것이다
		const AnimationFrameData& FrameData =
			AnimInfo->Sequence->GetFrameData(AnimInfo->Frame);

		Vector2 LT = m_RenderPos - m_Pivot * FrameData.Size + FrameData.Offset + m_Offset;
		LT.y -= PLAYER_BOTTOMHEIGHT;

		if (AnimInfo->Sequence->GetTextureType() == ETexture_Type::Atlas)
		{
			AnimInfo->Sequence->GetTexture()->Render(hDC, LT,
				FrameData.StartPos, FrameData.Size);
		}

		// Frame방식
		else
		{
			AnimInfo->Sequence->GetTexture()->Render(hDC, LT,
				FrameData.StartPos, FrameData.Size, AnimInfo->Frame);
		}

	}

	else
	{
		Vector2 LT = m_RenderPos - m_Pivot * m_Size + m_Offset;

		if (m_Texture)
		{
			m_Texture->Render(hDC, LT, m_ImageStart, m_Size);
		}

		else
		{
			Rectangle(hDC, (int)m_Pos.x, (int)m_Pos.y,
				(int)(m_Pos.x + m_Size.x), (int)(m_Pos.y + m_Size.y));
		}
	}

	// 디버그모드라면 충돌체 모양을 그려줄 것임
	// #ifdef _DEBUG으로 define은 Collider의 자식 클래스들의
	// Render함수에 정의되어져 있음
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; )
	{
		if (!(*iter)->IsActive())
		{
			iter = m_ColliderList.erase(iter);
			iterEnd = m_ColliderList.end();
			continue;
		}

		else if ((*iter)->GetEnable())
		{
			(*iter)->Render(hDC);
		}

		++iter;
	}

	wchar_t StrPos_x[32] = {};
	wchar_t StrPos_y[32] = {};
	wsprintf(StrPos_x, L"x : %d", (int)m_Pos.x);
	wsprintf(StrPos_y, L"y : %d", (int)m_Pos.y);
	//m_Pos -= m_Scene->GetCamera()->GetPos();
	TextOut(hDC, (int)m_RenderPos.x - 10, (int)m_RenderPos.y - 160, StrPos_x, lstrlen(StrPos_x));
	TextOut(hDC, (int)m_RenderPos.x - 10, (int)m_RenderPos.y - 140, StrPos_y, lstrlen(StrPos_y));

	m_PrevPos = m_Pos;
	m_Velocity = Vector2(0.f, 0.f);
}

CPlayer* CPlayer::Clone()
{
	return new CPlayer(*this);
}

void CPlayer::Jump(float DeltaTime)
{
	// 자유낙하중에 점프 불가능
	if (m_FallTime != 0.f)
		return;

	std::string CurTop = m_TopAnimation->m_CurrentAnimation->Sequence->GetName();
	std::string CurBottom = m_BottomAnimation->m_CurrentAnimation->Sequence->GetName();

	// 현재 이미 점프중인데 또 다시 점프 모션 처음으로 돌리거나
	// 수류탄 던지는 중인데 끊고 점프 모션으로 전환하면 안된다
	if (CurTop.find("Jump") == std::string::npos &&
		CurTop.find("Bomb") == std::string::npos)
	{
		CGameObject::Jump();
		m_JumpVelocity = 60.f;
		Move(Vector2(0.f, -1.f));

		if (CurTop.find("Right") != std::string::npos)
		{
			ChangeTopAnimation("PlayerVerticalJumpRightTop");
			ChangeBottomAnimation("PlayerVerticalJumpRightBottom");
		}

		else if (CurTop.find("Left") != std::string::npos)
		{
			ChangeTopAnimation("PlayerVerticalJumpLeftTop");
			ChangeBottomAnimation("PlayerVerticalJumpLeftBottom");
		}
		
		m_SitDown = false;
		m_MoveSpeed = 400.f;
	}
}

void CPlayer::Down(float DeltaTime)
{
	//Move(Vector2(0.f, 1.f));
	//ChangeAnimation("LucidNunNaRightWalk");
	std::string CurTop = m_TopAnimation->m_CurrentAnimation->Sequence->GetName();
	std::string CurBottom = m_BottomAnimation->m_CurrentAnimation->Sequence->GetName();

	if (!m_IsGround)
	{
		if (CurTop.find("PlayerJumpAttackDown") == std::string::npos)
		{
			if (CurTop.find("Right") != std::string::npos)
			{
				ChangeTopAnimation("PlayerJumpDownRightTop");
			}

			else if (CurTop.find("Left") != std::string::npos)
			{
				ChangeTopAnimation("PlayerJumpDownLeftTop");
			}

			else if (CurBottom == "PlayerSitDownIdleRight")
			{
				ChangeTopAnimation("PlayerJumpDownRightTop");
				ChangeBottomAnimation("PlayerVerticalJumpRightBottom");

				CCollider* Head = FindCollider("Head");
				Head->SetEnable(true);
			}

			else if (CurBottom == "PlayerSitDownIdleLeft")
			{
				ChangeTopAnimation("PlayerJumpDownLeftTop");
				ChangeBottomAnimation("PlayerVerticalJumpLeftBottom");

				CCollider* Head = FindCollider("Head");
				Head->SetEnable(true);
			}

		}
	}

	else if(CurBottom.find("Attack") == std::string::npos &&
		CurBottom.find("Bomb") == std::string::npos)
	{
		ChangeTopAnimation("Blank");
		
		if (CurTop.find("Right") != std::string::npos)
		{
			ChangeBottomAnimation("PlayerSitDownIdleRight");
		}

		else if (CurTop.find("Left") != std::string::npos)
		{
			ChangeBottomAnimation("PlayerSitDownIdleLeft");
		}
		m_SitDown = true;

		CCollider* Head = FindCollider("Head");
		Head->SetEnable(false);
	}
}

void CPlayer::LookUp(float DeltaTime)
{
	std::string CurTop = m_TopAnimation->m_CurrentAnimation->Sequence->GetName();

	// 현재 공격중이 아닐때만 LookUp유지
	if (CurTop.find("Attack") == std::string::npos)
	{
		if (CurTop.find("Right") != std::string::npos)
		{
			ChangeTopAnimation("PlayerLookUpRightTop");
		}

		else if (CurTop.find("Left") != std::string::npos)
		{
			ChangeTopAnimation("PlayerLookUpLeftTop");
		}
	}

}

void CPlayer::MoveLeft(float DeltaTime)
{
	std::string CurTop = m_TopAnimation->m_CurrentAnimation->Sequence->GetName();
	std::string CurBottom = m_BottomAnimation->m_CurrentAnimation->Sequence->GetName();

	// 위의 Move로 인해서 X방향으로만 이동하기 전에는 바닥과
	// 충돌중이었으나 X방향으로 이동함으로써 내리막길인 
	// 바닥과 순간적으로 충돌하지 않으나 아직 바닥과의 충돌함수가
	// 호출되기 전이라서 m_IsGround는 true로 되어 있어서
	// Player::Update에서 중력이 작용하는 코드도 이번 프레임에서는
	// 건너뛴다. 따라서 여기서 추가적으로 중력을 작용하는 코드를 작성해준다
	// 다만 이걸 따지는 경우는 Player와 Stage 충돌체간에만 따진다
	// Obstacle은 적용X
	if (m_IsGround)
	{
		CCollider* Bottom = FindCollider("Body");
		
		if (Bottom->CheckCollisionList("StagePixel"))
		{
			Vector2 HitPoint = Bottom->GetHitPoint();
			Vector2 Offset = Bottom->GetOffset();

			float Width = ((CColliderBox*)Bottom)->GetWidth();
			float Height = ((CColliderBox*)Bottom)->GetHeight();

			RectInfo Rect;
			Rect.Left = m_Pos.x - Width / 2.f + Offset.x;
			Rect.Top = m_Pos.y - Height / 2.f + Offset.y;
			Rect.Right = m_Pos.x + Width / 2.f + Offset.x;
			Rect.Bottom = m_Pos.y + Height / 2.f + Offset.y;

			((CColliderBox*)Bottom)->SetInfo(Rect);

			if (Rect.Left > HitPoint.x || Rect.Right < HitPoint.x)
			{
				m_FallTime += DeltaTime * m_GravityAccel * 30;

				m_Pos.y += 0.5f * GRAVITY * m_FallTime;
			}
		}
	}

	// 만약에 점프중이면 달리는 애니메이션으로 전환하면 안됨
	if (!m_Jump)
	{
		// 앉아있는 중에 왼쪽 이동키가 들어오면 왼쪽으로 기어가기
		if (m_SitDown)
		{
			ChangeTopAnimation("Blank");
			ChangeBottomAnimation("PlayerCrawlLeft");
			m_MoveSpeed = 150.f;

			Move(Vector2(-1.f, 0.f));
		}

		else
		{
			// 총을 쏘거나 폭탄을 던지는 도중에 캔슬하고
			// 움직이지 않도록 한다
			// VerticalJump중일때도 바꾸면 안되는 이유는
			// Obstacle에서 떨어질 때 VerticalJump 애니메이션으로
			// 전환되는데 그때 RunRight로 전환해버리면
			// PostUpdate에서 Idle로 전환되어버릴 수 있다.
			if (CurTop.find("NormalFire") == std::string::npos
				&& CurTop.find("Bomb") == std::string::npos 
				&& CurTop.find("VerticalJump") == std::string::npos)
			{
				ChangeTopAnimation("PlayerRunLeftTop");
			}
			
			if (CurTop.find("NormalFireRight") != std::string::npos ||
				CurTop.find("BombRight") != std::string::npos ||
				CurTop.find("VerticalJump") != std::string::npos)
				return;

			ChangeBottomAnimation("PlayerRunLeftBottom");
			m_MoveSpeed = 400.f;
			Move(Vector2(-1.f, 0.f));

		}
	}

	// 점프중이라면 FrontJump 애니메이션으로 전환
	else
	{
		// 만약 FrontJumpRight중인데 왼쪽 방향키가 들어오면
		// 애니메이션 FrontJumpLeft로 전환하지 않는다
		// 점프하면서 Fire하거나 폭탄 던지고 있는 중이라도 
		// FrontJumpLeft로 변환하지 않는다
		if (CurTop.find("FrontJumpRight") == std::string::npos &&
			CurTop.find("PlayerNormalFireLeftTop") == std::string::npos &&
			CurTop.find("BombLeftTop") == std::string::npos)
		{
			ChangeTopAnimation("PlayerFrontJumpLeftTop");
			ChangeBottomAnimation("PlayerFrontJumpLeftBottom");
		}
		Move(Vector2(-1.f, 0.f));
	}

}

void CPlayer::MoveRight(float DeltaTime)
{
	std::string CurTop = m_TopAnimation->m_CurrentAnimation->Sequence->GetName();
	std::string CurBottom = m_BottomAnimation->m_CurrentAnimation->Sequence->GetName();

	// 위의 Move로 인해서 X방향으로만 이동하기 전에는 바닥과
	// 충돌중이었으나 X방향으로 이동함으로써 내리막길인 
	// 바닥과 순간적으로 충돌하지 않으나 아직 바닥과의 충돌함수가
	// 호출되기 전이라서 m_IsGround는 true로 되어 있어서
	// Player::Update에서 중력이 작용하는 코드도 이번 프레임에서는
	// 건너뛴다. 따라서 여기서 추가적으로 중력을 작용하는 코드를 작성해준다
	// 다만 이걸 따지는 경우는 Player와 Stage 충돌체간에만 따진다
	// Obstacle은 적용X
	if (m_IsGround)
	{
		CCollider* Bottom = FindCollider("Body");

		if (Bottom->CheckCollisionList("StagePixel"))
		{
			Vector2 HitPoint = Bottom->GetHitPoint();
			Vector2 Offset = Bottom->GetOffset();

			float Width = ((CColliderBox*)Bottom)->GetWidth();
			float Height = ((CColliderBox*)Bottom)->GetHeight();

			RectInfo Rect;
			Rect.Left = m_Pos.x - Width / 2.f + Offset.x;
			Rect.Top = m_Pos.y - Height / 2.f + Offset.y;
			Rect.Right = m_Pos.x + Width / 2.f + Offset.x;
			Rect.Bottom = m_Pos.y + Height / 2.f + Offset.y;

			((CColliderBox*)Bottom)->SetInfo(Rect);

			float MidPoint = (Rect.Left + Rect.Right) / 2;

			if (MidPoint - ((CColliderBox*)Bottom)->GetWidth() / 2 > HitPoint.x)
			{
				m_FallTime += DeltaTime * m_GravityAccel * 30;

				m_Pos.y += 0.5f * GRAVITY * m_FallTime;
			}
		}
	}

	// 만약에 점프중이면 달리는 애니메이션으로 전환하면 안됨
	if (!m_Jump)
	{
		// 앉아있는 중에 오른쪽 이동키가 들어오면 오른쪽으로 기어가기
		if (m_SitDown)
		{
			ChangeTopAnimation("Blank");
			ChangeBottomAnimation("PlayerCrawlRight");
			m_MoveSpeed = 150.f;
			Move(Vector2(1.f, 0.f));
		}

		else
		{
			// 총을 쏘거나 폭탄을 던지는 도중에 캔슬하고
			// 움직이지 않도록 한다
			// VerticalJump중일때도 바꾸면 안되는 이유는
			// Obstacle에서 떨어질 때 VerticalJump 애니메이션으로
			// 전환되는데 그때 RunRight로 전환해버리면
			// PostUpdate에서 Idle로 전환되어버릴 수 있다.
			if (CurTop.find("NormalFire") == std::string::npos
				&& CurTop.find("Bomb") == std::string::npos
				&& CurTop.find("VerticalJump") == std::string::npos)
			{
				ChangeTopAnimation("PlayerRunRightTop");
			}

			if (CurTop.find("NormalFireLeft") != std::string::npos ||
				CurTop.find("BombLeft") != std::string::npos ||
				CurTop.find("VerticalJump") != std::string::npos)
				return;

			ChangeBottomAnimation("PlayerRunRightBottom");
			m_MoveSpeed = 400.f;
			Move(Vector2(1.f, 0.f));
		}
	}

	else
	{
		// FrontJumpLeft중인데 오른쪽 방향키가 들어오면
		// FrontJumpRight로 전환하지 않는다
		// 점프하면서 Fire하거나 폭탄 던지는중이라도
		// FrontJumpRight로 변환하지 않는다
		if (CurTop.find("FrontJumpLeft") == std::string::npos &&
			CurTop.find("PlayerNormalFireRightTop") == std::string::npos &&
			CurTop.find("BombRightTop") == std::string::npos)
		{
			ChangeTopAnimation("PlayerFrontJumpRightTop");
			ChangeBottomAnimation("PlayerFrontJumpRightBottom");
		}
		Move(Vector2(1.f, 0.f));
	}
}

void CPlayer::BulletFire(float DeltaTime)
{
	std::string CurTop = m_TopAnimation->m_CurrentAnimation->Sequence->GetName();
	std::string CurBottom = m_BottomAnimation->m_CurrentAnimation->Sequence->GetName();

	if (!m_IsGround && CurTop == "PlayerJumpDownRightTop")
	{
		ChangeTopAnimation("PlayerJumpAttackDownRightTop");
	}

	else if (!m_IsGround && CurTop == "PlayerJumpDownLeftTop")
	{
		ChangeTopAnimation("PlayerJumpAttackDownLeftTop");
	}

	else if (!m_IsGround && CurTop == "PlayerVerticalJumpRightTop")
	{
		ChangeTopAnimation("PlayerNormalFireRightTop");
	}

	else if (!m_IsGround && CurTop == "PlayerVerticalJumpLeftTop")
	{
		ChangeTopAnimation("PlayerNormalFireLeftTop");
	}

	else if (!m_IsGround && CurTop == "PlayerFrontJumpRightTop")
	{
		ChangeTopAnimation("PlayerNormalFireRightTop");
	}

	else if (!m_IsGround && CurTop == "PlayerFrontJumpLeftTop")
	{
		ChangeTopAnimation("PlayerNormalFireLeftTop");
	}

	// 점프한상태로 총을 발사하고 Idle로 돌아와서
	// 착지 전에 또 총을 발사할 때 들어오는 부분
	else if (!m_IsGround && CurTop == "PlayerIdleRightTop")
	{
		ChangeTopAnimation("PlayerNormalFireRightTop");
	}

	// 점프한상태로 총을 발사하고 Idle로 돌아와서
	// 착지 전에 또 총을 발사할 때 들어오는 부분
	else if (!m_IsGround && CurTop == "PlayerIdleLeftTop")
	{
		ChangeTopAnimation("PlayerNormalFireLeftTop");
	}

	else if (CurTop == "PlayerLookUpRightTop")
	{
		ChangeTopAnimation("PlayerLookUpAttackRightTop");
	}

	else if (CurTop == "PlayerLookUpLeftTop")
	{
		ChangeTopAnimation("PlayerLookUpAttackLeftTop");
	}

	else if (CurTop == "PlayerIdleRightTop" && CurBottom == "PlayerIdleRightBottom")
	{
		ChangeTopAnimation("PlayerNormalFireRightTop");
		ChangeBottomAnimation("PlayerNormalFireRightBottom");
	}

	else if (CurTop == "PlayerIdleLeftTop" && CurBottom == "PlayerIdleLeftBottom")
	{
		ChangeTopAnimation("PlayerNormalFireLeftTop");
		ChangeBottomAnimation("PlayerNormalFireLeftBottom");
	}

	else if (CurTop == "PlayerRunRightTop" && CurBottom == "PlayerRunRightBottom")
	 {
		ChangeTopAnimation("PlayerNormalFireRightTop");
	 }

	else if (CurTop == "PlayerRunLeftTop" && CurBottom == "PlayerRunLeftBottom")
	 {
		ChangeTopAnimation("PlayerNormalFireLeftTop");
	 }

	else if (CurBottom == "PlayerSitDownIdleRight")
	{
		ChangeBottomAnimation("SitDownNormalAttackRight");
	}

	else if (CurBottom == "PlayerSitDownIdleLeft")
	{
		ChangeBottomAnimation("SitDownNormalAttackLeft");
	}
}

void CPlayer::Pause(float DeltaTime)
{
	CGameManager::GetInst()->SetTimeScale(0.f);
}

void CPlayer::Resume(float DeltaTime)
{
	CGameManager::GetInst()->SetTimeScale(1.f);
}

void CPlayer::CloneBullet()
{
	// MainScene::Init에서 만들어놓았던 Prototype을 찾아서
	// 그 Prototype의 정보를 복사해서 실제로 Scene의 
	// m_ObjList에 넣어줘서 Scene에 배치한다
	CSharedPtr<CBullet> Bullet = m_Scene->CreateObject<CBullet>(
		"Bullet", "PlayerBullet",
		Vector2(0.f, 0.f), Vector2(20.f, 20.f));

	Bullet->SetZOrder(GetZOrder());

	std::string CurTop = m_TopAnimation->m_CurrentAnimation->Sequence->GetName();
	std::string CurBottom = m_BottomAnimation->m_CurrentAnimation->Sequence->GetName();

	if (m_SitDown)
	{
		if (CurBottom.find("NormalAttackRight") != std::string::npos)
		{
			Bullet->SetPos(m_Pos + Vector2(5.f, -40.f));
			Bullet->SetDir(1.f, 0.f);
		}

		else if (CurBottom.find("NormalAttackLeft") != std::string::npos)
		{
			Bullet->SetPos(m_Pos + Vector2(-5.f, -40.f));
			Bullet->SetDir(-1.f, 0.f);
		}
	}

	else
	{
		if (CurTop.find("Down") != std::string::npos)
		{
			Bullet->SetPos(m_Pos + Vector2(5.f,
				PLAYER_BOTTOMHEIGHT + 10.f));
			Bullet->SetDir(0.f, 1.f);
		}

		else if (CurTop.find("LookUp") != std::string::npos)
		{
			Bullet->SetPos(m_Pos - Vector2(0.f, PLAYER_TOPHEIGHT + 5.f));
			Bullet->SetDir(0.f, -1.f);
		}

		else if (CurTop.find("FireRight") != std::string::npos)
		{
			Bullet->SetPos(m_Pos + Vector2(PLAYER_TOPWIDTH / 2.f + 10.f,
				-(PLAYER_BOTTOMHEIGHT + PLAYER_TOPHEIGHT / 2.f) + 10.f));
			Bullet->SetDir(1.f, 0.f);
		}

		else if (CurTop.find("FireLeft") != std::string::npos)
		{
			Bullet->SetPos(m_Pos + Vector2(-PLAYER_TOPWIDTH / 2.f - 10.f,
				-(PLAYER_BOTTOMHEIGHT + PLAYER_TOPHEIGHT / 2.f) + 10.f));
			Bullet->SetDir(-1.f, 0.f);
		}
	}

	Bullet->SetTextureColorKey(255, 255, 255);

	m_Scene->GetSceneResource()->SoundPlay("NormalAttack");
}

void CPlayer::CloneBomb()
{
	CSharedPtr<CBullet> Bomb = m_Scene->CreateObject<CBullet>(
		"Bomb", "PlayerBomb",
		Vector2(0.f, 0.f), Vector2(59.f, 60.f));

	Bomb->SetZOrder(GetZOrder());

	std::string CurTop = m_TopAnimation->m_CurrentAnimation->Sequence->GetName();
	std::string CurBottom = m_BottomAnimation->m_CurrentAnimation->Sequence->GetName();

	if (!m_SitDown)
	{
		if (CurTop.find("Right") != std::string::npos)
		{

			Bomb->SetPos(m_Pos + Vector2(5.f, -PLAYER_TOPHEIGHT - 5.f));
			Bomb->SetDir(1.f, -1.f);
		}

		else if (CurTop.find("Left") != std::string::npos)
		{
			Bomb->SetPos(m_Pos + Vector2(-5.f, -PLAYER_TOPHEIGHT - 5.f));
			Bomb->SetDir(-1.f, -1.f);
		}
	}

	else
	{
		if (CurBottom.find("Right") != std::string::npos)
		{
			Bomb->SetPos(m_Pos + Vector2(5.f, -30.f));
			Bomb->SetDir(1.f, -1.f);
		}

		else if (CurBottom.find("Left") != std::string::npos)
		{
			Bomb->SetPos(m_Pos + Vector2(-5.f, -30.f));
			Bomb->SetDir(-1.f, -1.f);
		}
	}
}

void CPlayer::CreateTopAnimation()
{
	if (!m_TopAnimation)
	{
		m_TopAnimation = new CAnimation;

		m_TopAnimation->m_Owner = this;
		m_TopAnimation->m_Scene = m_Scene;
	}
}

void CPlayer::CreateBottomAnimation()
{
	if (!m_BottomAnimation)
	{
		m_BottomAnimation = new CAnimation;

		m_BottomAnimation->m_Owner = this;
		m_BottomAnimation->m_Scene = m_Scene;
	}
}

void CPlayer::AddTopAnimation(const std::string& SequenceName, bool Loop, float PlayTime, float PlayScale, bool Reverse)
{
	if (!m_TopAnimation)
	{
		m_TopAnimation = new CAnimation;

		m_TopAnimation->m_Owner = this;
		m_TopAnimation->m_Scene = m_Scene;
	}

	m_TopAnimation->AddAnimation(SequenceName, Loop, PlayTime, PlayScale, Reverse);
}

void CPlayer::AddBottomAnimation(const std::string& SequenceName, bool Loop, float PlayTime, float PlayScale, bool Reverse)
{
	if (!m_BottomAnimation)
	{
		m_BottomAnimation = new CAnimation;

		m_BottomAnimation->m_Owner = this;
		m_BottomAnimation->m_Scene = m_Scene;
	}

	m_BottomAnimation->AddAnimation(SequenceName, Loop, PlayTime, PlayScale, Reverse);
}

void CPlayer::SetTopAnimationPlayTime(const std::string& Name, float PlayTime)
{
	if (!m_TopAnimation)
		return;

	m_TopAnimation->SetPlayTime(Name, PlayTime);
}

void CPlayer::SetBottomAnimationPlayTime(const std::string& Name, float PlayTime)
{
	if (!m_BottomAnimation)
		return;

	m_BottomAnimation->SetPlayTime(Name, PlayTime);
}

void CPlayer::SetTopAnimationPlayScale(const std::string& Name, float PlayScale)
{
	if (!m_TopAnimation)
		return;

	m_TopAnimation->SetPlayScale(Name, PlayScale);
}

void CPlayer::SetBottomAnimationPlayScale(const std::string& Name, float PlayScale)
{
	if (!m_BottomAnimation)
		return;

	m_BottomAnimation->SetPlayScale(Name, PlayScale);
}

void CPlayer::SetTopCurrentAnimation(const std::string& Name)
{
	if (!m_TopAnimation)
		return;

	m_TopAnimation->SetCurrentAnimation(Name);
}

void CPlayer::SetBottomCurrentAnimation(const std::string& Name)
{
	if (!m_BottomAnimation)
		return;

	ChangeBottomAnimation(Name);
}

void CPlayer::ChangeTopAnimation(const std::string& Name)
{
	if (!m_TopAnimation)
		return;

	m_TopAnimation->ChangeAnimation(Name);
}

void CPlayer::ChangeBottomAnimation(const std::string& Name)
{
	if (!m_BottomAnimation)
		return;

	m_BottomAnimation->ChangeAnimation(Name);
}

bool CPlayer::CheckTopCurrentAnimation(const std::string& Name)
{
	return m_TopAnimation->CheckCurrentAnimation(Name);
}

bool CPlayer::CheckBottomCurrentAnimation(const std::string& Name)
{
	return m_BottomAnimation->CheckCurrentAnimation(Name);
}

void CPlayer::SetTopAnimationReverse(const std::string& Name, bool Reverse)
{
	if (!m_TopAnimation)
		return;

	m_TopAnimation->SetReverse(Name, Reverse);
}

void CPlayer::SetBottomAnimationReverse(const std::string& Name, bool Reverse)
{
	if (!m_BottomAnimation)
		return;

	m_BottomAnimation->SetReverse(Name, Reverse);
}

void CPlayer::SetTopAnimationLoop(const std::string& Name, bool Loop)
{
	if (!m_TopAnimation)
		return;

	m_TopAnimation->SetLoop(Name, Loop);
}

void CPlayer::SetBottomAnimationLoop(const std::string& Name, bool Loop)
{
	if (!m_BottomAnimation)
		return;

	m_BottomAnimation->SetLoop(Name, Loop);
}

void CPlayer::TopAttackEnd()
{
	std::string CurTop = m_TopAnimation->m_CurrentAnimation->Sequence->GetName();

	if (CurTop == "PlayerJumpAttackDownRightTop")
	{
		ChangeTopAnimation("PlayerJumpDownRightTop");
	}

	else if (CurTop == "PlayerJumpAttackDownLeftTop")
	{
		ChangeTopAnimation("PlayerJumpDownLeftTop");
	}

	else if (CurTop == "PlayerNormalFireRightTop")
	{
		ChangeTopAnimation("PlayerIdleRightTop");
	}

	else if (CurTop == "PlayerNormalFireLeftTop")
	{
		ChangeTopAnimation("PlayerIdleLeftTop");
	}

	else if (CurTop == "PlayerLookUpAttackRightTop")
	{
		ChangeTopAnimation("PlayerLookUpRightTop");
	}

	else if (CurTop == "PlayerLookUpAttackLeftTop")
	{
		ChangeTopAnimation("PlayerLookUpLeftTop");
	}
}

void CPlayer::BottomAttackEnd()
{
	if (m_BottomAnimation->m_CurrentAnimation->Sequence->GetName()
		== "PlayerNormalFireRightBottom")
		ChangeBottomAnimation("PlayerIdleRightBottom");

	else if (m_BottomAnimation->m_CurrentAnimation->Sequence->GetName()
		== "PlayerNormalFireLeftBottom")
		ChangeBottomAnimation("PlayerIdleLeftBottom");
}

void CPlayer::SitDownBottomAttackEnd()
{
	std::string BottomAni = m_BottomAnimation->m_CurrentAnimation->Sequence->GetName();
	if (BottomAni.find("Right") != std::string::npos)
	{
		ChangeBottomAnimation("PlayerSitDownIdleRight");
	}

	else if (BottomAni.find("Left") != std::string::npos)
	{
		ChangeBottomAnimation("PlayerSitDownIdleLeft");
	}
}

void CPlayer::Bomb(float DeltaTime)
{
	std::string CurTop = m_TopAnimation->m_CurrentAnimation->Sequence->GetName();
	std::string CurBottom = m_BottomAnimation->m_CurrentAnimation->Sequence->GetName();

	if (!m_SitDown)
	{
		if (CurTop.find("Right") != std::string::npos)
		{
			ChangeTopAnimation("BombRightTop");
		}

		else if (CurTop.find("Left") != std::string::npos)
		{
			ChangeTopAnimation("BombLeftTop");
		}
	}

	else
	{
		if (CurBottom.find("Right") != std::string::npos)
		{
			ChangeBottomAnimation("PlayerSitDownBombRight");
		}

		else if (CurBottom.find("Left") != std::string::npos)
		{
			ChangeBottomAnimation("PlayerSitDownBombLeft");
		}
	}
}

void CPlayer::BombEnd()
{
	std::string CurTop = m_TopAnimation->m_CurrentAnimation->Sequence->GetName();

	if (CurTop.find("Right") != std::string::npos)
	{
		ChangeTopAnimation("PlayerIdleRightTop");
	}

	else if (CurTop.find("Left") != std::string::npos)
	{
		ChangeTopAnimation("PlayerIdleLeftTop");
	}
}

void CPlayer::SitDownBombEnd()
{
	std::string CurBottom = m_BottomAnimation->m_CurrentAnimation->Sequence->GetName();

	if (CurBottom.find("Right") != std::string::npos)
	{
		ChangeBottomAnimation("PlayerSitDownIdleRight");
	}

	else if (CurBottom.find("Left") != std::string::npos)
	{
		ChangeBottomAnimation("PlayerSitDownIdleLeft");
	}
}

void CPlayer::CollisionBegin(CCollider* Src, CCollider* Dest, float DeltaTime)
{
	if (Dest->GetName() == "StagePixel")
	{

		bool FloorCollision = ((CColliderBox*)Src)->GetFloorCollision();
		bool CamCollision = ((CColliderBox*)Src)->GetCamCollision();

		// 바닥, 카메라 충돌체에 모두 충돌했을 경우
		if (FloorCollision && CamCollision)
		{
			// m_PhysicsSimulate = false;
			m_IsGround = true;

			m_Jump = false;
			// 카메라 충돌체와 닿았다면 바닥과도 닿아있을 것이므로
			// 바닥과의 Hit Point를 찾아서 CollisionRectToPixel에서 
			// 갱신해놓았을 것이다. 따라서 여기서 그 바닥과의 Hit Point로
			// player의 y좌표를 계속 갱신해서 카메라를 고정함과 동시에
			// player를 바닥에 붙어있게 만든다
			m_Pos.y = Src->GetHitPoint().y;
			m_MoveSpeed = 400.f;
			m_ObstacleFall = false;

			// 첫번째 카메라 충돌체에 부딪히면(Camel_Arabian 등장)
			// 죽이기 전까지는 카메라 x축 이동 고정
			//CCamera::GetInst()->SetXFix(true);
			//CCamera::GetInst()->SetYFix(true);
		}

		// 바닥 충돌체에만 충돌한 경우
		else if (FloorCollision)
		{
			m_IsGround = true;
			m_Jump = false;

			m_FallTime = 0.f;

			m_Pos.y = Src->GetHitPoint().y;
			m_FallStartY = m_Pos.y;
			m_JumpVelocity = 0.f;
			m_MoveSpeed = 400.f;
			m_ObstacleFall = false;

			// 지면이랑 닿는 순간 한번만 Idle 애니메이션으로 복귀
			std::string CurTop = m_TopAnimation->m_CurrentAnimation->Sequence->GetName();
			std::string CurBottom = m_BottomAnimation->m_CurrentAnimation->Sequence->GetName();

			if (!m_SitDown && CurBottom.find("Run") == std::string::npos &&
				CurBottom.find("Fire") == std::string::npos &&
				CurBottom.find("Crawl") == std::string::npos &&
				CurBottom.find("Right") != std::string::npos)
			{
				ChangeTopAnimation("PlayerIdleRightTop");
				ChangeBottomAnimation("PlayerIdleRightBottom");

				CCollider* Head = FindCollider("Head");
				Head->SetEnable(true);
			}

			else if (!m_SitDown && CurBottom.find("Run") == std::string::npos &&
				CurBottom.find("Fire") == std::string::npos &&
				CurBottom.find("Crawl") == std::string::npos &&
				CurBottom.find("Left") != std::string::npos)
			{
				ChangeTopAnimation("PlayerIdleLeftTop");
				ChangeBottomAnimation("PlayerIdleLeftBottom");

				CCollider* Head = FindCollider("Head");
				Head->SetEnable(true);
			}
		}
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
		m_ObstacleFall = false;

		// 지면이랑 닿는 순간 한번만 Idle 애니메이션으로 복귀
		std::string CurTop = m_TopAnimation->m_CurrentAnimation->Sequence->GetName();
		std::string CurBottom = m_BottomAnimation->m_CurrentAnimation->Sequence->GetName();

		if (!m_SitDown && CurBottom.find("Run") == std::string::npos &&
			CurBottom.find("Fire") == std::string::npos &&
			CurBottom.find("Crawl") == std::string::npos &&
			CurBottom.find("Right") != std::string::npos)
		{
			ChangeTopAnimation("PlayerIdleRightTop");
			ChangeBottomAnimation("PlayerIdleRightBottom");

			CCollider* Head = FindCollider("Head");
			Head->SetEnable(true);
		}

		else if (!m_SitDown && CurBottom.find("Run") == std::string::npos &&
			CurBottom.find("Fire") == std::string::npos &&
			CurBottom.find("Crawl") == std::string::npos &&
			CurBottom.find("Left") != std::string::npos)
		{
			ChangeTopAnimation("PlayerIdleLeftTop");
			ChangeBottomAnimation("PlayerIdleLeftBottom");

			CCollider* Head = FindCollider("Head");
			Head->SetEnable(true);
		}
	}		
}

void CPlayer::CollisionStay(CCollider* Src, CCollider* Dest, float DeltaTime)
{
	if (Dest->GetName() == "StagePixel")
	{

		bool FloorCollision = ((CColliderBox*)Src)->GetFloorCollision();
		bool CamCollision = ((CColliderBox*)Src)->GetCamCollision();

		// 바닥, 카메라 충돌체에 모두 충돌했을 경우
		if (FloorCollision && CamCollision)
		{
			m_IsGround = true;
			m_Jump = false;

			m_FallTime = 0.f;
			// 카메라 충돌체와 닿았다면 바닥과도 닿아있을 것이므로
			// 바닥과의 Hit Point를 찾아서 CollisionRectToPixel에서 
			// 갱신해놓았을 것이다. 따라서 여기서 그 바닥과의 Hit Point로
			// player의 y좌표를 계속 갱신해서 카메라를 고정함과 동시에
			// player를 바닥에 붙어있게 만든다
			m_Pos.y = Src->GetHitPoint().y;

			// 첫번째 카메라 충돌체에 부딪히면(Camel_Arabian 등장)
			// 죽이기 전까지는 카메라 x축 이동 고정
			//CCamera::GetInst()->SetXFix(true);
			//CCamera::GetInst()->SetYFix(true);
		}

		// 바닥 충돌체에만 충돌한 경우
		else if (FloorCollision)
		{

			std::string CurTop = m_TopAnimation->m_CurrentAnimation->Sequence->GetName();
			std::string CurBottom = m_BottomAnimation->m_CurrentAnimation->Sequence->GetName();

			m_IsGround = true;
			m_Jump = false;

			m_FallTime = 0.f;

			m_Pos.y = Src->GetHitPoint().y;
			m_FallStartY = m_Pos.y;
			m_JumpVelocity = 0.f;

		}
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

		// 지면이랑 닿는 순간 한번만 Idle 애니메이션으로 복귀
		std::string CurTop = m_TopAnimation->m_CurrentAnimation->Sequence->GetName();
		std::string CurBottom = m_BottomAnimation->m_CurrentAnimation->Sequence->GetName();

		if (!m_SitDown && CurBottom.find("Run") == std::string::npos &&
			CurBottom.find("Fire") == std::string::npos &&
			CurBottom.find("Crawl") == std::string::npos &&
			CurBottom.find("Right") != std::string::npos)
		{
			ChangeTopAnimation("PlayerIdleRightTop");
			ChangeBottomAnimation("PlayerIdleRightBottom");

			CCollider* Head = FindCollider("Head");
			Head->SetEnable(true);
		}

		else if (!m_SitDown && CurBottom.find("Run") == std::string::npos &&
			CurBottom.find("Fire") == std::string::npos &&
			CurBottom.find("Crawl") == std::string::npos &&
			CurBottom.find("Left") != std::string::npos)
		{
			ChangeTopAnimation("PlayerIdleLeftTop");
			ChangeBottomAnimation("PlayerIdleLeftBottom");

			CCollider* Head = FindCollider("Head");
			Head->SetEnable(true);
		}
	}
}

void CPlayer::CollisionEnd(CCollider* Src, CCollider* Dest, float DeltaTime)
{
	if ((Dest->GetName() == "BackObstaclePixel") ||
		(Dest->GetName() == "FrontObstaclePixel"))
	{
		m_IsGround = false;

		std::string CurTop = m_TopAnimation->m_CurrentAnimation->Sequence->GetName();
		std::string CurBottom = m_BottomAnimation->m_CurrentAnimation->Sequence->GetName();

		if (CurTop.find("Right") != std::string::npos)
		{
			ChangeTopAnimation("PlayerVerticalJumpRightTop");
			ChangeBottomAnimation("PlayerVerticalJumpRightBottom");

			// 점프하면서 지형지물과 충돌이 끝나는게 아니라
			// 떨어짐으로써 지형지물과 충돌이 끝나는 경우	
			// 앞으로 살짝 나아가면서 착지한다
			if (!m_Jump)
			{
				m_ObstacleFall = true;
			}
		}

		else if (CurTop.find("Left") != std::string::npos)
		{
			ChangeTopAnimation("PlayerVerticalJumpLeftTop");
			ChangeBottomAnimation("PlayerVerticalJumpLeftBottom");

			if (!m_Jump)
			{
				m_ObstacleFall = true;
			}
		}
	}
}

