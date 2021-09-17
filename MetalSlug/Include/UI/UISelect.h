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
    void ScreenAnimation();
    void PauseAnimation();
    void FaceBlinkAnimation(float DeltaTime);

public:
    void PlayCharacterSelectBGM();
};

