
#include "Player.h"
#include "../GameManager.h"
#include "../Scene/Scene.h"
#include "../Input.h"
#include "../Collision/ColliderBox.h"
#include "../Collision/ColliderSphere.h"

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

	// 실질적으로 key의 콜백함수들은 애니메이션을 바꿔주는
	// 용도이고, 애니메이션 시퀀스 내에서 총알나가고,
	// 원래 Idle로 돌아오고 이런 것들은 Notify로 설정해놓음
	CInput::GetInst()->SetCallback<CPlayer>("MoveUp",
		KeyState_Push, this, &CPlayer::MoveUp);

	CInput::GetInst()->SetCallback<CPlayer>("MoveDown",
		KeyState_Push, this, &CPlayer::MoveDown);

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

	// 피봇을 밭밑으로 보통 잡는다
	SetPivot(0.5f, 1.f);

	//SetTexture("Teemo", TEXT("teemo.bmp"));

	CreateTopAnimation();
	CreateBottomAnimation();
	AddTopAnimation("PlayerIdleTop", true, 0.8f);
	AddBottomAnimation("PlayerIdleBottom", true, 0.8f);


	//AddAnimationNotify<CPlayer>("LucidNunNaRightAttack",
	//	2, this, &CPlayer::Fire);
	//SetAnimationEndNotify<CPlayer>("LucidNunNaRightAttack",
	//	this, &CPlayer::AttackEnd);


	CColliderSphere* Head = AddCollider<CColliderSphere>("Head");
	// Head->SetExtent(40.f, 30.f);
	Head->SetRadius(20.f);
	Head->SetOffset(0.f, -60.f);
	Head->SetCollisionProfile("Player");

	CColliderBox* Body = AddCollider<CColliderBox>("Body");
	Body->SetExtent(80.f, 45.f);
	Body->SetOffset(0.f, -22.5f);
	Body->SetCollisionProfile("Player");

	return true;
}

void CPlayer::Update(float DeltaTime)
{
	CCharacter::Update(DeltaTime);

	if (GetAsyncKeyState(VK_F1) & 0x8000)
		SetAttackSpeed(0.5f);


	//if (CheckCurrentAnimation("LucidNunNaRightAttack"))
	//	SetOffset(0.f, 20.f);
	//else
	//	SetOffset(0.f, 0.f);
}

void CPlayer::PostUpdate(float DeltaTime)
{
	CCharacter::PostUpdate(DeltaTime);

	// 현재 애니메이션이 Walk인데 속도가 0이다는 것은
	// 멈췄다는 의미이다.
	//if (CheckCurrentAnimation("LucidNunNaRightWalk") &&
	//	m_Velocity.Length() == 0.f)
	//{
	//	ChangeAnimation("LucidNunNaRightIdle");
	//}
}

void CPlayer::Collision(float DeltaTime)
{
	CCharacter::Collision(DeltaTime);
}

void CPlayer::Render(HDC hDC)
{
	CCharacter::Render(hDC);
}

CPlayer* CPlayer::Clone()
{
	return new CPlayer(*this);
}

void CPlayer::MoveUp(float DeltaTime)
{
	//m_Pos.y -= 200.f * DeltaTime;
	Move(Vector2(0.f, -1.f));
	//ChangeAnimation("LucidNunNaRightWalk");
}

void CPlayer::MoveDown(float DeltaTime)
{
	//m_Pos.y += 200.f * DeltaTime;
	Move(Vector2(0.f, 1.f));
	//ChangeAnimation("LucidNunNaRightWalk");
}

void CPlayer::MoveLeft(float DeltaTime)
{
	//m_Pos.x -= 200.f * DeltaTime;
	Move(Vector2(-1.f, 0.f));
	//ChangeAnimation("LucidNunNaRightWalk");
}

void CPlayer::MoveRight(float DeltaTime)
{
	//m_Pos.x += 200.f * DeltaTime;
	Move(Vector2(1.f, 0.f));
	//ChangeAnimation("LucidNunNaRightWalk");
}

void CPlayer::BulletFire(float DeltaTime)
{
	//ChangeAnimation("LucidNunNaRightAttack");
}

void CPlayer::Pause(float DeltaTime)
{
	CGameManager::GetInst()->SetTimeScale(0.f);
}

void CPlayer::Resume(float DeltaTime)
{
	CGameManager::GetInst()->SetTimeScale(1.f);
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

void CPlayer::AttackEnd()
{
	ChangeAnimation("LucidNunNaRightIdle");
}

void CPlayer::Fire()
{
	// MainScene::Init에서 만들어놓았던 Prototype을 찾아서
	// 그 Prototype의 정보를 복사해서 실제로 Scene의 
	// m_ObjList에 넣어줘서 Scene에 배치한다
	CSharedPtr<CBullet> Bullet = m_Scene->CreateObject<CBullet>(
		"Bullet", "PlayerBullet",
		Vector2(m_Pos + Vector2(75.f, 0.f)),
		Vector2(50.f, 50.f));

}

