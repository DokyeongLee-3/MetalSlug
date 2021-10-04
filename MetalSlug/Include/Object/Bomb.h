#pragma once
#include "GameObject.h"

class CBomb :
    public CGameObject
{
    friend class CScene;

private:
    CBomb();
    CBomb(const CBomb& obj);
    virtual ~CBomb();

protected:
    Vector2		m_Dir;
    float		m_ForceXDir;
    float		m_ForceYDir;
    int		m_CollisionCount;
    bool    m_StartFall;

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

    void SetCollisionCount(int Count)
    {
        m_CollisionCount = Count;
    }

    float GetXForce()   const
    {
        return m_ForceXDir;
    }

    int CollisionCount()    const
    {
        return m_CollisionCount;
    }

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Collision(float DeltaTime);
    virtual void Render(HDC hDC);
    virtual CBomb* Clone();

public:
    void CollisionBegin(class CCollider* Src, class CCollider* Dest, float DeltaTime);

};

