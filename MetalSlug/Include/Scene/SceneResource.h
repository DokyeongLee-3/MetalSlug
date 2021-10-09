#pragma once

#include "../Resource/Texture.h"
#include "../Resource/AnimationSequence.h"
#include "../Resource/Sound.h"

class CSceneResource
{
	friend class CScene;

private:
	CSceneResource();
	~CSceneResource();

private:
	std::unordered_map<std::string, CSharedPtr<CTexture>>	m_mapTexture;
	std::unordered_map<std::string, CSharedPtr<CAnimationSequence>>	m_mapAnimationSequence;
	std::unordered_map<std::string, CSharedPtr<CSound>> m_mapSound;

public:
	bool LoadTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	// 애니메이션을 Frame방식으로 여러장 load하려고 할 때
	bool LoadTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name,
		const TCHAR* FullPath);
	CTexture* FindTexture(const std::string& Name);
	void SetTextureColorKey(const std::string& Name,
		const unsigned char r, const unsigned char g,
		const unsigned char b, int Index = 0);

public:
	// CreateAnimationSequence함수들은 Texture(이미 load 되었다면
	// 1번째 CreateAnimationSequence를 사용, load를 이제 해야한다면
	// 2번째나 3번째 CreateAnimationSequence를 사용)와 함께
	// CAnimationSequence를 binding하는 동작을 한다.


	// 이미 texture가 load되어서 CSceneResource의
	// m_mapAnimationSequence와 CResourceManager의
	// m_mapAnimationSequence에 TextureName을 가진
	// Texture가 이미 들어 있는 경우
	bool CreateAnimationSequence(const std::string& SequenceName,
		const std::string& TextureName);

	// TextureName을 가진 Texture를 load하면서 동시에
	// 그 Texture와 CAnimationSequence를 binding 해줘야 
	// 할 때 사용하는 함수
	bool CreateAnimationSequence(const std::string& SequenceName,
		const std::string& TextureName,
		const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	bool CreateAnimationSequence(const std::string& SequenceName,
		const std::string& TextureName,
		const std::vector<std::wstring>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	// 위에 CreateAnimationSequence랑 다르게 CResourceManage 거치지 않고
	// 바로 CSceneResource의 m_mapAnimationSequence에서 Sequence 찾아서
	// Sequence의 AddFrameData 호출 
	void AddAnimationFrameData(const std::string& SequenceName,
		const Vector2& StartPos,
		const Vector2& Size);
	void AddAnimationFrameData(const std::string& SequenceName,
		const AnimationFrameData& Data);
	void AddAnimationFrameData(const std::string& SequenceName,
		float PosX, float PosY, float SizeX, float SizeY);
	void SetAnimationFrameData(const std::string& SequenceName,
		const AnimationFrameData& Data, int Index);

	CAnimationSequence* FindAnimationSequence(const std::string& Name);


public:
	bool LoadSound(const std::string& GroupName,
		bool Loop, const std::string& Name, const char* FileName,
		const std::string& PathName = SOUND_PATH);
	bool SetVolume(int Volume);
	bool SetVolume(const std::string& GroupName, int Volume);
	bool SoundPlay(const std::string& Name);
	bool SoundStop(const std::string& Name);
	bool SoundPause(const std::string& Name);
	bool SoundResume(const std::string& Name);

	CSound* FindSound(const std::string& Name);
};

