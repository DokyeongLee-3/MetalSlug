#include "EffectHit.h"
#include "Bullet.h"
#include "../Scene/Scene.h"
#include "../Collision/ColliderBox.h"

CEffectHit::CEffectHit()
{

}

CEffectHit::CEffectHit(const CEffectHit& obj) :
	CGameObject(obj)
{
}

CEffectHit::~CEffectHit()
{
}

void CEffectHit::Start()
{
	CGameObject::Start();

	SetAnimationEndNotify<CEffectHit>("NormalAttackEffect", this,
		&CEffectHit::AnimationFinish);
	SetAnimationEndNotify<CEffectHit>("BombExplosionEffect", this,
		&CEffectHit::AnimationFinish);
}

bool CEffectHit::Init()
{
	if (!CGameObject::Init())
		return false;

	SetPivot(0.5f, 0.5f);
	SetZOrder(7);

	CreateAnimation();
	AddAnimation("NormalAttackEffect", false, 0.3f);
	AddAnimation("BombExplosionEffect", false, 1.f);

	if (m_Name == "NormalAttackEffect")
		ChangeAnimation("NormalAttackEffect");

	else if (m_Name == "BombExplosionEffect")
		ChangeAnimation("BombExplosionEffect");

	return true;
}

void CEffectHit::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CEffectHit::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CEffectHit::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CEffectHit::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CEffectHit* CEffectHit::Clone()
{
	return new CEffectHit(*this);
}

void CEffectHit::AnimationFinish()
{
	Destroy();
}

