#pragma once
#include "Collider.h"

class CColliderSphere :
    public CCollider
{
    friend class CGameObject;

protected:
    CColliderSphere();
    CColliderSphere(const CColliderSphere& collider);
    ~CColliderSphere();

protected:
    SphereInfo m_Info;
    bool m_CamCollision;
    bool m_FloorCollision;

public:
    void SetRadius(float Radius)
    {
        m_Info.Radius = Radius;
    }

    SphereInfo GetInfo() const
    {
        return m_Info;
    }

    virtual float GetBottom()	const
    {
        return m_Info.Center.y + m_Info.Radius;
    }

    void SetCamCollision(bool Collision)
    {
        m_CamCollision = Collision;
    }

    bool GetCamCollision()	const
    {
        return m_CamCollision;
    }

    void SetFloorCollision(bool Collision)
    {
        m_FloorCollision = Collision;
    }

    bool GetFloorCollision()	const
    {
        return m_FloorCollision;
    }

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render(HDC hDC);
    virtual CColliderSphere* Clone();
    virtual bool Collision(CCollider* Dest);
};

