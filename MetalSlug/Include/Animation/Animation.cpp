
#include "Animation.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CAnimation::CAnimation()	:
	m_CurrentAnimation(nullptr),
	m_Owner(nullptr),
	m_OwnerUI(nullptr)
{
}

CAnimation::CAnimation(const CAnimation& Anim)
{
	// Anim이 const 객체이므로 iterator도 const_iterator를 사용해야한다
	// const iterator도 ++, --는 가능하다(가리키는 객체에 대해서는 변경 불가)
	std::unordered_map<std::string, AnimationInfo*>::const_iterator iter = Anim.m_mapAnimation.begin();
	std::unordered_map<std::string, AnimationInfo*>::const_iterator iterEnd = Anim.m_mapAnimation.end();
	
	for (; iter != iterEnd; ++iter)
	{
		AnimationInfo* Info = new AnimationInfo;

		Info->Sequence = iter->second->Sequence;
		Info->FrameTime = iter->second->FrameTime;
		Info->PlayTime = iter->second->PlayTime;
		Info->PlayScale = iter->second->PlayScale;
		Info->Loop = iter->second->Loop;
		Info->Reverse = iter->second->Loop;

		// Anim이 갖고 있는 여러개의 Sequence중에
		// Anim의 현재 애니메이션을 나의 현재 애니메이션으로 설정
		if (Anim.m_CurrentAnimation->Sequence->GetName() == Info->Sequence->GetName())
			m_CurrentAnimation = Info;

		m_mapAnimation.insert(std::make_pair(iter->first, Info));
	}
}

CAnimation::~CAnimation()
{
	auto iter = m_mapAnimation.begin();
	auto iterEnd = m_mapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

void CAnimation::AddAnimation(
	const std::string& SequenceName,
	bool Loop, float PlayTime,
	float PlayScale, bool Reverse)
{
	// 이미 Sequence는 SceneResource에서 load해놓았을 것이고
	// 이 함수(AddAnimation)은 Sequence가 load되고 나서
	// CGameObject들(ex.CPlayer, CMonster)의 Init에서 
	// 호출될 것이므로 SceneResource에서 해당 Sequence를 무조건
	// 찾을 수 있을 것이다
	CAnimationSequence* Sequence = m_Scene->GetSceneResource()->FindAnimationSequence(SequenceName);

	if (!Sequence)
		return;

	AnimationInfo* Info = new AnimationInfo;

	Info->Sequence = Sequence;
	Info->Loop = Loop;
	Info->PlayTime = PlayTime;
	Info->PlayScale = PlayScale;
	Info->Reverse = Reverse;
	Info->FrameTime = PlayTime / Sequence->GetFrameCount();

	// 처음 추가되는 애니메이션일 경우 이 애니메이션을
	// 기본으로 설정한다
	if (m_mapAnimation.empty())
		m_CurrentAnimation = Info;

	m_mapAnimation.insert(std::make_pair(SequenceName, Info));
}

void CAnimation::SetPlayTime(const std::string& Name,
	float PlayTime)
{
	AnimationInfo* Info = FindInfo(Name);

	if (!Info)
		return;

	Info->PlayTime = PlayTime;
}

void CAnimation::SetPlayScale(const std::string& Name, 
	float PlayScale)
{
	AnimationInfo* Info = FindInfo(Name);

	if (!Info)
		return;

	Info->PlayScale = PlayScale;
}

void CAnimation::SetCurrentAnimation(
	const std::string& Name)
{
	m_CurrentAnimation = FindInfo(Name);

	m_CurrentAnimation->Frame = 0;
	m_CurrentAnimation->Time = 0.f;

	size_t Size = m_CurrentAnimation->vecNotify.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_CurrentAnimation->vecNotify[i]->Call = false;
	}
}

void CAnimation::ChangeAnimation(const std::string& Name)
{
	if (m_CurrentAnimation->Sequence->GetName() == Name)
		return;

	// 원래 진행중인 애니메이션의 frame을 0으로 돌려놓고
	// 다른것으로 변경
	if (m_CurrentAnimation->Reverse)
		m_CurrentAnimation->Frame = m_CurrentAnimation->Sequence->GetFrameCount() - 1;
	else
		m_CurrentAnimation->Frame = 0;

	m_CurrentAnimation->Time = 0.f;

	size_t Size = m_CurrentAnimation->vecNotify.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_CurrentAnimation->vecNotify[i]->Call = false;
	}

	m_CurrentAnimation = FindInfo(Name);

	if (m_CurrentAnimation->Reverse)
		m_CurrentAnimation->Frame = m_CurrentAnimation->Sequence->GetFrameCount() - 1;
	else
		m_CurrentAnimation->Frame = 0;

	m_CurrentAnimation->Time = 0.f;
	m_CurrentAnimation->FrameTime = m_CurrentAnimation->PlayTime / m_CurrentAnimation->Sequence->GetFrameCount();
}

bool CAnimation::CheckCurrentAnimation(const std::string& Name)
{
	return m_CurrentAnimation->Sequence->GetName() == Name;
}

void CAnimation::SetReverse(const std::string& Name,
	bool Reverse)
{
	AnimationInfo* Info = FindInfo(Name);

	if (!Info)
		return;

	Info->Reverse = Reverse;
}

void CAnimation::SetLoop(const std::string& Name, 
	bool Loop)
{
	AnimationInfo* Info = FindInfo(Name);

	if (!Info)
		return;

	Info->Loop = Loop;
}

void CAnimation::Update(float DeltaTime)
{
	m_CurrentAnimation->Time += DeltaTime * m_CurrentAnimation->PlayScale;

	bool AnimEnd = false;

	if (m_CurrentAnimation->Time >= m_CurrentAnimation->FrameTime)
	{
		m_CurrentAnimation->Time -= m_CurrentAnimation->FrameTime;

		if (m_CurrentAnimation->Reverse)
		{
			--m_CurrentAnimation->Frame;

			if (m_CurrentAnimation->Frame < 0)
				AnimEnd = true;
		}

		else
		{
			++m_CurrentAnimation->Frame;

			if (m_CurrentAnimation->Frame == m_CurrentAnimation->Sequence->GetFrameCount())
				AnimEnd = true;
		}
	}

	size_t Size = m_CurrentAnimation->vecNotify.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (!m_CurrentAnimation->vecNotify[i]->Call &&
			m_CurrentAnimation->vecNotify[i]->Frame ==
			m_CurrentAnimation->Frame)
		{
			m_CurrentAnimation->vecNotify[i]->Call = true;
			m_CurrentAnimation->vecNotify[i]->Function();
		}
	}

	if (AnimEnd)
	{
		if (m_CurrentAnimation->Loop)
		{

			if (m_CurrentAnimation->Reverse)
				m_CurrentAnimation->Frame = m_CurrentAnimation->Sequence->GetFrameCount() - 1;
			else
				m_CurrentAnimation->Frame = 0;
		}

		else
		{
			// Loop 옵션이 아니면 마지막 프레임에 멈춰져 있도록
			if (m_CurrentAnimation->Reverse)
				m_CurrentAnimation->Frame = 0;

			else
				m_CurrentAnimation->Frame = m_CurrentAnimation->Sequence->GetFrameCount() - 1;
		}

		// 모션이 끝났고, EndFunction이 있다면 호출
		if (m_CurrentAnimation->EndFunction)
		{
			m_CurrentAnimation->EndFunction();
		}

		if (m_CurrentAnimation->Loop)
		{
			size_t Size = m_CurrentAnimation->vecNotify.size();

			for (size_t i = 0; i < Size; ++i)
			{
				m_CurrentAnimation->vecNotify[i]->Call = false;
			}
		}
	}

}

CAnimation* CAnimation::Clone()
{
	return new CAnimation(*this);
}

AnimationInfo* CAnimation::FindInfo(
	const std::string& Name)
{
	auto iter = m_mapAnimation.find(Name);

	if (iter == m_mapAnimation.end())
		return nullptr;

	return iter->second;
}
