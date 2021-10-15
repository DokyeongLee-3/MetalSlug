#pragma once
#include "GameObject.h"
class CNPC :
    public CGameObject
{
    friend class CScene;

protected:
    CNPC();
    CNPC(const CNPC& obj);
    virtual ~CNPC();

private:
    ENPC_State m_State;
    class CPlayer* m_Target;
    bool m_AttackTime;
    // �ִϸ��̼� �����߿� ������ �ȵ� �� true�� �صα�
    bool m_Changing;

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Collision(float DeltaTime);
    virtual void Render(HDC hDC);
    virtual CNPC* Clone();

protected:
    // ���º� ������ ������ �Լ�
    void FiniteState();

public:
    void SetTarget(CPlayer* Target)
    {
        m_Target = Target;
    }

    CPlayer* GetTarget()    const
    {
        return m_Target;
    }

protected:
    void AddNPCAnimation();

    void CollisionBegin(CCollider* Src, CCollider* Dest,
        float DeltaTime);
    void CollisionStay(CCollider* Src, CCollider* Dest,
        float DeltaTime);
    void CollisionEnd(CCollider* Src, CCollider* Dest,
        float DeltaTime);
};

