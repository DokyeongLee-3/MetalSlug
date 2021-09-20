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

	GetCamera()->SetWorldResolution(STAGE_WIDTH, STAGE_HEIGHT);

	CEffectHit* EffectPrototype = CreatePrototype<CEffectHit>(
		"HitEffect");

	// CreatePrototype���� CBullet�� Init�� ȣ���ϹǷ�
	// CBullet�� Init���� CBullet�� CColliderSphere�� �����Ѵ�
	// ���� MonsterBullet�̶�� prototype���� �̹� Collider�� ����
	CBullet* PlayerBullet = CreatePrototype<CBullet>("PlayerBullet");
	CCollider* Collider = PlayerBullet->FindCollider("Body");

	if (Collider)
		Collider->SetCollisionProfile("PlayerAttack");

	// CreatePrototype���� CBullet�� Init�� ȣ���ϹǷ�
	// CBullet�� Init���� CBullet�� CColliderSphere�� �����Ѵ�
	// ���� MonsterBullet�̶�� prototype���� �̹� Collider�� ����
	CBullet* MonsterBullet = CreatePrototype<CBullet>("MonsterBullet");
	Collider = MonsterBullet->FindCollider("Body");

	if (Collider)
		Collider->SetCollisionProfile("MonsterAttack");


	CPlayer* Player = CreateObject<CPlayer>("Player");

	SetPlayer(Player);

	GetCamera()->SetTarget(Player);
	// Ÿ���� ȭ�� ���߾ӿ� �ֵ��� pivot ����
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
	GetSceneResource()->CreateAnimationSequence("PlayerIdleTop",
		"PlayerIdleTop", TEXT("Player/Right/Idle/Marco_IdleTop.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerIdleTop",
		255, 255, 255);

	// ���⼭ ���Ͽ��� FrameData�� �о�ͼ� Load����� �ҵ�
	for (int i = 0; i < 6; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("PlayerIdleTop",
			i * 82.f, 0.f, 82.f, 73.f);
	}


}
