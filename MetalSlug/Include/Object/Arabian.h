#pragma once

#include "Monster.h"


class CArabian :
    public CMonster
{
    friend class CScene;

private:
    // 칼을 던질 수 있는 사거리
    float m_ThrowDistance;
    float m_SwingDistance;
    EArabian_State m_State;
    class CPlayer* m_Target;
    bool m_AttackTime;
    // 애니메이션 동작중에 끊으면 안될 때 true로 해두기
    bool m_Changing;
    int m_ShuffleTime;
    int m_ShuffleLimit;

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

protected:
    // Arabian의 상태별 동작을 정의한 함수
    void FiniteState();
    void AddArabianAnimation();
    
    void CollisionBegin(CCollider* Src, CCollider* Dest,
        float DeltaTime);
    void CollisionStay(CCollider* Src, CCollider* Dest,
        float DeltaTime);
    void CollisionEnd(CCollider* Src, CCollider* Dest,
        float DeltaTime);

public:
    void SetThrowDistance(float Distance)
    {
        m_ThrowDistance = Distance;
    }

    float GetThrowDistance()    const
    {
        return m_ThrowDistance;
    }

    void SetTarget(CPlayer* Target)
    {
        m_Target = Target;
    }

    CPlayer* GetTarget()    const
    {
        return m_Target;
    }

    void SetChangeing(bool Changing)
    {
        m_Changing = Changing;
    }

    bool GetChanging() const 
    {
        return m_Changing;
    }

protected:
    void Swing();
    void SwingEnd();
    // Shuffle & Prepare 애니메이션
    void Shuffle();
    void ShuffleEnd();
    void Prepare();
    void PrepareEnd();
    void Throw();
    void ThrowEnd();
    void Running();
    void TurnEnd();
    void DeathEnd();

    void ShuffleMoveLeft();
    void ShuffleMoveRight();

    void CloneKnife();
    void SetCloseKnifeCollider();
    void DestroyCloseKnifeCollider();
};

