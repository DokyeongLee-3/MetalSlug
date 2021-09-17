#include "StartScene.h"
#include "SceneResource.h"
#include "../Object/EffectHit.h"
#include "Camera.h"
#include "../UI/UIStart.h"
#include "../UI/UISelect.h"
#include "../Input.h"

CStartScene::CStartScene()
{
}

CStartScene::~CStartScene()
{
}

bool CStartScene::Init()
{
	LoadSound();

	LoadAnimationSequence();

	CUIStart* StartWindow = CreateUIWindow<CUIStart>("StartWindow");

	return true;
}

void CStartScene::LoadAnimationSequence()
{
	GetSceneResource()->CreateAnimationSequence("StartScenePressStart",
		"StartScenePressStart", TEXT("Press1PStart.bmp"));

	GetSceneResource()->SetTextureColorKey("StartScenePressStart",
		255, 255, 255);

	for (int i = 0; i < 2; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("StartScenePressStart",
			i * 364.f, 0.f, 364.f, 26.f);
	}
}

void CStartScene::LoadSound()
{
	GetSceneResource()->LoadSound("UI", false, "CoinSound",
		"coin-sound.wav");
}
