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
    void Skill1(float DeltaTime);

public:
    void AttackEnd();
    void Fire();

    void Skill1End();
    void Skill1Enable();
};
