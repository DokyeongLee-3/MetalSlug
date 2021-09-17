
#include "Bullet.h"
#include "../Collision/ColliderBox.h"
#include "../Collision/ColliderSphere.h"
#include "EffectHit.h"
#include "../Scene/Scene.h"

CBullet::CBullet()
{
	m_Dir.x = 1.f;
	m_Dir.y = 0.f;

	m_Distance = 800.f;

	SetMoveSpeed(500.f);
}

CBullet::CBullet(const CBullet& obj)	:
	CGameObject(obj)
{
	m_Dir = obj.m_Dir;
	m_Distance = obj.m_Distance;
}

CBullet::~CBullet()
{

}

void CBullet::Start()
{
	CGameObject::Start();

	CCollider* Body = FindCollider("Body");

	Body->SetCollisionBeginFunction<CBullet>(this,
		&CBullet::CollisionBegin);
}

bool CBullet::Init()
{
	if (!CGameObject::Init())
		return false;
	
	//SetTexture("Bullet", TEXT("Hit2.bmp"));
	//SetSize(178.f, 164.f);
	//SetImageStart(178.f * 5, 0.f);
	//SetTextureColorKey(255, 0, 255);

	SetPivot(0.5f, 0.5f);

	CreateAnimation();
	AddAnimation("Bullet", true, 1.f);

	// 이미 MainScene::Init에서 Prototype만들때
	// Collider를 만들어줬고 Scene의 m_Prototype에 있는
	// Prototype을 복사해서 쓸 것이므로 아래 설정 필요x
	//CColliderBox* Body = AddCollider<CColliderBox>("Body");
	//Body->SetExtent(50.f, 50.f);
	//Body->SetOffset(0.f, 0.f);
	//Body->SetCollisionBeginFunction<CBullet>(this,
	//	&CBullet::CollisionBegin);

	CColliderSphere* Body = AddCollider<CColliderSphere>("Body");
	Body->SetRadius(25.f);
	Body->SetOffset(0.f, 0.f);

	return true;
}

void CBullet::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	Vector2 Dir = m_Dir;
	Dir.Normalize();

	Move(Dir);

	m_Distance -= GetMoveSpeedFrame();

	if (m_Distance <= 0.f)
	{
		Destroy();
	}

}

void CBullet::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CBullet::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CBullet::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CBullet* CBullet::Clone()
{
	return new CBullet(*this);
}

void CBullet::CollisionBegin(CCollider* Src, CCollider* Dest, float DeltaTime)
{
	Destroy();

	CEffectHit* Hit = m_Scene->CreateObject<CEffectHit>(
		"HitEffect", "HitEffect",
		m_Pos, Vector2(178.f, 164.f));
}
