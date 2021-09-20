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
    virtual void Render(HDC hDC);
    virtual CPlayer* Clone();

private:
    void MoveUp(float DeltaTime);
    void MoveDown(float DeltaTime);
    void MoveLeft(float DeltaTime);
    void MoveRight(float DeltaTime);
    void BulletFire(float DeltaTime);
    void Pause(float DeltaTime);
    void Resume(float DeltaTime);

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
    void AttackEnd();
    void Fire();

    void Skill1End();
    void Skill1Enable();
};

