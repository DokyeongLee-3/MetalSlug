
#include "UIMain.h"
#include "UIImage.h"
#include "NumberWidget.h"
#include "../GameManager.h"
#include "../Scene/SceneResource.h"
#include "../Scene/MainScene.h"
#include "../Input.h"

CUIMain::CUIMain()
{
}

CUIMain::~CUIMain()
{
}

bool CUIMain::Init()
{

	CUIImage* Credits = CreateWidget<CUIImage>("Credits");

	Credits->SetTexture("Credits", TEXT("Font/CREDITS.bmp"));
	Credits->SetTextureColorKey(0, 255, 0);
	Credits->SetPos(650.f, 700.f);
	Credits->SetZOrder(9);

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
	CreditNumber->SetZOrder(10);

	for (int i = 0; i < 10; ++i)
	{
		CreditNumber->SetTextureColorKey(0, 255, 0, i);
	}


	return true;
}

void CUIMain::Start()
{
	CInput::GetInst()->SetCallback<CUIMain>("SceneChange",
		KeyState_Down, this, &CUIMain::IncreaseCredit);
}

void CUIMain::IncreaseCredit(float DeltaTime)
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

void CUIMain::DecreaseCredit(float DeltaTime)
{
	int PlayerCredit = CGameManager::GetInst()->GetPlayerCredit();

	if (PlayerCredit == 0)
		return;

	CGameManager::GetInst()->SetPlayerCredit(--PlayerCredit);

	CNumberWidget* PlayerCreditWidget = FindWidget<CNumberWidget>("CreditNumber");

	PlayerCredit = CGameManager::GetInst()->GetPlayerCredit();

	PlayerCreditWidget->SetNumber(PlayerCredit);
}
