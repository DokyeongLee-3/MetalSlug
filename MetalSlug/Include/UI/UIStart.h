#pragma once
#include "UIWindow.h"

// StartScene UI Ŭ����
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
    // StartScene���� Enter������ ĳ���� ���� Scene����
    // �Ѿ �� �ְ� SelectScene������ִ� �ݹ� �Լ�
    void ScenePass(float DeltaTime);
};

