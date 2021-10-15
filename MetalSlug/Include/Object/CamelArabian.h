#pragma once

#include "Monster.h"
#include "Player.h"

class CCamelArabian :
    public CMonster
{
    friend class CScene;

private:
    ECamelArabian_State m_State;
    CPlayer* m_Target;
    // 애니메이션 동작중에 끊으면 안될 때 true로 해두기
    bool m_Changing;
    CAnimation* m_TopAnimation;
    CAnimation* m_BottomAnimation;

protected:
    CCamelArabian();
    CCamelArabian(const CCamelArabian& obj);
    virtual ~CCamelArabian();

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Collision(float DeltaTime);
    virtual void Render(HDC hDC);
    virtual CCamelArabian* Clone();

protected:
    // CamelArabian의 상태별 동작을 정의한 함수
    void FiniteState();


    void CollisionBegin(CCollider* Src, CCollider* Dest,
        float DeltaTime);
    void CollisionStay(CCollider* Src, CCollider* Dest,
        float DeltaTime);
    void CollisionEnd(CCollider* Src, CCollider* Dest,
        float DeltaTime);

private:
    void AddTopAnimation(const std::string& SequenceName,
        bool Loop = true, float PlayTime = 1.f, float PlayScale = 1.f,
        bool Reverse = false);
    void AddBottomAnimation(const std::string& SequenceName,
        bool Loop = true, float PlayTime = 1.f, float PlayScale = 1.f,
        bool Reverse = false);
    void ChangeBottomAnimation(const std::string& Name);
    void ChangeTopAnimation(const std::string& Name);
    bool CheckTopCurrentAnimation(const std::string& Name);
    bool CheckBottomCurrentAnimation(const std::string& Name);
    void AddCamelArabianAnimation();

public:
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

public:
    template <typename T>
    void AddBottomAnimationNotify(const std::string SequenceName,
        int Frame, T* Obj, void (T::* Func)())
    {
        m_BottomAnimation->AddNotify<T>(SequenceName, Frame,
            Obj, Func);
    }

public:
    template <typename T>
    void SetTopAnimationEndNotify(const std::string SequenceName,
        T* Obj, void (T::* Func)())
    {
        m_TopAnimation->SetEndNotify<T>(SequenceName,
            Obj, Func);
    }

public:
    template <typename T>
    void SetBottomAnimationEndNotify(const std::string SequenceName,
        T* Obj, void (T::* Func)())
    {
        m_BottomAnimation->SetEndNotify<T>(SequenceName,
            Obj, Func);
    }
};

