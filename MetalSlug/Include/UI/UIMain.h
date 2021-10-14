#pragma once
#include "UIWindow.h"

class CUIMain :
    public CUIWindow
{
    friend class CScene;

private:
    CUIMain();
    virtual ~CUIMain();

public:
    virtual bool Init();
    virtual void Start();

public:
    void IncreaseCredit(float DeltaTime);
    void DecreaseCredit(float DeltaTime);
};

