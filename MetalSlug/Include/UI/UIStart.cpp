
#include "UIStart.h"
#include "UIImage.h"
#include "../Input.h"
#include "../GameManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/SceneResource.h"
#include "../Scene/SelectScene.h"

CUIStart::CUIStart()
{
}

CUIStart::~CUIStart()
{
}

bool CUIStart::Init()
{
	Resolution	RS = CGameManager::GetInst()->GetResolution();

	CUIImage* StartBackground = CreateWidget<CUIImage>("StartBackground");

	StartBackground->SetTexture("StartBackground", TEXT("StartScene_Background.bmp"));
	StartBackground->SetPos(0.f, 0.f);

	CUIImage* StartUI = CreateWidget<CUIImage>("PressStart");

	StartUI->SetSize(364.f, 26.f);
	StartUI->SetPos(RS.Width / 2.f - StartUI->GetSize().x /2.f,
		660.f);
	StartUI->CreateAnimation();
	StartUI->AddAnimation("StartScenePressStart", true, 1.f);
	StartUI->SetZOrder(1);

	return true;
}

void CUIStart::Start()
{
	CUIWindow::Start();

	CInput::GetInst()->SetCallback<CUIStart>("SceneChange",
		KeyState_Down, this, &CUIStart::ScenePass);
}

void CUIStart::ScenePass(float DeltaTime)
{
	GetScene()->GetSceneResource()->SetVolume("UI", 30);
	GetScene()->GetSceneResource()->SoundPlay("CoinSound");

	CSceneManager::GetInst()->CreateScene<CSelectScene>();
}
