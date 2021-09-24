#pragma once
#include "GameObject.h"
class CBullet :
    public CGameObject
{
    friend class CScene;

private:
    CBullet();
    CBullet(const CBullet& obj);
    virtual ~CBullet();

protected:
    Vector2		m_Dir;
    float		m_Speed;
    float		m_Distance;

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

    void SetSpeed(float Speed)
    {
        m_Speed = Speed;
    }

    void SetDistance(float Distance)
    {
        m_Distance = Distance;
    }

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Collision(float DeltaTime);
    virtual void Render(HDC hDC);
    virtual CBullet* Clone();

public:
    void CollisionBegin(class CCollider* Src, class CCollider* Dest, float DeltaTime);
   
};

