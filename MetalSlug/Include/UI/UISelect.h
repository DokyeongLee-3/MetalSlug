#pragma once
#include "UIWindow.h"

class CUISelect :
    public CUIWindow
{
    friend class CScene;

private:
    CUISelect();
    virtual ~CUISelect();

public:
    virtual bool Init();
    virtual void Start();

public:
    void ScreenUpAnimation();
    void PauseAnimation();
    void FaceBlinkAnimation(float DeltaTime);
    void ScreenDownAnimation();
    void ScreenDownEffect();
    void PlayerIdle();
    void ConvertMainScene();

public:
    void PlayCharacterSelectBGM();
};

