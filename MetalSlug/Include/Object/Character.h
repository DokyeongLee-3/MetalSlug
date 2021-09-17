#pragma once
#include "GameObject.h"

class CCharacter :
    public CGameObject
{
    friend class CScene;

protected:
    CCharacter();
	CCharacter(const CCharacter& obj);
    virtual ~CCharacter();

protected:
	CharacterInfo m_CharacterInfo;

public:
	void SetCharacterInfo(int Attack, int Armor, int HP,
		int MP, int Level, int Exp, int Gold, float AttackSpeed,
		float AttackDistance)
	{
		m_CharacterInfo.Attack = Attack;
		m_CharacterInfo.Armor = Armor;
		m_CharacterInfo.HP = HP;
		m_CharacterInfo.HPMax = HP;
		m_CharacterInfo.MP = MP;
		m_CharacterInfo.MPMax = MP;
		m_CharacterInfo.Level = Level;
		m_CharacterInfo.Exp = Exp;
		m_CharacterInfo.Gold = Gold;
		m_CharacterInfo.AttackSpeed = AttackSpeed;
		m_CharacterInfo.AttackDistance = AttackDistance;
	}

	// 실질적으로 공격 속도를 늦추려면 공격모션에 대한 animation 
	// SequenceName을 알아야하는데 그건 CPlayer::Init에서 결정되기 때문에
	// 여기서는 모른다. 그런데 CCharacter에 해당되는 Player, Monster 모두
	// AttackSpeed를 조절하게 하고 싶으니 SetAttackSpeed를 virtual로 잡고
	// CPlayer같은 SetAttackSpeed를 자식에서 재정의해서, 자식에서 아래
	// 함수를 불러서 AttackSpeed를 설정하고, 어떤 이름의 공격 애니메이션에 
	// 변경된 AttackSpeed를 설정할건지는 각자 자식 클래스에서 재정의된
	// SetAttackSpeed에서 설정하기로 한다
	virtual void SetAttackSpeed(float Speed)
	{
		m_CharacterInfo.AttackSpeed = Speed;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CCharacter* Clone();
};

