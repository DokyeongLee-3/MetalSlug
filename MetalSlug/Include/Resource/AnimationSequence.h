#pragma once

#include "Texture.h"

// CResourceManager::ReleaseAnimationSequence에서
// m_mapAniamationSequence의 element를 erase하면
// CSharedPtr의 소멸자가 호출되면서 
// CAnimationSequence가 Release를 호출할 것이므로
// CRef를 상속받아줘야 한다
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
	// CResourceManager::CreateAnimation에서 CAnimationSequence를 동적할당하고
	// CAnimationSequence::Init을 호출
	bool Init(const std::string& Name, CTexture* Texture);
	// CSceneResource::AddAnimationFrameData에서 아래 함수 호출
	void AddFrameData(const Vector2& StartPos, const Vector2& Size);
	void AddFrameData(float PosX, float PosY, float SizeX, float SizeY);
};

