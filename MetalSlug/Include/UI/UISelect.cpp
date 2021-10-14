
#include "UISelect.h"
#include "UIImage.h"
#include "NumberWidget.h"
#include "../Scene/SelectScene.h"
#include "../Scene/SceneManager.h"
#include "../Scene/SceneResource.h"
#include "../GameManager.h"
#include "../Input.h"
#include "../Scene/MainScene.h"
#include "../Object/Player.h"

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

	CUIImage* Credits = CreateWidget<CUIImage>("Credits");
	CNumberWidget* CreditNumber = CreateWidget<CNumberWidget>("CreditNumber");

	std::vector<std::wstring> vecNumberFileName;

	for (int i = 0; i < 10; ++i)
	{
		TCHAR FileName[256] = {};

		wsprintf(FileName, TEXT("Font/%d.bmp"), i);

		vecNumberFileName.push_back(FileName);
	}

	CreditNumber->SetTexture("Number", vecNumberFileName);
	CreditNumber->SetNumber(CGameManager::GetInst()->GetPlayerCredit());
	CreditNumber->SetPos(Vector2(860.f, 703.f));
	CreditNumber->SetZOrder(3);

	for (int i = 0; i < 10; ++i)
	{
		CreditNumber->SetTextureColorKey(0, 255, 0, i);
	}

	Credits->SetTexture("Credits", TEXT("Font/CREDITS.bmp"));
	Credits->SetTextureColorKey(0, 255, 0);
	Credits->SetPos(650.f, 700.f);
	Credits->SetZOrder(3);

	SelectBackground->CreateAnimation();
	SelectBackground->AddAnimation("SelectSceneDarkness", false,
		1.2f);
	SelectBackground->AddAnimation("SelectSceneScreenUpAnimation", false,
		0.6f);

	SelectBackground->AddAnimation("PauseAnimation", true,
		10000000.f);

	SelectBackground->AddAnimation("FaceBlinkAnimation", false,
		0.6f);

	SelectBackground->SetPos(0.f, 0.f);
	SelectBackground->SetZOrder(1);
	SelectBackground->SetAnimationEndNotify<CUISelect>("SelectSceneDarkness",
		this, &CUISelect::ScreenUpAnimation);

	SelectBackground->AddAnimation("SelectSceneScreenDownAnimation", false,
		0.5f);

	SelectBackground->AddAnimation("SelectSceneScreenDownEffect", false,
		0.8f);

	SelectBackground->AddAnimation("SelectSceneIdle", true,
		2.4f);

	return true;
}

void CUISelect::Start()
{
	CInput::GetInst()->SetCallback<CUISelect>("SceneChange", 
		KeyState_Down, this, &CUISelect::IncreaseCredit);

	CUIWindow::Start();
}

void CUISelect::ScreenUpAnimation()
{
	CUIImage* Background = FindWidget<CUIImage>("SelectBackground");

	Background->ChangeAnimation("SelectSceneScreenUpAnimation");

	Background->SetAnimationEndNotify<CUISelect>("SelectSceneScreenUpAnimation", this, &CUISelect::PauseAnimation);

	Background->AddAnimationNotify<CUISelect>("SelectSceneScreenUpAnimation", 4, this, &CUISelect::PlayCharacterSelectBGM);
}

void CUISelect::PauseAnimation()
{
	CUIImage* Background = FindWidget<CUIImage>("SelectBackground");

	Background->ChangeAnimation("PauseAnimation");

	CUIImage* SelectP1 = CreateWidget<CUIImage>("SelectP1");

	SelectP1->CreateAnimation();
	SelectP1->AddAnimation("BlinkingAnimation", true, 0.4f);
	SelectP1->SetPos(84.f, 144.f);
	SelectP1->SetZOrder(2);

	// 캐릭터 얼굴 위에 P1이라는 UI가 깜빡거릴 때 부터
	// 캐릭터 선택키가 활성화되어야 한다
	CInput::GetInst()->SetCallback<CUISelect>("Fire",
		KeyState_Down, this, &CUISelect::FaceBlinkAnimation);
}

void CUISelect::FaceBlinkAnimation(float DeltaTime)
{
	//CInput::GetInst()->ClearCallback();
	CUIImage* Background = FindWidget<CUIImage>("SelectBackground");

	Background->ChangeAnimation("FaceBlinkAnimation");
	GetScene()->GetSceneResource()->SoundPlay("CharacterSelectOK");

	Background->SetAnimationEndNotify<CUISelect>("FaceBlinkAnimation",
		this, &CUISelect::ScreenDownAnimation);
}

void CUISelect::ScreenDownAnimation()
{
	CUIImage* Background = FindWidget<CUIImage>("SelectBackground");

	Background->ChangeAnimation("SelectSceneScreenDownAnimation");
	Background->SetAnimationEndNotify<CUISelect>("SelectSceneScreenDownAnimation",
		this, &CUISelect::ScreenDownEffect);
}

void CUISelect::ScreenDownEffect()
{
	CUIImage* Background = FindWidget<CUIImage>("SelectBackground");

	Background->ChangeAnimation("SelectSceneScreenDownEffect");

	Background->SetAnimationEndNotify<CUISelect>("SelectSceneScreenDownEffect",
		this, &CUISelect::PlayerIdle);
}

void CUISelect::PlayerIdle()
{
	CUIImage* Background = FindWidget<CUIImage>("SelectBackground");

	Background->ChangeAnimation("SelectSceneIdle");

	Background->SetAnimationEndNotify<CUISelect>("SelectSceneIdle",
		this, &CUISelect::ConvertMainScene);
}

void CUISelect::ConvertMainScene()
{
	CSceneManager::GetInst()->CreateScene<CMainScene>();
}

void CUISelect::PlayCharacterSelectBGM()
{
	GetScene()->GetSceneResource()->SetVolume("BGM", 40);
	GetScene()->GetSceneResource()->SoundPlay("CharacterSelectBGM");
}

void CUISelect::IncreaseCredit(float DeltaTime)
{
	int PlayerCredit = CGameManager::GetInst()->GetPlayerCredit();

	if (PlayerCredit == 9)
		return;

	CGameManager::GetInst()->SetPlayerCredit(++PlayerCredit);

	CNumberWidget* PlayerCreditWidget = FindWidget<CNumberWidget>("CreditNumber");

	PlayerCredit = CGameManager::GetInst()->GetPlayerCredit();

	PlayerCreditWidget->SetNumber(PlayerCredit);
	GetScene()->GetSceneResource()->SoundPlay("CoinSound");
}
