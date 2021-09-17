#pragma once

#include "../Resource/AnimationSequence.h"


struct AnimationNotify
{
	// 몇번째 프레임에 Function을 호출할 것인지
	int Frame;
	// AnimationNotify에서 Call멤버는 이미 Function이
	// 호출됐는지 확인하는 용도 만약에 한 프레임당
	// 1초인데 Animation::Update가 1초안에 여러번 호출되면
	// vecNotify의 번째 인덱스에 bind된 함수가 한번만 호출되야
	// 하는데 여러번 불릴것임 따라서 bool변수를 둬서
	// 호출 됐는지 안됐는지 파악하는 용도로 사용한다 
	bool Call;
	std::function<void()> Function;

	AnimationNotify() :
		Frame(0),
		Call(false)
	{

	}
};


// AnimationInfo는 Animation 클래스에서만 들고 있기 때문에
// 공유되지 않는다 -> CSharedPtr 사용 X
struct AnimationInfo
{
	// AnimationSequence는 공유되니까(by ResourceManager,
	// CSceneResource, AnimationInfo) SharedPtr을 사용
	CSharedPtr<CAnimationSequence> Sequence;
	// 현재 애니메이션이 몇번째 프레임을 렌더링중인지
	int Frame;
	// 이번 프레임 시작하고 얼마나 지났는지
	float Time;
	// 각 프레임별 시간
	float FrameTime;
	// 모션 한번 돌때 필요한 시간
	float PlayTime;
	// 재생 배율
	float PlayScale;
	bool Loop;
	bool Reverse;

	std::function<void()> EndFunction;
	std::vector<AnimationNotify*> vecNotify;

	AnimationInfo() :
		Frame(0),
		Time(0.f),
		FrameTime(0.f),
		PlayTime(0.f),
		Loop(false),
		Reverse(false)
	{
	}
	
	~AnimationInfo()
	{
		size_t	Size = vecNotify.size();

		for (size_t i = 0; i < Size; ++i)
		{
			SAFE_DELETE(vecNotify[i]);
		}
	}

};