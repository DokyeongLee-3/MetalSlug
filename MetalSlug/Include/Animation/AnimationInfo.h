#pragma once

#include "../Resource/AnimationSequence.h"


struct AnimationNotify
{
	// ���° �����ӿ� Function�� ȣ���� ������
	int Frame;
	// AnimationNotify���� Call����� �̹� Function��
	// ȣ��ƴ��� Ȯ���ϴ� �뵵 ���࿡ �� �����Ӵ�
	// 1���ε� Animation::Update�� 1�ʾȿ� ������ ȣ��Ǹ�
	// vecNotify�� ��° �ε����� bind�� �Լ��� �ѹ��� ȣ��Ǿ�
	// �ϴµ� ������ �Ҹ����� ���� bool������ �ּ�
	// ȣ�� �ƴ��� �ȵƴ��� �ľ��ϴ� �뵵�� ����Ѵ� 
	bool Call;
	std::function<void()> Function;

	AnimationNotify() :
		Frame(0),
		Call(false)
	{

	}
};


// AnimationInfo�� Animation Ŭ���������� ��� �ֱ� ������
// �������� �ʴ´� -> CSharedPtr ��� X
struct AnimationInfo
{
	// AnimationSequence�� �����Ǵϱ�(by ResourceManager,
	// CSceneResource, AnimationInfo) SharedPtr�� ���
	CSharedPtr<CAnimationSequence> Sequence;
	// ���� �ִϸ��̼��� ���° �������� ������������
	int Frame;
	// �̹� ������ �����ϰ� �󸶳� ��������
	float Time;
	// �� �����Ӻ� �ð�
	float FrameTime;
	// ��� �ѹ� ���� �ʿ��� �ð�
	float PlayTime;
	// ��� ����
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