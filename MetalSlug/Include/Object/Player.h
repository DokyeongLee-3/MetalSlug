#pragma once
#include "Character.h"
#include "Bullet.h"

class CPlayer :
    public CCharacter
{
    friend class CScene;

private:
    CPlayer();
    CPlayer(const CPlayer& obj);
    virtual ~CPlayer();

public:
    virtual void SetAttackSpeed(float Speed)
    {
        CCharacter::SetAttackSpeed(Speed);

        SetAnimationPlayScale("LucidNunNaRightAttack", Speed);
    }

protected:
    CAnimation* m_TopAnimation;
    CAnimation* m_BottomAnimation;

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Collision(float DeltaTime);
    virtual void PrevRender();
    virtual void Render(HDC hDC);
    virtual CPlayer* Clone();

private:
    void Jump(float DeltaTime);
    void Down(float DeltaTime);
    void MoveLeft(float DeltaTime);
    void MoveRight(float DeltaTime);
    // 총쏘는 애니메이션으로 바꿔주기만 하는 함수
    void BulletFire(float DeltaTime);
    void Pause(float DeltaTime);
    void Resume(float DeltaTime);
    
    // 총쏘는 애니메이션의 EndNotify로 설정해두고
    // 총알 prototype을 만드는 함수
    void CloneBullet();

private:
    void CreateTopAnimation();
    void CreateBottomAnimation();
    void AddTopAnimation(const std::string& SequenceName,
        bool Loop = true, float PlayTime = 1.f, float PlayScale = 1.f,
        bool Reverse = false);
    void AddBottomAnimation(const std::string& SequenceName,
        bool Loop = true, float PlayTime = 1.f, float PlayScale = 1.f,
        bool Reverse = false);
    void SetTopAnimationPlayTime(const std::string& Name,
        float PlayTime);
    void SetBottomAnimationPlayTime(const std::string& Name,
        float PlayTime);
    void SetTopAnimationPlayScale(const std::string& Name,
        float PlayScale);
    void SetBottomAnimationPlayScale(const std::string& Name,
        float PlayScale);
    void SetTopCurrentAnimation(const std::string& Name);
    void SetBottomCurrentAnimation(const std::string& Name);
    void ChangeTopAnimation(const std::string& Name);
    void ChangeBottomAnimation(const std::string& Name);
    bool CheckTopCurrentAnimation(const std::string& Name);
    bool CheckBottomCurrentAnimation(const std::string& Name);
    void SetTopAnimationReverse(const std::string& Name,
        bool Reverse);
    void SetBottomAnimationReverse(const std::string& Name,
        bool Reverse);
    void SetTopAnimationLoop(const std::string& Name,
        bool Loop);
    void SetBottomAnimationLoop(const std::string& Name,
        bool Loop);

public:
    void TopAttackEnd();
    void BottomAttackEnd();

    void CollisionBegin(CCollider* Src, CCollider* Dest,
        float DeltaTime);
    void CollisionStay(CCollider* Src, CCollider* Dest,
        float DeltaTime);

public:
    template <typename T>
    void AddTopAnimationNotify(const std::string SequenceName,
        int Frame, T* Obj, void (T::* Func)())
    {
        m_TopAnimation->AddNotify<T>(SequenceName, Frame,
            Obj, Func);
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

