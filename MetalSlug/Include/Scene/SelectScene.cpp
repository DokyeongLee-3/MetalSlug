
#include "SelectScene.h"
#include "SceneResource.h"
#include "../UI/UISelect.h"
#include "../GameManager.h"

CSelectScene::CSelectScene()
{
}

CSelectScene::~CSelectScene()
{
}

bool CSelectScene::Init()
{
	LoadSound();

	LoadAnimationSequence();

	CUISelect* SelectWindow = CreateUIWindow<CUISelect>("SelectWindow");

	return true;
}

void CSelectScene::LoadAnimationSequence()
{
	GetSceneResource()->CreateAnimationSequence("SelectSceneDarkness",
		"SelectSceneDarkness", TEXT("SelectScene_Background.bmp"));

	GetSceneResource()->SetTextureColorKey("SelectSceneDarkness",
		255, 255, 255);

	for (int i = 0; i < 10; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("SelectSceneDarkness",
			i * 1024.f, 0.f, 1024.f, 768.f);
	}


	GetSceneResource()->CreateAnimationSequence("SelectSceneScreenUpAnimation",
		"SelectSceneScreenUpAnimation", TEXT("SelectScene_ScreenAnimation.bmp"));

	GetSceneResource()->SetTextureColorKey("SelectSceneScreenUpAnimation",
		255, 255, 255);

	for (int i = 0; i < 23; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("SelectSceneScreenUpAnimation",
			i * 1024.f, 0.f, 1024.f, 768.f);
	}


	GetSceneResource()->CreateAnimationSequence("BlinkingAnimation",
		"BlinkingAnimation", TEXT("SelectScene_P1Animation.bmp"));

	GetSceneResource()->SetTextureColorKey("BlinkingAnimation",
		255, 255, 255);

	for (int i = 0; i < 2; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("BlinkingAnimation",
			i * 153.f, 0.f, 153.f, 101.f);
	}

	GetSceneResource()->CreateAnimationSequence("PauseAnimation",
		"PauseAnimation", TEXT("SelectScene_ScreenPause.bmp"));

	GetSceneResource()->SetTextureColorKey("PauseAnimation",
		255, 255, 255);

	for (int i = 0; i < 1; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("PauseAnimation",
			i * 1024.f, 0.f, 1024.f, 768.f);
	}

	GetSceneResource()->CreateAnimationSequence("FaceBlinkAnimation",
		"FaceBlinkAnimation", TEXT("SelectScene_BlinkAnimation.bmp"));

	GetSceneResource()->SetTextureColorKey("FaceBlinkAnimation",
		255, 255, 255);

	for (int i = 0; i < 7; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("FaceBlinkAnimation",
			i * 1024.f, 0.f, 1024.f, 768.f);
	}

	GetSceneResource()->CreateAnimationSequence("SelectSceneScreenDownAnimation",
		"SelectSceneScreenDownAnimation", TEXT("SelectScene_ScreenDownAnimation.bmp"));

	GetSceneResource()->SetTextureColorKey("SelectSceneScreenDownAnimation",
		255, 255, 255);

	for (int i = 0; i < 12; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("SelectSceneScreenDownAnimation",
			i * 1024.f, 0.f, 1024.f, 768.f);
	}

	GetSceneResource()->CreateAnimationSequence("SelectSceneScreenDownEffect",
		"SelectSceneScreenDownEffect", TEXT("SelectScene_ScreenDownEffect.bmp"));

	GetSceneResource()->SetTextureColorKey("SelectSceneScreenDownEffect",
		255, 255, 255);

	for (int i = 0; i < 8; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("SelectSceneScreenDownEffect",
			i * 1024.f, 0.f, 1024.f, 768.f);
	}

	GetSceneResource()->CreateAnimationSequence("SelectSceneIdle",
		"SelectSceneIdle", TEXT("SelectScene_Idle.bmp"));

	GetSceneResource()->SetTextureColorKey("SelectSceneIdle",
		255, 255, 255);

	for (int i = 0; i < 18; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("SelectSceneIdle",
			i * 1024.f, 0.f, 1024.f, 768.f);
	}
}

void CSelectScene::LoadSound()
{
	GetSceneResource()->LoadSound("BGM", true, "CharacterSelectBGM",
		"CharacterSelectBGM.wav");

	GetSceneResource()->SetVolume("BGM", 40);

	GetSceneResource()->LoadSound("UI", false, "CharacterSelectOK",
		"OKSound.mp3");
}
