#pragma once
#include "Monster.h"
class CArabian :
    public CMonster
{
    friend class CScene;

private:


protected:
    CArabian();
    CArabian(const CArabian& obj);
    virtual ~CArabian();

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Collision(float DeltaTime);
    virtual void Render(HDC hDC);
    virtual CArabian* Clone();
};

