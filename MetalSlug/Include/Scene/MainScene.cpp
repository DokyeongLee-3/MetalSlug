#include "MainScene.h"
#include "../Object/Player.h"
#include "../Object/Monster.h"
#include "SceneResource.h"
#include "../Object/EffectHit.h"
#include "Camera.h"
#include "../UI/UIWindow.h"
#include "../UI/UIImage.h"
#include "../Object/Stage.h"
#include "../Object/Background.h"

CMainScene::CMainScene()
{
}

CMainScene::~CMainScene()
{

}

bool CMainScene::Init()
{
	LoadAnimationSequence();
	LoadBackground();
	LoadSound();

	GetCamera()->SetWorldResolution(STAGE_WIDTH, STAGE_HEIGHT);

	CStage* Stage = CreateObject<CStage>("Stage");

	CEffectHit* EffectPrototype = CreatePrototype<CEffectHit>(
		"NormalAttacEffect");

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
	//CBullet* MonsterBullet = CreatePrototype<CBullet>("MonsterBullet");
	//Collider = MonsterBullet->FindCollider("Body");

	//if (Collider)
	//	Collider->SetCollisionProfile("MonsterAttack");

	CPlayer* Player = CreateObject<CPlayer>("Player", Vector2(100.f, 300.f));

	SetPlayer(Player);

	GetCamera()->SetTarget(Player);
	// 타겟이 화면 정중앙에 있도록 pivot 설정
	GetCamera()->SetTargetPivot(0.5f, 0.5f);

	//CMonster* Monster = CreateObject<CMonster>("Monster",
	//	Vector2(1000.f, 100.f));

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
	GetSceneResource()->CreateAnimationSequence("SandyWave",
		"SandyWave", TEXT("Background/Background2_Animation.bmp"));

	GetSceneResource()->SetTextureColorKey("SandyWave",
		255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("SandyWave",
			i * 700.f, 0.f, 700.f, 99.f);
	}

	FILE* FileStream;

	GetSceneResource()->CreateAnimationSequence("PlayerIdleRightTop",
		"PlayerIdleRightTop", TEXT("Player/Right/Idle/Marco_IdleTop.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerIdleRightTop",
		255, 255, 255);

	// 여기서 파일에서 FrameData를 읽어와서 Load해줘야 할듯
	fopen_s(&FileStream, "PlayerIdleRightTop.txt", "rt");

	if (FileStream)
	{
		char	Line[128] = {};
		AnimationFrameData Data = {};
		// fgets 함수는 \n을 만나게 되면 거기까지만 읽어오게 된다.
		fgets(Line, 128, FileStream);

		for (int i = 0; i < 6; ++i)
		{
			fread(&Data, sizeof(AnimationFrameData), 1, FileStream);
			GetSceneResource()->AddAnimationFrameData("PlayerIdleRightTop",
				Data);
			Data = {};
		}
	}

	fclose(FileStream);


	GetSceneResource()->CreateAnimationSequence("PlayerIdleRightBottom",
		"PlayerIdleRightBottom", TEXT("Player/Right/Idle/Marco_IdleBottom.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerIdleRightBottom",
		255, 255, 255);

	fopen_s(&FileStream, "PlayerIdleRightBottom.txt", "rt");

	if (FileStream)
	{
		char	Line[128] = {};
		AnimationFrameData Data = {};
		// fgets 함수는 \n을 만나게 되면 거기까지만 읽어오게 된다.
		fgets(Line, 128, FileStream);

		for (int i = 0; i < 6; ++i)
		{
			fread(&Data, sizeof(AnimationFrameData), 1, FileStream);
			GetSceneResource()->AddAnimationFrameData("PlayerIdleRightBottom",
				Data);
			Data = {};
		}
	}

	fclose(FileStream);

	GetSceneResource()->CreateAnimationSequence("PlayerIdleLeftTop",
		"PlayerIdleLeftTop", TEXT("Player/Left/Idle/Marco_IdleTop.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerIdleLeftTop",
		255, 255, 255);

	// 여기서 파일에서 FrameData를 읽어와서 Load해줘야 할듯
	fopen_s(&FileStream, "PlayerIdleLeftTop.txt", "rt");

	if (FileStream)
	{
		char	Line[128] = {};
		AnimationFrameData Data = {};
		// fgets 함수는 \n을 만나게 되면 거기까지만 읽어오게 된다.
		fgets(Line, 128, FileStream);

		for (int i = 0; i < 6; ++i)
		{
			fread(&Data, sizeof(AnimationFrameData), 1, FileStream);
			GetSceneResource()->AddAnimationFrameData("PlayerIdleLeftTop",
				Data);
			Data = {};
		}
	}

	fclose(FileStream);

	GetSceneResource()->CreateAnimationSequence("PlayerIdleLeftBottom",
		"PlayerIdleLeftBottom", TEXT("Player/Left/Idle/Marco_IdleBottom.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerIdleLeftBottom",
		255, 255, 255);

	fopen_s(&FileStream, "PlayerIdleLeftBottom.txt", "rt");

	if (FileStream)
	{
		char	Line[128] = {};
		AnimationFrameData Data = {};
		// fgets 함수는 \n을 만나게 되면 거기까지만 읽어오게 된다.
		fgets(Line, 128, FileStream);

		for (int i = 0; i < 6; ++i)
		{
			fread(&Data, sizeof(AnimationFrameData), 1, FileStream);
			GetSceneResource()->AddAnimationFrameData("PlayerIdleLeftBottom",
				Data);
			Data = {};
		}
	}

	fclose(FileStream);

	//////// NormalFire Animation ////////

	GetSceneResource()->CreateAnimationSequence("PlayerNormalFireRightTop",
		"PlayerNormalFireRightTop", TEXT("Player/Right/Attack/Marco_NormalFireTop.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerNormalFireRightTop",
		255, 255, 255);

	// 여기서 파일에서 FrameData를 읽어와서 Load해줘야 할듯
	fopen_s(&FileStream, "PlayerNormalFireRightTop.txt", "rt");

	if (FileStream)
	{
		char	Line[128] = {};
		AnimationFrameData Data = {};
		// fgets 함수는 \n을 만나게 되면 거기까지만 읽어오게 된다.
		fgets(Line, 128, FileStream);

		for (int i = 0; i < 10; ++i)
		{
			fread(&Data, sizeof(AnimationFrameData), 1, FileStream);
			GetSceneResource()->AddAnimationFrameData("PlayerNormalFireRightTop", Data);
			Data = {};
		}
	}

	fclose(FileStream);

	GetSceneResource()->CreateAnimationSequence("PlayerNormalFireRightBottom",
		"PlayerNormalFireRightBottom", TEXT("Player/Right/Attack/Marco_NormalFireBottom.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerNormalFireRightBottom",
		255, 255, 255);

	// 여기서 파일에서 FrameData를 읽어와서 Load해줘야 할듯
	fopen_s(&FileStream, "PlayerNormalFireRightBottom.txt", "rt");

	if (FileStream)
	{
		char	Line[128] = {};
		AnimationFrameData Data = {};
		// fgets 함수는 \n을 만나게 되면 거기까지만 읽어오게 된다.
		fgets(Line, 128, FileStream);

		for (int i = 0; i < 10; ++i)
		{
			fread(&Data, sizeof(AnimationFrameData), 1, FileStream);
			GetSceneResource()->AddAnimationFrameData("PlayerNormalFireRightBottom", Data);
			Data = {};
		}
	}

	fclose(FileStream);




	GetSceneResource()->CreateAnimationSequence("PlayerNormalFireLeftTop",
		"PlayerNormalFireLeftTop", TEXT("Player/Left/Attack/Marco_NormalFireTop.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerNormalFireLeftTop",
		255, 255, 255);

	// 여기서 파일에서 FrameData를 읽어와서 Load해줘야 할듯
	fopen_s(&FileStream, "PlayerNormalFireLeftTop.txt", "rt");

	if (FileStream)
	{
		char	Line[128] = {};
		AnimationFrameData Data = {};
		// fgets 함수는 \n을 만나게 되면 거기까지만 읽어오게 된다.
		fgets(Line, 128, FileStream);

		for (int i = 0; i < 10; ++i)
		{
			fread(&Data, sizeof(AnimationFrameData), 1, FileStream);
			GetSceneResource()->AddAnimationFrameData("PlayerNormalFireLeftTop", Data);
			Data = {};
		}
	}

	fclose(FileStream);

	GetSceneResource()->CreateAnimationSequence("PlayerNormalFireLeftBottom",
		"PlayerNormalFireLeftBottom", TEXT("Player/Left/Attack/Marco_NormalFireBottom.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerNormalFireLeftBottom",
		255, 255, 255);

	// 여기서 파일에서 FrameData를 읽어와서 Load해줘야 할듯
	fopen_s(&FileStream, "PlayerNormalFireLeftBottom.txt", "rt");

	if (FileStream)
	{
		char	Line[128] = {};
		AnimationFrameData Data = {};
		// fgets 함수는 \n을 만나게 되면 거기까지만 읽어오게 된다.
		fgets(Line, 128, FileStream);

		for (int i = 0; i < 10; ++i)
		{
			fread(&Data, sizeof(AnimationFrameData), 1, FileStream);
			GetSceneResource()->AddAnimationFrameData("PlayerNormalFireLeftBottom", Data);
			Data = {};
		}
	}

	fclose(FileStream);


	//// PlayerRun ///

	GetSceneResource()->CreateAnimationSequence("PlayerRunRightTop",
		"PlayerRunRightTop", TEXT("Player/Right/Run/Marco_RunTop.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerRunRightTop",
		255, 255, 255);

	// 여기서 파일에서 FrameData를 읽어와서 Load해줘야 할듯
	fopen_s(&FileStream, "PlayerRunRightTop.txt", "rt");

	if (FileStream)
	{
		char	Line[128] = {};
		AnimationFrameData Data = {};
		// fgets 함수는 \n을 만나게 되면 거기까지만 읽어오게 된다.
		fgets(Line, 128, FileStream);

		for (int i = 0; i < 12; ++i)
		{
			fread(&Data, sizeof(AnimationFrameData), 1, FileStream);
			GetSceneResource()->AddAnimationFrameData("PlayerRunRightTop", Data);
			Data = {};
		}
	}

	fclose(FileStream);

	GetSceneResource()->CreateAnimationSequence("PlayerRunRightBottom",
		"PlayerRunRightBottom", TEXT("Player/Right/Run/Marco_RunBottom.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerRunRightBottom",
		255, 255, 255);

	// 여기서 파일에서 FrameData를 읽어와서 Load해줘야 할듯
	fopen_s(&FileStream, "PlayerRunRightBottom.txt", "rt");

	if (FileStream)
	{
		char	Line[128] = {};
		AnimationFrameData Data = {};
		// fgets 함수는 \n을 만나게 되면 거기까지만 읽어오게 된다.
		fgets(Line, 128, FileStream);

		for (int i = 0; i < 18; ++i)
		{
			fread(&Data, sizeof(AnimationFrameData), 1, FileStream);
			GetSceneResource()->AddAnimationFrameData("PlayerRunRightBottom", Data);
			Data = {};
		}
	}

	fclose(FileStream);

	GetSceneResource()->CreateAnimationSequence("PlayerRunLeftTop",
		"PlayerRunLeftTop", TEXT("Player/Left/Run/Marco_RunTop.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerRunLeftTop",
		255, 255, 255);

	// 여기서 파일에서 FrameData를 읽어와서 Load해줘야 할듯
	fopen_s(&FileStream, "PlayerRunLeftTop.txt", "rt");

	if (FileStream)
	{
		char	Line[128] = {};
		AnimationFrameData Data = {};
		// fgets 함수는 \n을 만나게 되면 거기까지만 읽어오게 된다.
		fgets(Line, 128, FileStream);

		for (int i = 0; i < 12; ++i)
		{
			fread(&Data, sizeof(AnimationFrameData), 1, FileStream);
			GetSceneResource()->AddAnimationFrameData("PlayerRunLeftTop", Data);
			Data = {};
		}
	}

	fclose(FileStream);

	GetSceneResource()->CreateAnimationSequence("PlayerRunLeftBottom",
		"PlayerRunLeftBottom", TEXT("Player/Left/Run/Marco_RunBottom.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerRunLeftBottom",
		255, 255, 255);

	// 여기서 파일에서 FrameData를 읽어와서 Load해줘야 할듯
	fopen_s(&FileStream, "PlayerRunLeftBottom.txt", "rt");

	if (FileStream)
	{
		char	Line[128] = {};
		AnimationFrameData Data = {};
		// fgets 함수는 \n을 만나게 되면 거기까지만 읽어오게 된다.
		fgets(Line, 128, FileStream);

		for (int i = 0; i < 18; ++i)
		{
			fread(&Data, sizeof(AnimationFrameData), 1, FileStream);
			GetSceneResource()->AddAnimationFrameData("PlayerRunLeftBottom", Data);
			Data = {};
		}
	}

	fclose(FileStream);



	//// PlayerJump ///

	GetSceneResource()->CreateAnimationSequence("PlayerVerticalJumpRightTop",
		"PlayerVerticalJumpRightTop", TEXT("Player/Right/Jump/Marco_VerticalJumpTop.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerVerticalJumpRightTop",
		255, 255, 255);

	// 여기서 파일에서 FrameData를 읽어와서 Load해줘야 할듯
	fopen_s(&FileStream, "PlayerVerticalJumpRightTop.txt", "rt");

	if (FileStream)
	{
		char	Line[128] = {};
		AnimationFrameData Data = {};
		// fgets 함수는 \n을 만나게 되면 거기까지만 읽어오게 된다.
		fgets(Line, 128, FileStream);

		for (int i = 0; i < 6; ++i)
		{
			fread(&Data, sizeof(AnimationFrameData), 1, FileStream);
			GetSceneResource()->AddAnimationFrameData("PlayerVerticalJumpRightTop", Data);
			Data = {};
		}
	}

	fclose(FileStream);

	GetSceneResource()->CreateAnimationSequence("PlayerVerticalJumpRightBottom",
		"PlayerVerticalJumpRightBottom", TEXT("Player/Right/Jump/Marco_VerticalJumpBottom.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerVerticalJumpRightBottom",
		255, 255, 255);

	// 여기서 파일에서 FrameData를 읽어와서 Load해줘야 할듯
	fopen_s(&FileStream, "PlayerVerticalJumpRightBottom.txt", "rt");

	if (FileStream)
	{
		char	Line[128] = {};
		AnimationFrameData Data = {};
		// fgets 함수는 \n을 만나게 되면 거기까지만 읽어오게 된다.
		fgets(Line, 128, FileStream);

		for (int i = 0; i < 11; ++i)
		{
			fread(&Data, sizeof(AnimationFrameData), 1, FileStream);
			GetSceneResource()->AddAnimationFrameData("PlayerVerticalJumpRightBottom", Data);
			Data = {};
		}
	}

	fclose(FileStream);

	GetSceneResource()->CreateAnimationSequence("PlayerVerticalJumpLeftTop",
		"PlayerVerticalJumpLeftTop", TEXT("Player/Left/Jump/Marco_VerticalJumpTop.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerVerticalJumpLeftTop",
		255, 255, 255);

	// 여기서 파일에서 FrameData를 읽어와서 Load해줘야 할듯
	fopen_s(&FileStream, "PlayerVerticalJumpLeftTop.txt", "rt");

	if (FileStream)
	{
		char	Line[128] = {};
		AnimationFrameData Data = {};
		// fgets 함수는 \n을 만나게 되면 거기까지만 읽어오게 된다.
		fgets(Line, 128, FileStream);

		for (int i = 0; i < 6; ++i)
		{
			fread(&Data, sizeof(AnimationFrameData), 1, FileStream);
			GetSceneResource()->AddAnimationFrameData("PlayerVerticalJumpLeftTop", Data);
			Data = {};
		}
	}

	fclose(FileStream);

	GetSceneResource()->CreateAnimationSequence("PlayerVerticalJumpLeftBottom",
		"PlayerVerticalJumpLeftBottom", TEXT("Player/Left/Jump/Marco_VerticalJumpBottom.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerVerticalJumpLeftBottom",
		255, 255, 255);

	// 여기서 파일에서 FrameData를 읽어와서 Load해줘야 할듯
	fopen_s(&FileStream, "PlayerVerticalJumpLeftBottom.txt", "rt");

	if (FileStream)
	{
		char	Line[128] = {};
		AnimationFrameData Data = {};
		// fgets 함수는 \n을 만나게 되면 거기까지만 읽어오게 된다.
		fgets(Line, 128, FileStream);

		for (int i = 0; i < 11; ++i)
		{
			fread(&Data, sizeof(AnimationFrameData), 1, FileStream);
			GetSceneResource()->AddAnimationFrameData("PlayerVerticalJumpLeftBottom", Data);
			Data = {};
		}
	}

	fclose(FileStream);


	GetSceneResource()->CreateAnimationSequence("NormalAttackEffect",
		"NormalAttackEffect", TEXT("NormalAttackEffect.bmp"));

	GetSceneResource()->SetTextureColorKey("NormalAttackEffect",
		255, 0, 255);

	// 여기서 파일에서 FrameData를 읽어와서 Load해줘야 할듯
	fopen_s(&FileStream, "NormalAttackEffect.txt", "rt");

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

	// Jump하면서 아래로 조준하는 (공격X) 애니메이션

	GetSceneResource()->CreateAnimationSequence("PlayerJumpDownRightTop",
		"PlayerJumpDownRightTop", TEXT("Player/Right/Jump/Marco_JumpDownTop.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerJumpDownRightTop",
		255, 255, 255);
	
	AnimationFrameData Data = {};
	Data.StartPos.x = 0.f;
	Data.StartPos.y = 0.f;
	Data.Size.x = 60.f;
	Data.Size.y = 105.f;
	Data.Offset.x = 0.f;
	Data.Offset.y = 50.f;

	GetSceneResource()->AddAnimationFrameData("PlayerJumpDownRightTop", Data);


	GetSceneResource()->CreateAnimationSequence("PlayerJumpDownLeftTop",
		"PlayerJumpDownLeftTop", TEXT("Player/Left/Jump/Marco_JumpDownTop.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerJumpDownLeftTop",
		255, 255, 255);

	GetSceneResource()->AddAnimationFrameData("PlayerJumpDownLeftTop", Data);


	// Jump하면서 아래로 총알 발사하는 애니메이션

	GetSceneResource()->CreateAnimationSequence("PlayerJumpAttackDownRightTop",
		"PlayerJumpAttackDownRightTop", TEXT("Player/Right/Jump/Marco_JumpAttackDownTop.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerJumpAttackDownRightTop",
		255, 255, 255);

	// 여기서 파일에서 FrameData를 읽어와서 Load해줘야 할듯
	fopen_s(&FileStream, "PlayerJumpAttackDownRightTop.txt", "rt");

	if (FileStream)
	{
		char	Line[128] = {};
		AnimationFrameData Data = {};
		// fgets 함수는 \n을 만나게 되면 거기까지만 읽어오게 된다.
		fgets(Line, 128, FileStream);

		for (int i = 0; i < 7; ++i)
		{
			fread(&Data, sizeof(AnimationFrameData), 1, FileStream);
			GetSceneResource()->AddAnimationFrameData("PlayerJumpAttackDownRightTop", Data);
			Data = {};
		}
	}

	fclose(FileStream);

	GetSceneResource()->CreateAnimationSequence("PlayerJumpAttackDownLeftTop",
		"PlayerJumpAttackDownLeftTop", TEXT("Player/Left/Jump/Marco_JumpAttackDownTop.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerJumpAttackDownLeftTop",
		255, 255, 255);

	// 여기서 파일에서 FrameData를 읽어와서 Load해줘야 할듯
	fopen_s(&FileStream, "PlayerJumpAttackDownLeftTop.txt", "rt");

	if (FileStream)
	{
		char	Line[128] = {};
		AnimationFrameData Data = {};
		// fgets 함수는 \n을 만나게 되면 거기까지만 읽어오게 된다.
		fgets(Line, 128, FileStream);

		for (int i = 0; i < 7; ++i)
		{
			fread(&Data, sizeof(AnimationFrameData), 1, FileStream);
			GetSceneResource()->AddAnimationFrameData("PlayerJumpAttackDownLeftTop", Data);
			Data = {};
		}
	}

	fclose(FileStream);
}

void CMainScene::LoadBackground()
{
	// 하늘 Background
	CBackground* Sky = CreateObject<CBackground>("Sky");
	Sky->SetPos(0.f, 50.f);
	Sky->SetPivot(0.f, 0.f);
	Sky->SetPhysicsSimulate(false);
	Sky->SetTexture("Sky",
		TEXT("Background/Background3.bmp"));

	// 아지랑이 애니메이션
	CBackground* SandyWave1 = CreateObject<CBackground>("SandyWave1");
	SandyWave1->SetPos(0.f, 500.f);
	SandyWave1->SetPivot(0.f, 0.f);
	SandyWave1->SetPhysicsSimulate(false);
	SandyWave1->CreateAnimation();
	SandyWave1->AddAnimation("SandyWave", true, 2.f, 1.f, false);

	CBackground* SandyWave2 = CreateObject<CBackground>("SandyWave2");
	SandyWave2->SetPos(700.f, 500.f);
	SandyWave2->SetPivot(0.f, 0.f);
	SandyWave2->SetPhysicsSimulate(false);
	SandyWave2->CreateAnimation();
	SandyWave2->AddAnimation("SandyWave", true, 2.f, 1.f, false);

	CBackground* SandyWave3 = CreateObject<CBackground>("SandyWave3");
	SandyWave3->SetPos(1400.f, 500.f);
	SandyWave3->SetPivot(0.f, 0.f);
	SandyWave3->SetPhysicsSimulate(false);
	SandyWave3->CreateAnimation();
	SandyWave3->AddAnimation("SandyWave", true, 2.f, 1.f, false);

	CBackground* SandyWave4 = CreateObject<CBackground>("SandyWave4");
	SandyWave4->SetPos(2100.f, 500.f);
	SandyWave4->SetPivot(0.f, 0.f);
	SandyWave4->SetPhysicsSimulate(false);
	SandyWave4->CreateAnimation();
	SandyWave4->AddAnimation("SandyWave", true, 2.f, 1.f, false);

	// Stage 바로 위 모래사막 Background
	CBackground* Desert = CreateObject<CBackground>("Desert");
	Desert->SetPos(0.f, 450.f);
	Desert->SetPivot(0.f, 0.f);
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
}
