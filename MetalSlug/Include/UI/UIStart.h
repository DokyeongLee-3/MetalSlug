#pragma once
#include "UIWindow.h"

// StartScene UI 클래스
class CUIStart :
    public CUIWindow
{
    friend class CScene;

private:
    CUIStart();
    virtual ~CUIStart();

public:
    virtual bool Init();
    virtual void Start();

public:
    // StartScene에서 Enter누르면 캐릭터 고르는 Scene으로
    // 넘어갈 수 있게 SelectScene만들어주는 콜백 함수
    void ScenePass(float DeltaTime);
};

