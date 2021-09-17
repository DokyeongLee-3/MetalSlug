
#include "Player.h"
#include "../GameManager.h"
#include "../Scene/Scene.h"
#include "../Input.h"
#include "../Collision/ColliderBox.h"
#include "../Collision/ColliderSphere.h"

CPlayer::CPlayer()
{
}

CPlayer::CPlayer(const CPlayer& obj)	:
	CCharacter(obj)
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::Start()
{
	CCharacter::Start();

	// ���������� key�� �ݹ��Լ����� �ִϸ��̼��� �ٲ��ִ�
	// �뵵�̰�, �ִϸ��̼� ������ ������ �Ѿ˳�����,
	// ���� Idle�� ���ƿ��� �̷� �͵��� Notify�� �����س���
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

	// �Ǻ��� ������� ���� ��´�
	SetPivot(0.5f, 1.f);

	//SetTexture("Teemo", TEXT("teemo.bmp"));

	CreateAnimation();
	AddAnimation("LucidNunNaRightIdle");
	AddAnimation("LucidNunNaRightWalk", true, 0.6f);
	AddAnimation("LucidNunNaRightAttack", false, 0.5f);

	AddAnimationNotify<CPlayer>("LucidNunNaRightAttack",
		2, this, &CPlayer::Fire);
	SetAnimationEndNotify<CPlayer>("LucidNunNaRightAttack",
		this, &CPlayer::AttackEnd);


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


	if (CheckCurrentAnimation("LucidNunNaRightAttack"))
		SetOffset(0.f, 20.f);
	else
		SetOffset(0.f, 0.f);
}

void CPlayer::PostUpdate(float DeltaTime)
{
	CCharacter::PostUpdate(DeltaTime);

	// ���� �ִϸ��̼��� Walk�ε� �ӵ��� 0�̴ٴ� ����
	// ����ٴ� �ǹ��̴�.
	if (CheckCurrentAnimation("LucidNunNaRightWalk") &&
		m_Velocity.Length() == 0.f)
	{
		ChangeAnimation("LucidNunNaRightIdle");
	}
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
	ChangeAnimation("LucidNunNaRightWalk");
}

void CPlayer::MoveDown(float DeltaTime)
{
	//m_Pos.y += 200.f * DeltaTime;
	Move(Vector2(0.f, 1.f));
	ChangeAnimation("LucidNunNaRightWalk");
}

void CPlayer::MoveLeft(float DeltaTime)
{
	//m_Pos.x -= 200.f * DeltaTime;
	Move(Vector2(-1.f, 0.f));
	ChangeAnimation("LucidNunNaRightWalk");
}

void CPlayer::MoveRight(float DeltaTime)
{
	//m_Pos.x += 200.f * DeltaTime;
	Move(Vector2(1.f, 0.f));
	ChangeAnimation("LucidNunNaRightWalk");
}

void CPlayer::BulletFire(float DeltaTime)
{
	ChangeAnimation("LucidNunNaRightAttack");
}

void CPlayer::Pause(float DeltaTime)
{
	CGameManager::GetInst()->SetTimeScale(0.f);
}

void CPlayer::Resume(float DeltaTime)
{
	CGameManager::GetInst()->SetTimeScale(1.f);
}

void CPlayer::AttackEnd()
{
	ChangeAnimation("LucidNunNaRightIdle");
}

void CPlayer::Fire()
{
	// MainScene::Init���� �������Ҵ� Prototype�� ã�Ƽ�
	// �� Prototype�� ������ �����ؼ� ������ Scene�� 
	// m_ObjList�� �־��༭ Scene�� ��ġ�Ѵ�
	CSharedPtr<CBullet> Bullet = m_Scene->CreateObject<CBullet>(
		"Bullet", "PlayerBullet",
		Vector2(m_Pos + Vector2(75.f, 0.f)),
		Vector2(50.f, 50.f));

}

