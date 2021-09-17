
#include "UISelect.h"
#include "UIImage.h"
#include "../Scene/SelectScene.h"
#include "../Scene/SceneManager.h"
#include "../Scene/SceneResource.h"
#include "../GameManager.h"
#include "../Input.h"


CUISelect::CUISelect()
{
}

CUISelect::~CUISelect()
{
}

bool CUISelect::Init()
{
	Resolution	RS = CGameManager::GetInst()->GetResolution();

	CUIImage* SelectBackground = CreateWidget<CUIImage>("SelectBackground");

	SelectBackground->CreateAnimation();
	SelectBackground->AddAnimation("SelectSceneDarkness", false,
		1.f);
	SelectBackground->AddAnimation("SelectSceneScreenAnimation", false,
		1.f);

	SelectBackground->AddAnimation("PauseAnimation", true,
		10000000.f);

	SelectBackground->AddAnimation("FaceBlinkAnimation", false,
		0.2f);

	SelectBackground->SetPos(0.f, 0.f);
	SelectBackground->SetZOrder(1);
	SelectBackground->SetAnimationEndNotify<CUISelect>("SelectSceneDarkness",
		this, &CUISelect::ScreenAnimation);

	return true;
}

void CUISelect::Start()
{
	CUIWindow::Start();
}

void CUISelect::ScreenAnimation()
{
	CUIImage* Background = FindWidget<CUIImage>("SelectBackground");

	Background->ChangeAnimation("SelectSceneScreenAnimation");

	Background->SetAnimationEndNotify<CUISelect>("SelectSceneScreenAnimation", this, &CUISelect::PauseAnimation);

	Background->AddAnimationNotify<CUISelect>("SelectSceneScreenAnimation", 4, this, &CUISelect::PlayCharacterSelectBGM);
}

void CUISelect::PauseAnimation()
{
	CUIImage* Background = FindWidget<CUIImage>("SelectBackground");

	Background->ChangeAnimation("PauseAnimation");

	CUIImage* SelectP1 = CreateWidget<CUIImage>("SelectP1");

	SelectP1->CreateAnimation();
	SelectP1->AddAnimation("BlinkingAnimation", true, 0.4f);
	SelectP1->SetPos(84.f, 146.f);
	SelectP1->SetZOrder(2);

	// 캐릭터 얼굴 위에 P1이라는 UI가 깜빡거릴 때 부터
	// 캐릭터 선택키가 활성화되어야 한다
	CInput::GetInst()->SetCallback<CUISelect>("Fire",
		KeyState_Down, this, &CUISelect::FaceBlinkAnimation);
}

void CUISelect::FaceBlinkAnimation(float DeltaTime)
{
	CUIImage* Background = FindWidget<CUIImage>("SelectBackground");

	Background->ChangeAnimation("FaceBlinkAnimation");
	GetScene()->GetSceneResource()->SoundPlay("CharacterSelectOK");
}

void CUISelect::PlayCharacterSelectBGM()
{
	GetScene()->GetSceneResource()->SetVolume("BGM", 40);
	GetScene()->GetSceneResource()->SoundPlay("CharacterSelectBGM");
}
