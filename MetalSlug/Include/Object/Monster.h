#pragma once

#include "Character.h"


class CMonster :
    public CCharacter
{
    friend class CScene;

private:
    Vector2 m_Dir;
    // 몇초마다 총알을 발사하는지에 대한 멤버 변수
    float	m_FireTimeMax;
    // Deltatime을 누적시켜서 FireLimitTime에 도달했는지에 체크하는 변수
    float	m_FireTime;
    // 몬스터의 총알 3발중 1발은 Player쪽으로 쏘도록 
    // 하기 위해 3발까지 count하는 용도
    int m_Count;

protected:
    CMonster();
    CMonster(const CMonster& obj);
    virtual ~CMonster();

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Collision(float DeltaTime);
    virtual void Render(HDC hDC);
    virtual CMonster* Clone();
};

