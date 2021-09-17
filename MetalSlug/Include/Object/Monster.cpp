#include "Monster.h"
#include "Bullet.h"
#include "../Scene/Scene.h"
#include "../Collision/ColliderBox.h"

CMonster::CMonster()	:
	m_FireTime(0.f),
	m_FireTimeMax(1.f)
{
	m_Dir.x = 0.f;
	m_Dir.y = 1.f;
}

CMonster::CMonster(const CMonster& obj) :
	CCharacter(obj)
{
	m_Dir = obj.m_Dir;
	m_FireTime = obj.m_FireTime;
	m_FireTimeMax = obj.m_FireTimeMax;
	m_Count = obj.m_Count;
}

CMonster::~CMonster()
{
}

void CMonster::Start()
{
	CCharacter::Start();
}

bool CMonster::Init()
{
	if (!CCharacter::Init())
		return false;

	SetPivot(0.5f, 1.f);

	CreateAnimation();
	AddAnimation("LucidNunNaLeftIdle");

	CColliderBox* Body = AddCollider<CColliderBox>("Body");
	Body->SetExtent(82.f, 73.f);
	Body->SetOffset(0.f, -36.5f);
	Body->SetCollisionProfile("Monster");

	return true;
}

void CMonster::Update(float DeltaTime)
{
	CCharacter::Update(DeltaTime);

	//m_Pos += m_Dir * 300.f * DeltaTime;

	if (m_Pos.y >= 720.f)
	{
		m_Pos.y = 720.f;
		m_Dir.y = -1.f;
	}

	else if (m_Pos.y - m_Size.y <= 0.f)
	{
		m_Pos.y = m_Size.y;
		m_Dir.y = 1.f;
	}

	m_FireTime += DeltaTime;

	if (m_FireTime >= m_FireTimeMax)
	{
		m_FireTime -= m_FireTimeMax;

		++m_Count;

		// m_Scene->CreateObj를 좌변 Bullet에 할당 하는 순간
		// CSharedPtr에서 T* Ptr을 인자로 받는 복사 생성자가 호출되면서
		// ref count가 2가 되지만 이 블록이 끝나면 Bullet은 지역변수라서
		// 소멸되므로 소멸자가 호출돼서 ref count = 1로 복귀
		// Bullet의 Pivot은 (0.5f, 0.5f), Monster의 Pivot은(0.5f, 1.f)
		CSharedPtr<CBullet> Bullet = m_Scene->CreateObject<CBullet>("Bullet",
			"MonsterBullet", Vector2(m_Pos - Vector2(m_Size.x / 2.f + 25.f, m_Size.y/2.f)),
			Vector2(50.f, 50.f));

		if (m_Count % 3 != 0)
		{
			Bullet->SetDir(-1.f, 0.f);
		}

		else
		{
			//CGameObject* Player = m_Scene->FindObject("Player");
			CGameObject* Player = m_Scene->GetPlayer();
			float Angle = GetAngle(Bullet->GetPos(), Player->GetPos());

			Bullet->SetDir(Angle);
		}
	}
}

void CMonster::PostUpdate(float DeltaTime)
{
	CCharacter::PostUpdate(DeltaTime);
}

void CMonster::Collision(float DeltaTime)
{
	CCharacter::Collision(DeltaTime);
}

void CMonster::Render(HDC hDC)
{
	CCharacter::Render(hDC);
}

CMonster* CMonster::Clone()
{
	return new CMonster(*this);
}

