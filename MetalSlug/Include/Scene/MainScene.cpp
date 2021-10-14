#include "MainScene.h"
#include "../Object/Player.h"
#include "../Object/Monster.h"
#include "SceneResource.h"
#include "../Object/EffectHit.h"
#include "Camera.h"
#include "../UI/UIWindow.h"
#include "../UI/UIImage.h"
#include "../UI/UIMain.h"
#include "../Object/Stage.h"
#include "../Object/Background.h"
#include "../Object/Bomb.h"
#include "../Object/Obstacle.h"
#include "../Collision/ColliderPixel.h"
#include "../Object/Arabian.h"
#include "../Object/Knife.h"

CMainScene::CMainScene()
{
}

CMainScene::~CMainScene()
{

}

bool CMainScene::Init()
{
	LoadAnimationSequence();
	LoadPlayerAnimationSequence();
	LoadMonsterAnimationSequence();
	LoadBackground();
	LoadSound();
	LoadObstacle();

	CUIMain* MainWindow = CreateUIWindow<CUIMain>("MainWindow");

	CPlayer* Player = CreateObject<CPlayer>("Player", Vector2(100.f, 500.f));

	SetPlayer(Player);

	CreateArabian();

	GetCamera()->SetWorldResolution(STAGE_WIDTH, STAGE_HEIGHT);

	CStage* Stage = CreateObject<CStage>("Stage");

	CEffectHit* EffectPrototype = CreatePrototype<CEffectHit>(
		"NormalAttackEffect");
	EffectPrototype = CreatePrototype<CEffectHit>(
		"BombExplosionEffect");

	// CreatePrototype에서 CBullet의 Init을 호출하므로
	// CBullet의 Init에서 CBullet의 CColliderSphere을 생성한다
	// 따라서 MonsterBullet이라는 prototype에도 이미 Collider가 존재
	CBullet* PlayerBullet = CreatePrototype<CBullet>("PlayerBullet");
	CBomb* Bomb = CreatePrototype<CBomb>("PlayerBomb");
	CKnife* Knife = CreatePrototype<CKnife>("ArabianKnife");

	CCollider* Collider = PlayerBullet->FindCollider("Body");

	if (Collider)
		Collider->SetCollisionProfile("PlayerAttack");

	Collider = Bomb->FindCollider("Body");

	if (Collider)
		Collider->SetCollisionProfile("PlayerAttack");

	// CreatePrototype에서 CBullet의 Init을 호출하므로
	// CBullet의 Init에서 CBullet의 CColliderSphere을 생성한다
	// 따라서 MonsterBullet이라는 prototype에도 이미 Collider가 존재
	// CBullet* MonsterBullet = CreatePrototype<CBullet>("MonsterBullet");
	// Collider = MonsterBullet->FindCollider("Body");

	//if (Collider)
	//	Collider->SetCollisionProfile("MonsterAttack");

	CArabian* Arabian1 = (CArabian*)FindObject("Arabian1");
	CArabian* Arabian2 = (CArabian*)FindObject("Arabian2");

	GetCamera()->SetTarget(Player);
	// 타겟이 화면 정중앙에 있도록 pivot 설정
	GetCamera()->SetTargetPivot(0.5f, 0.5f);


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
	FILE* FileStream;

	GetSceneResource()->CreateAnimationSequence("SandyWave",
		"SandyWave", TEXT("Background/Background2_Animation.bmp"));

	GetSceneResource()->SetTextureColorKey("SandyWave",
		255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("SandyWave",
			i * 700.f, 0.f, 700.f, 99.f);
	}

	GetSceneResource()->CreateAnimationSequence("NormalAttackEffect",
		"NormalAttackEffect", TEXT("NormalAttackEffect.bmp"));

	GetSceneResource()->SetTextureColorKey("NormalAttackEffect",
		255, 0, 255);

	fopen_s(&FileStream, "FrameData/NormalAttackEffect.txt", "rt");

	if (FileStream)
	{
		char	Line[128] = {};
		AnimationFrameData Data = {};
		// fgets 함수는 \n을 만나게 되면 거기까지만 읽어오게 된다.
		fgets(Line, 128, FileStream);

		for (int i = 0; i < 11; ++i)
		{
			fread(&Data, sizeof(AnimationFrameData), 1, FileStream);
			GetSceneResource()->AddAnimationFrameData("NormalAttackEffect", Data);
			Data = {};
		}
	}

	fclose(FileStream);

	GetSceneResource()->CreateAnimationSequence("BombRight",
		"BombRight", TEXT("Item/Bomb/Right/Bomb.bmp"));

	GetSceneResource()->SetTextureColorKey("BombRight",
		0, 248, 0);

	fopen_s(&FileStream, "FrameData/Bomb.txt", "rt");

	if (FileStream)
	{
		char	Line[128] = {};
		AnimationFrameData Data = {};
		// fgets 함수는 \n을 만나게 되면 거기까지만 읽어오게 된다.
		fgets(Line, 128, FileStream);

		for (int i = 0; i < 16; ++i)
		{
			fread(&Data, sizeof(AnimationFrameData), 1, FileStream);
			GetSceneResource()->AddAnimationFrameData("BombRight", Data);
			Data = {};
		}
	}

	GetSceneResource()->CreateAnimationSequence("BombExplosionEffect",
		"BombExplosionEffect", TEXT("Effect/BombExplosion.bmp"));

	GetSceneResource()->SetTextureColorKey("BombExplosionEffect",
		0, 248, 0);

	fopen_s(&FileStream, "FrameData/BombExplosion.txt", "rt");

	if (FileStream)
	{
		char	Line[128] = {};
		AnimationFrameData Data = {};
		// fgets 함수는 \n을 만나게 되면 거기까지만 읽어오게 된다.
		fgets(Line, 128, FileStream);

		for (int i = 0; i < 21; ++i)
		{
			fread(&Data, sizeof(AnimationFrameData), 1, FileStream);
			GetSceneResource()->AddAnimationFrameData("BombExplosionEffect", Data);
			Data = {};
		}
	}

	GetSceneResource()->CreateAnimationSequence("KnifeRight",
		"KnifeRight", TEXT("Monster/Arabian/Knife/Right/ArabianKnife.bmp"));

	GetSceneResource()->SetTextureColorKey("KnifeRight",
		255, 255, 255);

	fopen_s(&FileStream, "FrameData/KnifeRotateRight.fdat", "rb");

	if (FileStream)
	{
		char	Line[128] = {};
		AnimationFrameData Data = {};
		// fgets 함수는 \n을 만나게 되면 거기까지만 읽어오게 된다.
		fgets(Line, 128, FileStream);

		for (int i = 0; i < 18; ++i)
		{
			fread(&Data, sizeof(AnimationFrameData), 1, FileStream);
			GetSceneResource()->AddAnimationFrameData("KnifeRight", Data);
			Data = {};
		}
	}

	GetSceneResource()->CreateAnimationSequence("KnifeLeft",
		"KnifeLeft", TEXT("Monster/Arabian/Knife/Left/ArabianKnife.bmp"));

	GetSceneResource()->SetTextureColorKey("KnifeLeft",
		255, 255, 255);

	fopen_s(&FileStream, "FrameData/KnifeRotateLeft.fdat", "rb");

	if (FileStream)
	{
		char	Line[128] = {};
		AnimationFrameData Data = {};
		// fgets 함수는 \n을 만나게 되면 거기까지만 읽어오게 된다.
		fgets(Line, 128, FileStream);

		for (int i = 0; i < 18; ++i)
		{
			fread(&Data, sizeof(AnimationFrameData), 1, FileStream);
			GetSceneResource()->AddAnimationFrameData("KnifeLeft", Data);
			Data = {};
		}
	}
}

void CMainScene::LoadBackground()
{
	// 하늘 Background
	CBackground* Sky = CreateObject<CBackground>("Sky");
	Sky->SetPos(0.f, 50.f);
	Sky->SetPivot(0.f, 0.f);
	Sky->SetZOrder(0);
	Sky->SetPhysicsSimulate(false);
	Sky->SetTexture("Sky",
		TEXT("Background/Background3.bmp"));

	// 아지랑이 애니메이션
	CBackground* SandyWave1 = CreateObject<CBackground>("SandyWave1");
	SandyWave1->SetPos(0.f, 500.f);
	SandyWave1->SetPivot(0.f, 0.f);
	SandyWave1->SetZOrder(1);
	SandyWave1->SetPhysicsSimulate(false);
	SandyWave1->CreateAnimation();
	SandyWave1->AddAnimation("SandyWave", true, 1.5f, 1.f, false);

	CBackground* SandyWave2 = CreateObject<CBackground>("SandyWave2");
	SandyWave2->SetPos(700.f, 495.f);
	SandyWave2->SetPivot(0.f, 0.f);
	SandyWave2->SetZOrder(1);
	SandyWave2->SetPhysicsSimulate(false);
	SandyWave2->CreateAnimation();
	SandyWave2->AddAnimation("SandyWave", true, 2.f, 1.f, false);

	CBackground* SandyWave3 = CreateObject<CBackground>("SandyWave3");
	SandyWave3->SetPos(1400.f, 500.f);
	SandyWave3->SetPivot(0.f, 0.f);
	SandyWave3->SetZOrder(1);
	SandyWave3->SetPhysicsSimulate(false);
	SandyWave3->CreateAnimation();
	SandyWave3->AddAnimation("SandyWave", true, 2.f, 1.f, false);

	CBackground* SandyWave4 = CreateObject<CBackground>("SandyWave4");
	SandyWave4->SetPos(2100.f, 500.f);
	SandyWave4->SetPivot(0.f, 0.f);
	SandyWave4->SetZOrder(1);
	SandyWave4->SetPhysicsSimulate(false);
	SandyWave4->CreateAnimation();
	SandyWave4->AddAnimation("SandyWave", true, 2.f, 1.f, false);

	// Stage 바로 위 모래사막 Background
	CBackground* Desert = CreateObject<CBackground>("Desert");
	Desert->SetPos(0.f, 450.f);
	Desert->SetPivot(0.f, 0.f);
	Desert->SetZOrder(2);
	Desert->SetPhysicsSimulate(false);
	Desert->SetTexture("Desert",
		TEXT("Background/Background1_transparent.bmp"));
	Desert->SetTextureColorKey(255, 255, 255);

}

void CMainScene::LoadSound()
{
	GetSceneResource()->LoadSound("Effect", false, "NormalAttack",
		"NormalAttackSound.wav");
	GetSceneResource()->SetVolume("Effect", 60);

	GetSceneResource()->LoadSound("UI", false, "CoinSound",
		"coin-sound.wav");
}

void CMainScene::LoadPlayerAnimationSequence()
{
	FILE* FileStream;

	//GetSceneResource()->CreateAnimationSequence("PlayerIdleRightTop",
	//	"PlayerIdleRightTop", TEXT("Player/Right/Idle/Marco_IdleTop.bmp"));

	//GetSceneResource()->SetTextureColorKey("PlayerIdleRightTop",
	//	255, 255, 255);

	//// 여기서 파일에서 FrameData를 읽어와서 Load해줘야 할듯
	//fopen_s(&FileStream, "FrameData/PlayerIdleRightTop.txt", "rt");

	//if (FileStream)
	//{
	//	char	Line[128] = {};
	//	AnimationFrameData Data = {};
	//	// fgets 함수는 \n을 만나게 되면 거기까지만 읽어오게 된다.
	//	fgets(Line, 128, FileStream);

	//	for (int i = 0; i < 6; ++i)
	//	{
	//		fread(&Data, sizeof(AnimationFrameData), 1, FileStream);
	//		GetSceneResource()->AddAnimationFrameData("PlayerIdleRightTop",
	//			Data);
	//		Data = {};
	//	}
	//}

	//fclose(FileStream);

	GetSceneResource()->CreateAnimationSequence("PlayerIdleRightTop",
		"PlayerIdleRightTop", TEXT("Player/Right/Idle/Marco_IdleTop.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerIdleRightTop",
	255, 255, 255);

	CAnimationSequence* Sequence = GetSceneResource()->FindAnimationSequence("PlayerIdleRightTop");
	Sequence->SetFrameCount(6);

	GetSceneResource()->CreateAnimationSequence("PlayerIdleRightBottom",
	"PlayerIdleRightBottom", TEXT("Player/Right/Idle/Marco_IdleBottom.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerIdleRightBottom",
	255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerIdleRightBottom");
	Sequence->SetFrameCount(6);

	GetSceneResource()->CreateAnimationSequence("PlayerIdleLeftTop",
	"PlayerIdleLeftTop", TEXT("Player/Left/Idle/Marco_IdleTop.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerIdleLeftTop",
	255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerIdleLeftTop");
	Sequence->SetFrameCount(6);

	GetSceneResource()->CreateAnimationSequence("PlayerIdleLeftBottom",
	"PlayerIdleLeftBottom", TEXT("Player/Left/Idle/Marco_IdleBottom.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerIdleLeftBottom",
	255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerIdleLeftBottom");
	Sequence->SetFrameCount(6);

	GetSceneResource()->CreateAnimationSequence("PlayerNormalFireRightTop",
		"PlayerNormalFireRightTop", TEXT("Player/Right/Attack/Marco_NormalFireTop.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerNormalFireRightTop",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerNormalFireRightTop");
	Sequence->SetFrameCount(10);

	GetSceneResource()->CreateAnimationSequence("PlayerNormalFireRightBottom",
		"PlayerNormalFireRightBottom", TEXT("Player/Right/Attack/Marco_NormalFireBottom.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerNormalFireRightBottom",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerNormalFireRightBottom");
	Sequence->SetFrameCount(10);

	GetSceneResource()->CreateAnimationSequence("PlayerNormalFireLeftTop",
		"PlayerNormalFireLeftTop", TEXT("Player/Left/Attack/Marco_NormalFireTop.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerNormalFireLeftTop",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerNormalFireLeftTop");
	Sequence->SetFrameCount(10);

	GetSceneResource()->CreateAnimationSequence("PlayerNormalFireLeftBottom",
		"PlayerNormalFireLeftBottom", TEXT("Player/Left/Attack/Marco_NormalFireBottom.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerNormalFireLeftBottom",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerNormalFireLeftBottom");
	Sequence->SetFrameCount(10);

	GetSceneResource()->CreateAnimationSequence("PlayerRunRightTop",
		"PlayerRunRightTop", TEXT("Player/Right/Run/Marco_RunTop.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerRunRightTop",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerRunRightTop");
	Sequence->SetFrameCount(12);

	GetSceneResource()->CreateAnimationSequence("PlayerRunRightBottom",
		"PlayerRunRightBottom", TEXT("Player/Right/Run/Marco_RunBottom.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerRunRightBottom",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerRunRightBottom");
	Sequence->SetFrameCount(18);

	GetSceneResource()->CreateAnimationSequence("PlayerRunLeftTop",
		"PlayerRunLeftTop", TEXT("Player/Left/Run/Marco_RunTop.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerRunLeftTop",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerRunLeftTop");
	Sequence->SetFrameCount(12);

	GetSceneResource()->CreateAnimationSequence("PlayerRunLeftBottom",
		"PlayerRunLeftBottom", TEXT("Player/Left/Run/Marco_RunBottom.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerRunLeftBottom",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerRunLeftBottom");
	Sequence->SetFrameCount(18);

	GetSceneResource()->CreateAnimationSequence("PlayerVerticalJumpRightTop",
		"PlayerVerticalJumpRightTop", TEXT("Player/Right/Jump/Marco_VerticalJumpTop.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerVerticalJumpRightTop",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerVerticalJumpRightTop");
	Sequence->SetFrameCount(6);

	GetSceneResource()->CreateAnimationSequence("PlayerVerticalJumpRightBottom",
		"PlayerVerticalJumpRightBottom", TEXT("Player/Right/Jump/Marco_VerticalJumpBottom.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerVerticalJumpRightBottom",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerVerticalJumpRightBottom");
	Sequence->SetFrameCount(11);

	GetSceneResource()->CreateAnimationSequence("PlayerVerticalJumpLeftTop",
		"PlayerVerticalJumpLeftTop", TEXT("Player/Left/Jump/Marco_VerticalJumpTop.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerVerticalJumpLeftTop",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerVerticalJumpLeftTop");
	Sequence->SetFrameCount(6);

	GetSceneResource()->CreateAnimationSequence("PlayerVerticalJumpLeftBottom",
		"PlayerVerticalJumpLeftBottom", TEXT("Player/Left/Jump/Marco_VerticalJumpBottom.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerVerticalJumpLeftBottom",
		255, 255, 255);
	
	Sequence = GetSceneResource()->FindAnimationSequence("PlayerVerticalJumpLeftBottom");
	Sequence->SetFrameCount(11);

	// Jump하면서 아래로 조준하는 (공격X) 애니메이션

	GetSceneResource()->CreateAnimationSequence("PlayerJumpDownRightTop",
		"PlayerJumpDownRightTop", TEXT("Player/Right/Jump/Marco_JumpDownTop.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerJumpDownRightTop",
		255, 255, 255);

	AnimationFrameData Data = {};
	Data.StartPos.x = 0.f;
	Data.StartPos.y = 0.f;
	Data.Size.x = 70.f;
	Data.Size.y = 123.f;
	Data.Offset.x = 2.f;
	Data.Offset.y = 62.f;

	GetSceneResource()->AddAnimationFrameData("PlayerJumpDownRightTop", Data);


	GetSceneResource()->CreateAnimationSequence("PlayerJumpDownLeftTop",
		"PlayerJumpDownLeftTop", TEXT("Player/Left/Jump/Marco_JumpDownTop.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerJumpDownLeftTop",
		255, 255, 255);

	GetSceneResource()->AddAnimationFrameData("PlayerJumpDownLeftTop", Data);


	GetSceneResource()->CreateAnimationSequence("PlayerJumpAttackDownRightTop",
		"PlayerJumpAttackDownRightTop", TEXT("Player/Right/Jump/Marco_JumpAttackDownTop.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerJumpAttackDownRightTop",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerJumpAttackDownRightTop");
	Sequence->SetFrameCount(7);

	GetSceneResource()->CreateAnimationSequence("PlayerJumpAttackDownLeftTop",
		"PlayerJumpAttackDownLeftTop", TEXT("Player/Left/Jump/Marco_JumpAttackDownTop.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerJumpAttackDownLeftTop",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerJumpAttackDownLeftTop");
	Sequence->SetFrameCount(7);

	GetSceneResource()->CreateAnimationSequence("PlayerLookUpRightTop",
		"PlayerLookUpRightTop", TEXT("Player/Right/LookUp/LookUpTop.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerLookUpRightTop",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerLookUpRightTop");
	Sequence->SetFrameCount(4);

	GetSceneResource()->CreateAnimationSequence("PlayerLookUpLeftTop",
		"PlayerLookUpLeftTop", TEXT("Player/Left/LookUp/LookUpTop.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerLookUpLeftTop",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerLookUpLeftTop");
	Sequence->SetFrameCount(4);

	GetSceneResource()->CreateAnimationSequence("PlayerLookUpAttackRightTop",
		"PlayerLookUpAttackRightTop", TEXT("Player/Right/LookUp/LookUpNormalAttackTop.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerLookUpAttackRightTop",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerLookUpAttackRightTop");
	Sequence->SetFrameCount(10);


	GetSceneResource()->CreateAnimationSequence("PlayerLookUpAttackLeftTop",
		"PlayerLookUpAttackLeftTop", TEXT("Player/Left/LookUp/LookUpNormalAttackTop.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerLookUpAttackLeftTop",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerLookUpAttackLeftTop");
	Sequence->SetFrameCount(10);

	GetSceneResource()->CreateAnimationSequence("PlayerFrontJumpRightTop",
		"PlayerFrontJumpRightTop", TEXT("Player/Right/Jump/Marco_FrontJumpTop.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerFrontJumpRightTop",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerFrontJumpRightTop");
	Sequence->SetFrameCount(6);

	GetSceneResource()->CreateAnimationSequence("PlayerFrontJumpRightBottom",
		"PlayerFrontJumpRightBottom", TEXT("Player/Right/Jump/Marco_FrontJumpBottom.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerFrontJumpRightBottom",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerFrontJumpRightBottom");
	Sequence->SetFrameCount(6);

	GetSceneResource()->CreateAnimationSequence("PlayerFrontJumpLeftTop",
		"PlayerFrontJumpLeftTop", TEXT("Player/Left/Jump/Marco_FrontJumpTop.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerFrontJumpLeftTop",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerFrontJumpLeftTop");
	Sequence->SetFrameCount(6);

	GetSceneResource()->CreateAnimationSequence("PlayerFrontJumpLeftBottom",
		"PlayerFrontJumpLeftBottom", TEXT("Player/Left/Jump/Marco_FrontJumpBottom.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerFrontJumpLeftBottom",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerFrontJumpLeftBottom");
	Sequence->SetFrameCount(6);

	GetSceneResource()->CreateAnimationSequence("PlayerSitDownIdleRight",
		"PlayerSitDownIdleRight", TEXT("Player/Right/SitDown/SitDownIdleRight.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerSitDownIdleRight",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerSitDownIdleRight");
	Sequence->SetFrameCount(4);

	GetSceneResource()->CreateAnimationSequence("PlayerSitDownIdleLeft",
		"PlayerSitDownIdleLeft", TEXT("Player/Left/SitDown/SitDownIdleLeft.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerSitDownIdleLeft",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerSitDownIdleLeft");
	Sequence->SetFrameCount(4);

	GetSceneResource()->CreateAnimationSequence("PlayerCrawlRight",
		"PlayerCrawlRight", TEXT("Player/Right/SitDown/CrawlRight.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerCrawlRight",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerCrawlRight");
	Sequence->SetFrameCount(7);

	GetSceneResource()->CreateAnimationSequence("PlayerCrawlLeft",
		"PlayerCrawlLeft", TEXT("Player/Left/SitDown/CrawlLeft.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerCrawlLeft",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerCrawlLeft");
	Sequence->SetFrameCount(7);

	GetSceneResource()->CreateAnimationSequence("PlayerSitDownNormalAttackRight",
		"PlayerSitDownNormalAttackRight", TEXT("Player/Right/SitDown/SitDownNormalAttack.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerSitDownNormalAttackRight",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerSitDownNormalAttackRight");
	Sequence->SetFrameCount(9);

	GetSceneResource()->CreateAnimationSequence("PlayerSitDownNormalAttackLeft",
		"PlayerSitDownNormalAttackLeft", TEXT("Player/Left/SitDown/SitDownNormalAttack.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerSitDownNormalAttackLeft",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerSitDownNormalAttackLeft");
	Sequence->SetFrameCount(9);

	GetSceneResource()->CreateAnimationSequence("PlayerBombRightTop",
		"PlayerBombRightTop", TEXT("Player/Right/Bomb/BombTop.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerBombRightTop",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerBombRightTop");
	Sequence->SetFrameCount(5);

	GetSceneResource()->CreateAnimationSequence("PlayerBombLeftTop",
		"PlayerBombLeftTop", TEXT("Player/Left/Bomb/BombTop.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerBombLeftTop",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerBombLeftTop");
	Sequence->SetFrameCount(5);

	GetSceneResource()->CreateAnimationSequence("PlayerSitDownBombRight",
		"PlayerSitDownBombRight", TEXT("Player/Right/SitDown/SitDownBomb.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerSitDownBombRight",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerSitDownBombRight");
	Sequence->SetFrameCount(4);

	GetSceneResource()->CreateAnimationSequence("PlayerSitDownBombLeft",
		"PlayerSitDownBombLeft", TEXT("Player/Left/SitDown/SitDownBomb.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerSitDownBombLeft",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerSitDownBombLeft");
	Sequence->SetFrameCount(4);

	GetSceneResource()->CreateAnimationSequence("PlayerDeathRight",
		"PlayerDeathRight", TEXT("Player/Right/Death/PlayerDeath.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerDeathRight",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerDeathRight");
	Sequence->SetFrameCount(29);

	GetSceneResource()->CreateAnimationSequence("PlayerDeathLeft",
		"PlayerDeathLeft", TEXT("Player/Left/Death/PlayerDeath.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerDeathLeft",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("PlayerDeathLeft");
	Sequence->SetFrameCount(29);

	// PlayerDeath애니메이션은 상하체가 합쳐져 있기 때문에 Framedata를 만드는
	// 툴에서 어쩔 수 없이 blank 애니메이션을 넣어줘서 파일을 써서
	// 여기서도 쓰진 않지만 blank 애니메이션을 읽어오기 위해서 여기서 생성해주기만 한다
	GetSceneResource()->CreateAnimationSequence("Blank",
		"Blank", TEXT("Player/Blank.bmp"));

	GetSceneResource()->SetTextureColorKey("Blank",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("Blank");
	Sequence->SetFrameCount(1);

	// Player의 FrameData 차례대로 읽어오기 
	fopen_s(&FileStream, "FrameData/PlayerFrameData.fdat", "rb");

	if (FileStream)
	{
		fseek(FileStream, 0, SEEK_SET);
		while (!feof(FileStream))
		{
			char	Line[128] = {};
			int AnimNameLength = -1;
			AnimationFrameData Data = {};

			fread(&AnimNameLength, sizeof(int), 1, FileStream);

			if (AnimNameLength == -1)
				break;

			fread(Line, AnimNameLength, sizeof(char), FileStream);
			std::string AnimName = Line;


			while (AnimName[AnimName.length() - 1] == '1' ||
				AnimName[AnimName.length() - 1] == '2' ||
				AnimName[AnimName.length() - 1] == '3' ||
				AnimName[AnimName.length() - 1] == '4' ||
				AnimName[AnimName.length() - 1] == '5' ||
				AnimName[AnimName.length() - 1] == '6' ||
				AnimName[AnimName.length() - 1] == '7' ||
				AnimName[AnimName.length() - 1] == '8' ||
				AnimName[AnimName.length() - 1] == '9')
			{
				AnimName.erase(AnimName.length() - 1);
			}

			Sequence = GetSceneResource()->FindAnimationSequence(AnimName);

			int FrameCount = Sequence->GetFrameCount();

			for (int i = 0; i < FrameCount; ++i)
			{
				fread(&Data, sizeof(AnimationFrameData), 1, FileStream);
				GetSceneResource()->SetAnimationFrameData(AnimName, Data, i);
				Data = {};
			}
		}
	}

	fclose(FileStream);
}

void CMainScene::LoadMonsterAnimationSequence()
{
	FILE* FileStream;

	GetSceneResource()->CreateAnimationSequence("ArabianShuffleRight",
		"ArabianShuffleRight", TEXT("Monster/Arabian/Shuffle/Right/Arabian_Shuffle.bmp"));

	GetSceneResource()->SetTextureColorKey("ArabianShuffleRight",
		255, 255, 255);

	CAnimationSequence* Sequence = GetSceneResource()->FindAnimationSequence("ArabianShuffleRight");
	Sequence->SetFrameCount(12);


	GetSceneResource()->CreateAnimationSequence("ArabianShuffleLeft",
		"ArabianShuffleLeft", TEXT("Monster/Arabian/Shuffle/Left/Arabian_Shuffle.bmp"));

	GetSceneResource()->SetTextureColorKey("ArabianShuffleLeft",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("ArabianShuffleLeft");
	Sequence->SetFrameCount(12);


	// Turn의 Framedata는 frame수도 적고 간단해서 파일에서 읽지 않고
	// 직접 값을 넣어준다
	GetSceneResource()->CreateAnimationSequence("ArabianTurnLeft",
		"ArabianTurnLeft", TEXT("Monster/Arabian/Turn/Turn.bmp"));

	GetSceneResource()->SetTextureColorKey("ArabianTurnLeft",
		255, 255, 255);

	GetSceneResource()->CreateAnimationSequence("ArabianTurnRight",
		"ArabianTurnRight", TEXT("Monster/Arabian/Turn/Turn.bmp"));

	GetSceneResource()->SetTextureColorKey("ArabianTurnRight",
		255, 255, 255);

	AnimationFrameData Data = {};
	Data.StartPos.x = 0;
	Data.StartPos.y = 0;
	Data.Size.x = 115.f;
	Data.Size.y = 135.f;
	Data.Offset.x = 1;
	Data.Offset.y = 0;

	GetSceneResource()->AddAnimationFrameData("ArabianTurnLeft", Data);
	GetSceneResource()->AddAnimationFrameData("ArabianTurnRight", Data);

	Data.StartPos.x = 115.f;

	GetSceneResource()->AddAnimationFrameData("ArabianTurnLeft", Data);
	GetSceneResource()->AddAnimationFrameData("ArabianTurnRight", Data);

	GetSceneResource()->CreateAnimationSequence("ArabianRunRight",
		"ArabianRunRight", TEXT("Monster/Arabian/Run/Right/Arabian_Run.bmp"));

	GetSceneResource()->SetTextureColorKey("ArabianRunRight",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("ArabianRunRight");
	Sequence->SetFrameCount(12);

	GetSceneResource()->CreateAnimationSequence("ArabianRunLeft",
		"ArabianRunLeft", TEXT("Monster/Arabian/Run/Left/Arabian_Run.bmp"));

	GetSceneResource()->SetTextureColorKey("ArabianRunLeft",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("ArabianRunLeft");
	Sequence->SetFrameCount(12);

	GetSceneResource()->CreateAnimationSequence("ArabianPrepareLeft",
		"ArabianPrepareLeft", TEXT("Monster/Arabian/Prepare/Left/Prepare.bmp"));

	GetSceneResource()->SetTextureColorKey("ArabianPrepareLeft",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("ArabianPrepareLeft");
	Sequence->SetFrameCount(4);

	GetSceneResource()->CreateAnimationSequence("ArabianPrepareRight",
		"ArabianPrepareRight", TEXT("Monster/Arabian/Prepare/Right/Prepare.bmp"));

	GetSceneResource()->SetTextureColorKey("ArabianPrepareRight",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("ArabianPrepareRight");
	Sequence->SetFrameCount(4);

	GetSceneResource()->CreateAnimationSequence("ArabianAttackLeft",
		"ArabianAttackLeft", TEXT("Monster/Arabian/Attack/Left/ArabianAttack.bmp"));

	GetSceneResource()->SetTextureColorKey("ArabianAttackLeft",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("ArabianAttackLeft");
	Sequence->SetFrameCount(7);

	GetSceneResource()->CreateAnimationSequence("ArabianAttackRight",
		"ArabianAttackRight", TEXT("Monster/Arabian/Attack/Right/ArabianAttack.bmp"));

	GetSceneResource()->SetTextureColorKey("ArabianAttackRight",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("ArabianAttackRight");
	Sequence->SetFrameCount(7);

	GetSceneResource()->CreateAnimationSequence("ArabianThrowRight",
		"ArabianThrowRight", TEXT("Monster/Arabian/Throwing/Right/Throw.bmp"));

	GetSceneResource()->SetTextureColorKey("ArabianThrowRight",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("ArabianThrowRight");
	Sequence->SetFrameCount(19);

	GetSceneResource()->CreateAnimationSequence("ArabianThrowLeft",
		"ArabianThrowLeft", TEXT("Monster/Arabian/Throwing/Left/Throw.bmp"));

	GetSceneResource()->SetTextureColorKey("ArabianThrowLeft",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("ArabianThrowLeft");
	Sequence->SetFrameCount(19);

	GetSceneResource()->CreateAnimationSequence("ArabianDeathAnimationRight2",
		"ArabianDeathAnimationRight2", TEXT("Monster/Arabian/Death/Right/DeathAnimation2.bmp"));

	GetSceneResource()->SetTextureColorKey("ArabianDeathAnimationRight2",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("ArabianDeathAnimationRight2");
	Sequence->SetFrameCount(19);

	GetSceneResource()->CreateAnimationSequence("ArabianDeathAnimationLeft2",
		"ArabianDeathAnimationLeft2", TEXT("Monster/Arabian/Death/Left/DeathAnimation2.bmp"));

	GetSceneResource()->SetTextureColorKey("ArabianDeathAnimationLeft2",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("ArabianDeathAnimationLeft2");
	Sequence->SetFrameCount(19);

	GetSceneResource()->CreateAnimationSequence("ArabianDeathAnimationRight1",
		"ArabianDeathAnimationRight1", TEXT("Monster/Arabian/Death/Right/DeathAnimation1.bmp"));

	GetSceneResource()->SetTextureColorKey("ArabianDeathAnimationRight1",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("ArabianDeathAnimationRight1");
	Sequence->SetFrameCount(28);

	GetSceneResource()->CreateAnimationSequence("ArabianDeathAnimationLeft1",
		"ArabianDeathAnimationLeft1", TEXT("Monster/Arabian/Death/Left/DeathAnimation1.bmp"));

	GetSceneResource()->SetTextureColorKey("ArabianDeathAnimationLeft1",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("ArabianDeathAnimationLeft1");
	Sequence->SetFrameCount(28);

	GetSceneResource()->CreateAnimationSequence("ArabianDeathAnimationRight3",
		"ArabianDeathAnimationRight3", TEXT("Monster/Arabian/Death/Right/DeathAnimation3.bmp"));

	GetSceneResource()->SetTextureColorKey("ArabianDeathAnimationRight3",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("ArabianDeathAnimationRight3");
	Sequence->SetFrameCount(13);

	GetSceneResource()->CreateAnimationSequence("ArabianDeathAnimationLeft3",
		"ArabianDeathAnimationLeft3", TEXT("Monster/Arabian/Death/Left/DeathAnimation3.bmp"));

	GetSceneResource()->SetTextureColorKey("ArabianDeathAnimationLeft3",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("ArabianDeathAnimationLeft3");
	Sequence->SetFrameCount(13);

	GetSceneResource()->CreateAnimationSequence("ArabianJumpLeft",
		"ArabianJumpLeft", TEXT("Monster/Arabian/Jump/Left/Jump.bmp"));

	GetSceneResource()->SetTextureColorKey("ArabianJumpLeft",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("ArabianJumpLeft");
	Sequence->SetFrameCount(9);

	GetSceneResource()->CreateAnimationSequence("ArabianJumpRight",
		"ArabianJumpRight", TEXT("Monster/Arabian/Jump/Right/Jump.bmp"));

	GetSceneResource()->SetTextureColorKey("ArabianJumpRight",
		255, 255, 255);

	Sequence = GetSceneResource()->FindAnimationSequence("ArabianJumpRight");
	Sequence->SetFrameCount(9);


	// Arabian의 FrameData 차례대로 읽어오기 
	fopen_s(&FileStream, "FrameData/ArabianFrameData.fdat", "rb");

	if (FileStream)
	{
		fseek(FileStream, 0, SEEK_SET);
		while (!feof(FileStream))
		{
			char	Line[MAX_PATH] = {};
			int AnimNameLength = -1;
			AnimationFrameData Data = {};

			fread(&AnimNameLength, sizeof(int), 1, FileStream);

			if (AnimNameLength == -1)
				break;

			fread(Line, AnimNameLength, sizeof(char), FileStream);
			std::string AnimName = Line;

			Sequence = GetSceneResource()->FindAnimationSequence(AnimName);

			int FrameCount = Sequence->GetFrameCount();

			for (int i = 0; i < FrameCount; ++i)
			{
				fread(&Data, sizeof(AnimationFrameData), 1, FileStream);
				GetSceneResource()->SetAnimationFrameData(AnimName, Data, i);
				Data = {};
			}
		}
	}

	fclose(FileStream);
}

void CMainScene::LoadObstacle()
{
	GetSceneResource()->LoadTexture("FrontObstacle",
		TEXT("Obstacle/Front.bmp"));
	GetSceneResource()->SetTextureColorKey("FrontObstacle",
		255, 255, 255);

	GetSceneResource()->LoadTexture("BackObstacle",
		TEXT("Obstacle/Back.bmp"));
	GetSceneResource()->SetTextureColorKey("BackObstacle",
		255, 255, 255);

	CObstacle* FrontObstacle = CreateObject<CObstacle>("FrontObstacle",
		Vector2(2021.f, 522.f), Vector2(400.f, 314.f));

	FrontObstacle->SetTexture("FrontObstacle");

	CObstacle* BackObstacle = CreateObject<CObstacle>("BackObstacle",
		Vector2(1838.f, 530.f), Vector2(480.f, 288.f));

	BackObstacle->SetTexture("BackObstacle");


	CColliderPixel* Pixel = FrontObstacle->AddCollider<CColliderPixel>("FrontObstaclePixel");
	Pixel->SetPixelInfo(TEXT("Obstacle/Front_Collider.bmp"));
	Pixel->SetExtent((int)400, (int)314);
	Pixel->SetStartPos(2021.f, 522.f);
	Pixel->SetCollisionProfile("Obstacle");

	Pixel = BackObstacle->AddCollider<CColliderPixel>("BackObstaclePixel");
	Pixel->SetPixelInfo(TEXT("Obstacle/Back_Collider.bmp"));
	Pixel->SetExtent((int)480, (int)288);
	Pixel->SetStartPos(1838.f, 530.f);
	Pixel->SetCollisionProfile("Obstacle");
}

void CMainScene::CreateArabian()
{
	CSharedPtr<CArabian> Arabian1 = CreateObject<CArabian>("Arabian1",
		Vector2(1200.f, 500.f), Vector2(160.f, 162.f));

	CSharedPtr<CArabian> Arabian2 = CreateObject<CArabian>("Arabian2",
		Vector2(1300.f, 500.f), Vector2(160.f, 162.f));

	Arabian2->SetThrowDistance(500.f);
}

void CMainScene::GenArabian1()
{
	CSharedPtr<CArabian> Arabian3 = CreateObject<CArabian>("Arabian3",
		Vector2(2200.f, 300.f), Vector2(160.f, 162.f));
	Arabian3->SetThrowDistance(500.f);
}

void CMainScene::GenArabian2()
{
}
