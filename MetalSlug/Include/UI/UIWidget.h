#pragma once
#include "../Ref.h"

// UI클래스들의 부모클래스
class CUIWidget :
    public CRef
{
public:
    CUIWidget();
    CUIWidget(const CUIWidget& widget);
    virtual ~CUIWidget();

protected:
    class CScene* m_Scene;
    // 이 값이 높으면 우선순위가 높아서 나중에 그려줌
    int m_ZOrder;
    // UIWindow의 Visibility와는 별개로 UIWidget도
    // 각각의 m_Visibility 멤버를 갖고 있다.
    bool m_Visibility;
    class CUIWindow* m_Owner;
    Vector2     m_Pos;
    Vector2     m_Size;
    // CButton같은 클래스에서 m_MouseHovered이면
    // ButtonState바꾸려고 만든 멤버
    bool    m_MouseHovered;

public:
    Vector2 GetPos()    const
    {
        return m_Pos;
    }

    Vector2 GetSize()   const
    {
        return m_Size;
    }

    void SetPos(const Vector2& Pos)
    {
        m_Pos = Pos;
    }

    void SetPos(float x, float y)
    {
        m_Pos = Vector2(x, y);
    }

    void SetSize(const Vector2& Size)
    {
        m_Size = Size;
    }

    void SetSize(float x, float y)
    {
        m_Size = Vector2(x, y);
    }

    void SetOwner(class CUIWindow* Owner)
    {
        m_Owner = Owner;
    }

    void SetScene(class CScene* Scene)
    {
        m_Scene = Scene;
    }

    void SetVisibility(bool Visibility)
    {
        m_Visibility = Visibility;
    }

    bool GetVisibility()    const
    {
        return m_Visibility;
    }

    void SetZOrder(int ZOrder)
    {
        m_ZOrder = ZOrder;
    }

    int GetZOrder() const
    {
        return m_ZOrder;
    }

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Collision(float DeltaTime);
    virtual void Render(HDC hDC);

public:
    bool CollisionMouse(const Vector2& MousePos, float DeltaTime);

public:
    // Widget영역으로 마우스가 올라왔을 때 콜백함수 
    virtual void CollisionMouseHoveredCallback(float DeltaTime);
    // Widget영역 바깥으로 마우스가 빠져나갔을 때 콜백함수
    virtual void CollisionMouseReleaseCallback(float DeltaTime);
};

