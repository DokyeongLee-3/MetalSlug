#pragma once

#include "Monster.h"
#include "Player.h"

class CArabian :
    public CMonster
{
    friend class CScene;

private:
    // 플레이어와 거리를 감지할 수 있는 한계
    // 이 거리 안으로 들어오면 플레이어를 추적한다
    float m_DetectDistance;
    EArabian_State m_State;
    CPlayer* m_Target;

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

