#include "MainScene.h"
#include "../Object/Player.h"
#include "../Object/Monster.h"
#include "SceneResource.h"
#include "../Object/EffectHit.h"
#include "Camera.h"
#include "../UI/UIWindow.h"
#include "../UI/UIImage.h"

CMainScene::CMainScene()
{
}

CMainScene::~CMainScene()
{

}

bool CMainScene::Init()
{
	LoadAnimationSequence();

	GetCamera()->SetWorldResolution(10000.f, 10000.f);

	CEffectHit* EffectPrototype = CreatePrototype<CEffectHit>(
		"HitEffect");

	// CreatePrototype에서 CBullet의 Init을 호출하므로
	// CBullet의 Init에서 CBullet의 CColliderSphere을 생성한다
	// 따라서 MonsterBullet이라는 prototype에도 이미 Collider가 존재
	CBullet* PlayerBullet = CreatePrototype<CBullet>("PlayerBullet");
	CCollider* Collider = PlayerBullet->FindCollider("Body");

	if (Collider)
		Collider->SetCollisionProfile("PlayerAttack");

	// CreatePrototype에서 CBullet의 Init을 호출하므로
	// CBullet의 Init에서 CBullet의 CColliderSphere을 생성한다
	// 따라서 MonsterBullet이라는 prototype에도 이미 Collider가 존재
	CBullet* MonsterBullet = CreatePrototype<CBullet>("MonsterBullet");
	Collider = MonsterBullet->FindCollider("Body");

	if (Collider)
		Collider->SetCollisionProfile("MonsterAttack");


	CPlayer* Player = CreateObject<CPlayer>("Player");

	SetPlayer(Player);

	GetCamera()->SetTarget(Player);
	// 타겟이 화면 정중앙에 있도록 pivot 설정
	GetCamera()->SetTargetPivot(0.5f, 0.5f);

	CMonster* Monster = CreateObject<CMonster>("Monster",
		Vector2(1000.f, 100.f));

	//CUIWindow* TestWindow = CreateUIWindow<CUIWindow>("TestWindow");

	//CUIImage* Image = TestWindow->CreateWidget<CUIImage>("TestImage");

	//Image->SetTexture("Test", TEXT("teemo.bmp"));
	//Image->SetPos(100.f, 100.f);
	//Image->SetZOrder(1);

	//CUIImage* Image1 = TestWindow->CreateWidget<CUIImage>("TestImage1");
	//Image1->SetTexture("Test1", TEXT("Start.bmp"));
	//Image1->SetPos(150.f, 100.f);

	return true;
}

void CMainScene::LoadAnimationSequence()
{
	GetSceneResource()->CreateAnimationSequence("LucidNunNaRightIdle",
		"LucidNunNaRightIdle", TEXT("Player/Right/astand.bmp"));

	GetSceneResource()->SetTextureColorKey("LucidNunNaRightIdle",
		255, 0, 255);

	for (int i = 0; i < 6; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("LucidNunNaRightIdle",
			i * 82.f, 0.f, 82.f, 73.f);
	}

	GetSceneResource()->CreateAnimationSequence("LucidNunNaRightWalk",
		"LucidNunNaRightWalk", TEXT("Player/Right/awalk.bmp"));

	GetSceneResource()->SetTextureColorKey("LucidNunNaRightWalk",
		255, 0, 255);

	for (int i = 0; i < 4; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("LucidNunNaRightWalk",
			i * 85.f, 0.f, 85.f, 75.f);
	}

	GetSceneResource()->CreateAnimationSequence("LucidNunNaRightAttack",
		"LucidNunNaRightAttack", TEXT("Player/Right/aswing.bmp"));

	GetSceneResource()->SetTextureColorKey("LucidNunNaRightAttack",
		255, 0, 255);

	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("LucidNunNaRightAttack",
			i * 176.f, 0.f, 176.f, 89.f);
	}

	GetSceneResource()->CreateAnimationSequence("LucidNunNaRightSkill1",
		"LucidNunNaRightSkill1", TEXT("Player/Right/ashoot1.bmp"));

	GetSceneResource()->SetTextureColorKey("LucidNunNaRightSkill1",
		255, 0, 255);

	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("LucidNunNaRightSkill1",
			i * 70.f, 0.f, 70.f, 81.f);
	}

	GetSceneResource()->CreateAnimationSequence("HitRight",
		"HitRight", TEXT("Hit2.bmp"));

	GetSceneResource()->SetTextureColorKey("HitRight",
		255, 0, 255);

	for (int i = 0; i < 6; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("HitRight",
			i * 178.f, 0.f, 178.f, 164.f);
	}

	GetSceneResource()->CreateAnimationSequence("Bullet",
		"Bullet", TEXT("Smoke.bmp"));

	GetSceneResource()->SetTextureColorKey("Bullet",
		255, 0, 255);

	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			GetSceneResource()->AddAnimationFrameData("Bullet",
				j * 92.f, i * 92.f, 92.f, 92.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("LucidNunNaLeftIdle",
		"LucidNunNaLeftIdle", TEXT("Player/Left/astand_left.bmp"));

	GetSceneResource()->SetTextureColorKey("LucidNunNaLeftIdle",
		255, 0, 255);

	for (int i = 0; i < 6; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("LucidNunNaLeftIdle",
			i * 82.f, 0.f, 82.f, 73.f);
	}
}
