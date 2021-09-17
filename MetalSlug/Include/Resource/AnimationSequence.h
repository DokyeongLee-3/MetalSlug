#pragma once

#include "Texture.h"

// CResourceManager::ReleaseAnimationSequence����
// m_mapAniamationSequence�� element�� erase�ϸ�
// CSharedPtr�� �Ҹ��ڰ� ȣ��Ǹ鼭 
// CAnimationSequence�� Release�� ȣ���� ���̹Ƿ�
// CRef�� ��ӹ޾���� �Ѵ�
class CAnimationSequence	:
	public CRef
{

	friend class CResourceManager;

private:
	CAnimationSequence();
	~CAnimationSequence();

private:
	ETexture_Type	m_TextureType;
	CSharedPtr<CTexture>	m_Texture;
	std::vector<AnimationFrameData> m_vecFrameData;

public:
	CTexture* GetTexture()
	{
		return m_Texture;
	}

	ETexture_Type GetTextureType()	const
	{
		return m_TextureType;
	}

	const AnimationFrameData& GetFrameData(int Index)	const
	{
		return m_vecFrameData[Index];
	}

	int GetFrameCount()	const
	{
		return (int)m_vecFrameData.size();
	}

public:
	// CResourceManager::CreateAnimation���� CAnimationSequence�� �����Ҵ��ϰ�
	// CAnimationSequence::Init�� ȣ��
	bool Init(const std::string& Name, CTexture* Texture);
	// CSceneResource::AddAnimationFrameData���� �Ʒ� �Լ� ȣ��
	void AddFrameData(const Vector2& StartPos, const Vector2& Size);
	void AddFrameData(float PosX, float PosY, float SizeX, float SizeY);
};

