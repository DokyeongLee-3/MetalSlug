#pragma once
#include "GameObject.h"

class CKnife :
    public CGameObject
{

    friend class CScene;

private:
    CKnife();
    CKnife(const CKnife& obj);
    virtual ~CKnife();

protected:
    Vector2		m_Dir;
    float		m_ForceXDir;
    float		m_ForceYDir;
    bool    m_StartFall;
    // ���� �����ִ� ��������
    bool    m_Stuck;

public:
    void SetDir(float x, float y)
    {
        m_Dir.x = x;
        m_Dir.y = y;
    }

    void SetDir(float Angle)
    {
        m_Dir.x = cosf(DegreeToRadian(Angle));
        m_Dir.y = sinf(DegreeToRadian(Angle));
    }

    void SetXForce(float Force)
    {
        m_ForceXDir = Force;
    }

    float GetXForce()   const
    {
        return m_ForceXDir;
    }


public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Collision(float DeltaTime);
    virtual void Render(HDC hDC);
    virtual CKnife* Clone();

public:
    void CollisionBegin(class CCollider* Src, class CCollider* Dest, float DeltaTime);


private:
    // Į�� ���̶� �浹�ż� ������ �� ȣ���ϴ� EndNotify
    void DestroyAfterStuck();
};

