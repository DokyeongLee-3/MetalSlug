#pragma once

#include "Monster.h"


class CArabian :
    public CMonster
{
    friend class CScene;

private:
    // Į�� ���� �� �ִ� ��Ÿ�
    float m_ThrowDistance;
    float m_SwingDistance;
    EArabian_State m_State;
    class CPlayer* m_Target;
    bool m_AttackTime;
    // �ִϸ��̼� �����߿� ������ �ȵ� �� true�� �صα�
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
    // Arabian�� ���º� ������ ������ �Լ�
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
    // Shuffle & Prepare �ִϸ��̼�
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

