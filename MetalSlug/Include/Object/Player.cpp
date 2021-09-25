
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
	m_BottomAnimation(nullptr)
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

	// ���������� key�� �ݹ��Լ����� �ִϸ��̼��� �ٲ��ִ�
	// �뵵�̰�, �ִϸ��̼� ������ ������ �Ѿ˳�����,
	// ���� Idle�� ���ƿ��� �̷� �͵��� Notify�� �����س���
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

	CInput::GetInst()->SetCallback<CPlayer>("Pause",
		KeyState_Down, this, &CPlayer::Pause);

	CInput::GetInst()->SetCallback<CPlayer>("Resume",
		KeyState_Down, this, &CPlayer::Resume);

}

bool CPlayer::Init()
{
	if (!CCharacter::Init())
		return false;

	m_MoveSpeed = 400.f;
	// �Ǻ��� ������� ���� ��´�
	SetPivot(0.5f, 1.f);

	CreateTopAnimation();
	CreateBottomAnimation();
	AddTopAnimation("PlayerIdleRightTop", true, 0.8f);
	AddBottomAnimation("PlayerIdleRightBottom", true, 0.8f);
	AddTopAnimation("PlayerIdleLeftTop", true, 0.8f, 1.f, true);
	AddBottomAnimation("PlayerIdleLeftBottom", true, 0.8f, 1.f, true);

	AddTopAnimation("PlayerNormalFireRightTop", false, 0.2f);
	AddBottomAnimation("PlayerNormalFireRightBottom", false, 0.2f);
	AddTopAnimation("PlayerNormalFireLeftTop", false, 0.2f, 1.f, true);
	AddBottomAnimation("PlayerNormalFireLeftBottom", false, 0.2f, 1.f, true);
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

	AddTopAnimation("PlayerJumpAttackDownRightTop", false, 0.2f);
	AddTopAnimation("PlayerJumpAttackDownLeftTop", false, 0.2f, 1.f, true);

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

	AddTopAnimation("PlayerLookUpAttackRightTop", false, 0.2f);
	AddTopAnimation("PlayerLookUpAttackLeftTop", false, 0.2f, 1.f, true);

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


	CColliderSphere* Head = AddCollider<CColliderSphere>("Head");
	// Head->SetExtent(40.f, 30.f);
	Head->SetRadius(20.f);
	Head->SetOffset(0.f, -60.f);
	Head->SetCollisionProfile("Player");

	CColliderBox* Body = AddCollider<CColliderBox>("Body");
	Body->SetExtent(80.f, 45.f);
	Body->SetOffset(0.f, -22.5f);
	Body->SetCollisionProfile("Player");
	Body->SetCollisionBeginFunction<CPlayer>(this, &CPlayer::CollisionBegin);
	Body->SetCollisionStayFunction<CPlayer>(this, &CPlayer::CollisionStay);

	m_PhysicsSimulate = true;
	m_IsGround = false;
	SetJumpVelocity(50.f);

	return true;
}

void CPlayer::Update(float DeltaTime)
{
	m_IsGround = false;

	if (!m_Start)
	{
		Start();
	}

	if (m_TopAnimation)
		m_TopAnimation->Update(DeltaTime);

	if (m_BottomAnimation)
		m_BottomAnimation->Update(DeltaTime);


	// �߷��� �����Ѵ�.
	if (!m_IsGround && m_PhysicsSimulate)
	{
		// �������� �ð��� ���������ش�.
		m_FallTime += DeltaTime * m_GravityAccel;

		float	Velocity = 0.f;

		if (m_Jump)
			Velocity = m_JumpVelocity * m_FallTime;

		m_Pos.y = m_FallStartY - (Velocity - 0.5f * GRAVITY * m_FallTime * m_FallTime);
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

		// ��Ȱ��ȭ �Ǿ��ִ� �浹ü�� Update���� �ʴ´�
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

	// ���� �ִϸ��̼��� Walk�ε� �ӵ��� 0�̴ٴ� ����
	// ����ٴ� �ǹ��̴�.


	std::string CurBottomAnimation = m_BottomAnimation->m_CurrentAnimation->Sequence->GetName();
	std::string CurTopAnimation = m_TopAnimation->m_CurrentAnimation->Sequence->GetName();

	if (CurBottomAnimation.find("Run") != std::string::npos)
	{
		if (m_Velocity.Length() == 0.f)
		{
			if (CurBottomAnimation.find("Right") != std::string::npos)
			{
				m_TopAnimation->ChangeAnimation("PlayerIdleRightTop");
				m_BottomAnimation->ChangeAnimation("PlayerIdleRightBottom");
			}

			else
			{
				m_TopAnimation->ChangeAnimation("PlayerIdleLeftTop");
				m_BottomAnimation->ChangeAnimation("PlayerIdleLeftBottom");
			}
		}
	}

	if (CurTopAnimation.find("LookUp") != std::string::npos)
	{
		if (!(GetAsyncKeyState('W') & 0x8000))
		{
			if (CurBottomAnimation.find("Right") != std::string::npos)
			{
				m_TopAnimation->ChangeAnimation("PlayerIdleRightTop");
				m_BottomAnimation->ChangeAnimation("PlayerIdleRightBottom");
			}

			else
			{
				m_TopAnimation->ChangeAnimation("PlayerIdleLeftTop");
				m_BottomAnimation->ChangeAnimation("PlayerIdleLeftBottom");
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

		// ��Ȱ��ȭ �Ǿ��ִ� �浹ü�� PostUpdate���� �ʴ´�
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

		// CAnimation::Update���� m_CurrentAnimation�� Frame ��ȣ��
		// �ð��� ������ ���� ��� update���ֹǷ�, GetFrameData���ڿ���
		// ������ �´� Frame��ȣ�� �Ѱ��� ���̰�, GetFrameData��
		// CAnimationSequence�� m_vecFrameData���� Frame��ȣ�� �´�
		// FrameData�� �Ѱ��� ���̴�
		const AnimationFrameData& FrameData =
			AnimInfo->Sequence->GetFrameData(AnimInfo->Frame);

		Vector2 LT = m_RenderPos - m_Pivot * FrameData.Size + m_Offset;

		if (AnimInfo->Sequence->GetTextureType() == ETexture_Type::Atlas)
		{
			AnimInfo->Sequence->GetTexture()->Render(hDC, LT,
				FrameData.StartPos, FrameData.Size);
		}

		// Frame���
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

		// CAnimation::Update���� m_CurrentAnimation�� Frame ��ȣ��
		// �ð��� ������ ���� ��� update���ֹǷ�, GetFrameData���ڿ���
		// ������ �´� Frame��ȣ�� �Ѱ��� ���̰�, GetFrameData��
		// CAnimationSequence�� m_vecFrameData���� Frame��ȣ�� �´�
		// FrameData�� �Ѱ��� ���̴�
		const AnimationFrameData& FrameData =
			AnimInfo->Sequence->GetFrameData(AnimInfo->Frame);

		Vector2 LT = m_RenderPos - m_Pivot * FrameData.Size + FrameData.Offset + m_Offset;
		LT.y -= PLAYER_BOTTOMHEIGHT;

		if (AnimInfo->Sequence->GetTextureType() == ETexture_Type::Atlas)
		{
			AnimInfo->Sequence->GetTexture()->Render(hDC, LT,
				FrameData.StartPos, FrameData.Size);
		}

		// Frame���
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

	// ����׸���� �浹ü ����� �׷��� ����
	// #ifdef _DEBUG���� define�� Collider�� �ڽ� Ŭ��������
	// Render�Լ��� ���ǵǾ��� ����
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

	//wchar_t StrPos_x[32] = {};
	//wchar_t StrPos_y[32] = {};
	//wsprintf(StrPos_x, L"x : %d", (int)m_Pos.x);
	//wsprintf(StrPos_y, L"y : %d", (int)m_Pos.y);
	////m_Pos -= m_Scene->GetCamera()->GetPos();
	//TextOut(hDC, (int)m_RenderPos.x - 10, (int)m_RenderPos.y - 150, StrPos_x, lstrlen(StrPos_x));
	//TextOut(hDC, (int)m_RenderPos.x - 10, (int)m_RenderPos.y - 130, StrPos_y, lstrlen(StrPos_y));

	m_PrevPos = m_Pos;
	m_Velocity = Vector2(0.f, 0.f);
}

CPlayer* CPlayer::Clone()
{
	return new CPlayer(*this);
}

void CPlayer::Jump(float DeltaTime)
{
	//m_Pos.y -= 200.f * DeltaTime;
	CGameObject::Jump();
	m_JumpVelocity = 60.f;
	Move(Vector2(0.f, -1.f));
	std::string CurTop = m_TopAnimation->m_CurrentAnimation->Sequence->GetName();
	std::string CurBottom = m_BottomAnimation->m_CurrentAnimation->Sequence->GetName();

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
}

void CPlayer::Down(float DeltaTime)
{
	Move(Vector2(0.f, 1.f));
	//ChangeAnimation("LucidNunNaRightWalk");
	std::string CurTop = m_TopAnimation->m_CurrentAnimation->Sequence->GetName();

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
		}

	}
}

void CPlayer::LookUp(float DeltaTime)
{
	std::string CurTop = m_TopAnimation->m_CurrentAnimation->Sequence->GetName();

	// ���� �������� �ƴҶ��� LookUp����
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
	Move(Vector2(-1.f, 0.f));
	// ���࿡ �������̸� �޸��� �ִϸ��̼����� ��ȯ�ϸ� �ȵ�
	if (!m_Jump)
	{
		if (m_TopAnimation->m_CurrentAnimation->Sequence->GetName() != "PlayerNormalFireLeftTop")
		{
			ChangeTopAnimation("PlayerRunLeftTop");
		}

		ChangeBottomAnimation("PlayerRunLeftBottom");
	}
}

void CPlayer::MoveRight(float DeltaTime)
{
	Move(Vector2(1.f, 0.f));

	// ���࿡ �������̸� �޸��� �ִϸ��̼����� ��ȯ�ϸ� �ȵ�
	if (!m_Jump)
	{
		if (m_TopAnimation->m_CurrentAnimation->Sequence->GetName() != "PlayerNormalFireRightTop")
		{
			ChangeTopAnimation("PlayerRunRightTop");
		}

		ChangeBottomAnimation("PlayerRunRightBottom");
	}
}

void CPlayer::BulletFire(float DeltaTime)
{
	std::string CurTop = m_TopAnimation->m_CurrentAnimation->Sequence->GetName();
	std::string CurBottom = m_BottomAnimation->m_CurrentAnimation->Sequence->GetName();

	if (!m_IsGround && CurTop == "PlayerJumpDownRightTop")
	{
		m_TopAnimation->ChangeAnimation("PlayerJumpAttackDownRightTop");
	}

	else if (!m_IsGround && CurTop == "PlayerJumpDownLeftTop")
	{
		m_TopAnimation->ChangeAnimation("PlayerJumpAttackDownLeftTop");
	}

	else if (CurTop == "PlayerLookUpRightTop")
	{
		m_TopAnimation->ChangeAnimation("PlayerLookUpAttackRightTop");
	}

	else if (CurTop == "PlayerLookUpLeftTop")
	{
		m_TopAnimation->ChangeAnimation("PlayerLookUpAttackLeftTop");
	}

	else if (CurTop == "PlayerIdleRightTop" && CurBottom == "PlayerIdleRightBottom")
	{
		m_TopAnimation->ChangeAnimation("PlayerNormalFireRightTop");
		m_BottomAnimation->ChangeAnimation("PlayerNormalFireRightBottom");
	}

	else if (CurTop == "PlayerIdleLeftTop" && CurBottom == "PlayerIdleLeftBottom")
	{
		m_TopAnimation->ChangeAnimation("PlayerNormalFireLeftTop");
		m_BottomAnimation->ChangeAnimation("PlayerNormalFireLeftBottom");
	}

	else if (CurTop == "PlayerRunRightTop" && CurBottom == "PlayerRunRightBottom")
	 {
		 m_TopAnimation->ChangeAnimation("PlayerNormalFireRightTop");
	 }

	else if (CurTop == "PlayerRunLeftTop" && CurBottom == "PlayerRunLeftBottom")
	 {
		 m_TopAnimation->ChangeAnimation("PlayerNormalFireLeftTop");
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
	// MainScene::Init���� �������Ҵ� Prototype�� ã�Ƽ�
	// �� Prototype�� ������ �����ؼ� ������ Scene�� 
	// m_ObjList�� �־��༭ Scene�� ��ġ�Ѵ�
	CSharedPtr<CBullet> Bullet = m_Scene->CreateObject<CBullet>(
		"Bullet", "PlayerBullet",
		Vector2(0.f, 0.f), Vector2(20.f, 20.f));

	std::string CurTop = m_TopAnimation->m_CurrentAnimation->Sequence->GetName();

	if (CurTop.find("Down") != std::string::npos)
	{
		Bullet->SetPos(m_Pos + Vector2(5.f,
			PLAYER_BOTTOMHEIGHT  + 10.f));
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


	Bullet->SetTextureColorKey(255, 255, 255);

	m_Scene->GetSceneResource()->SoundPlay("NormalAttack");
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

	m_BottomAnimation->SetCurrentAnimation(Name);
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
		m_TopAnimation->ChangeAnimation("PlayerJumpDownRightTop");
	}

	else if (CurTop == "PlayerJumpAttackDownLeftTop")
	{
		m_TopAnimation->ChangeAnimation("PlayerJumpDownLeftTop");
	}

	else if (CurTop == "PlayerNormalFireRightTop")
	{
		m_TopAnimation->ChangeAnimation("PlayerIdleRightTop");
	}

	else if (CurTop == "PlayerNormalFireLeftTop")
	{
		m_TopAnimation->ChangeAnimation("PlayerIdleLeftTop");
	}

	else if (CurTop == "PlayerLookUpAttackRightTop")
	{
		m_TopAnimation->ChangeAnimation("PlayerLookUpRightTop");
	}

	else if (CurTop == "PlayerLookUpAttackLeftTop")
	{
		m_TopAnimation->ChangeAnimation("PlayerLookUpLeftTop");
	}
}

void CPlayer::BottomAttackEnd()
{
	if (m_BottomAnimation->m_CurrentAnimation->Sequence->GetName()
		== "PlayerNormalFireRightBottom")
		m_BottomAnimation->ChangeAnimation("PlayerIdleRightBottom");

	else if (m_BottomAnimation->m_CurrentAnimation->Sequence->GetName()
		== "PlayerNormalFireLeftBottom")
		m_BottomAnimation->ChangeAnimation("PlayerIdleLeftBottom");
}

void CPlayer::CollisionBegin(CCollider* Src, CCollider* Dest, float DeltaTime)
{
	if (Dest->GetName() == "StagePixel")
	{

		bool FloorCollision = ((CColliderBox*)Src)->GetFloorCollision();
		bool CamCollision = ((CColliderBox*)Src)->GetCamCollision();

		// �ٴ�, ī�޶� �浹ü�� ��� �浹���� ���
		if (FloorCollision && CamCollision)
		{
			// m_PhysicsSimulate = false;
			m_IsGround = true;

			m_Jump = false;
			// ī�޶� �浹ü�� ��Ҵٸ� �ٴڰ��� ������� ���̹Ƿ�
			// �ٴڰ��� Hit Point�� ã�Ƽ� CollisionRectToPixel���� 
			// �����س����� ���̴�. ���� ���⼭ �� �ٴڰ��� Hit Point��
			// player�� y��ǥ�� ��� �����ؼ� ī�޶� �����԰� ���ÿ�
			// player�� �ٴڿ� �پ��ְ� �����
			m_Pos.y = Src->GetHitPoint().y;

			// ù��° ī�޶� �浹ü�� �ε�����(Camel_Arabian ����)
			// ���̱� �������� ī�޶� x�� �̵� ����
			//CCamera::GetInst()->SetXFix(true);
			//CCamera::GetInst()->SetYFix(true);
		}

		// �ٴ� �浹ü���� �浹�� ���
		else if (FloorCollision)
		{
			m_IsGround = true;
			m_Jump = false;

			m_FallTime = 0.f;

			m_Pos.y = Src->GetHitPoint().y;
			m_FallStartY = m_Pos.y;
			m_JumpVelocity = 0.f;

			// �����̶� ��� ���� �ѹ��� Idle �ִϸ��̼����� ����
			std::string CurTop = m_TopAnimation->m_CurrentAnimation->Sequence->GetName();
			std::string CurBottom = m_BottomAnimation->m_CurrentAnimation->Sequence->GetName();

			if (CurTop.find("Run") == std::string::npos &&
				CurTop.find("Fire") == std::string::npos &&
				CurTop.find("Right") != std::string::npos)
			{
				ChangeTopAnimation("PlayerIdleRightTop");
				ChangeBottomAnimation("PlayerIdleRightBottom");
			}

			if (CurBottom.find("Run") == std::string::npos &&
				CurBottom.find("Fire") == std::string::npos &&
				CurBottom.find("Left") != std::string::npos)
			{
				ChangeTopAnimation("PlayerIdleLeftTop");
				ChangeBottomAnimation("PlayerIdleLeftBottom");
			}
		}
	}
}

void CPlayer::CollisionStay(CCollider* Src, CCollider* Dest, float DeltaTime)
{
	if (Dest->GetName() == "StagePixel")
	{

		bool FloorCollision = ((CColliderBox*)Src)->GetFloorCollision();
		bool CamCollision = ((CColliderBox*)Src)->GetCamCollision();

		// �ٴ�, ī�޶� �浹ü�� ��� �浹���� ���
		if (FloorCollision && CamCollision)
		{
			m_IsGround = true;
			m_Jump = false;

			m_FallTime = 0.f;
			// ī�޶� �浹ü�� ��Ҵٸ� �ٴڰ��� ������� ���̹Ƿ�
			// �ٴڰ��� Hit Point�� ã�Ƽ� CollisionRectToPixel���� 
			// �����س����� ���̴�. ���� ���⼭ �� �ٴڰ��� Hit Point��
			// player�� y��ǥ�� ��� �����ؼ� ī�޶� �����԰� ���ÿ�
			// player�� �ٴڿ� �پ��ְ� �����
			m_Pos.y = Src->GetHitPoint().y;

			// ù��° ī�޶� �浹ü�� �ε�����(Camel_Arabian ����)
			// ���̱� �������� ī�޶� x�� �̵� ����
			//CCamera::GetInst()->SetXFix(true);
			//CCamera::GetInst()->SetYFix(true);
		}

		// �ٴ� �浹ü���� �浹�� ���
		else if (FloorCollision)
		{
			m_IsGround = true;
			m_Jump = false;

			m_FallTime = 0.f;

			m_Pos.y = Src->GetHitPoint().y;
			m_FallStartY = m_Pos.y;
			m_JumpVelocity = 0.f;
		}
	}
}

