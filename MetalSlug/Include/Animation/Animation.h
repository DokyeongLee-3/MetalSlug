#pragma once

#include "AnimationInfo.h"

// GameObject만 갖고 있음 -> 공유되는게 아니므로
// 멤버들도 CShredPtr을 사용하는 멤버가 없음
class CAnimation
{
	friend class CGameObject;
	friend class CPlayer;
	friend class CUIImage;
	friend class CCamelArabian;

private:
	CAnimation();
	CAnimation(const CAnimation& Anim);
	~CAnimation();

private:
	class CGameObject* m_Owner;
	class CUIImage* m_OwnerUI;
	class CScene* m_Scene;
	std::unordered_map<std::string, AnimationInfo*> m_mapAnimation;
	AnimationInfo* m_CurrentAnimation;

public:
	// Player나 Monster같은 각종 GameObject들이 
	// 사용하는 함수
	void AddAnimation(const std::string& SequenceName,
		bool Loop = true, float PlayTime = 1.f,
		float PlayScale = 1.f, bool Reverse = false);
	void SetPlayTime(const std::string& Name,
		float PlayTime);
	void SetPlayScale(const std::string& Name,
		float PlayScale);
	void SetCurrentAnimation(const std::string& Name);
	void ChangeAnimation(const std::string& Name);
	bool CheckCurrentAnimation(const std::string& Name);
	void SetReverse(const std::string& Name, bool Reverse);
	void SetLoop(const std::string& Name, bool Loop);
	void Update(float DeltaTime);
	CAnimation* Clone();

private:
	AnimationInfo* FindInfo(const std::string& Name);

public:
	AnimationInfo* GetCurrentAnimation()	const
	{
		return m_CurrentAnimation;
	}

public:
	template <typename T>
	void SetEndNotify(const std::string& SequenceName,
		T* Obj, void(T::* Func)())
	{
		AnimationInfo* Info = FindInfo(SequenceName);

		if (!Info)
			return;

		Info->EndFunction = std::bind(Func, Obj);
	}

	template <typename T>
	void AddNotify(const std::string& SequenceName,
		int Frame, T* Obj, void (T::* Func)())
	{
		AnimationInfo* Info = FindInfo(SequenceName);

		if (!Info)
			return;

		AnimationNotify* Notify = new AnimationNotify;

		Notify->Frame = Frame;
		Notify->Function = std::bind(Func, Obj);

		Info->vecNotify.push_back(Notify);
	}

};

