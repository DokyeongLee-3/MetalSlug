
#include "ResourceManager.h"

CResourceManager* CResourceManager::m_Inst = nullptr;

CResourceManager::CResourceManager()	:
	m_System(nullptr),
	m_MasterGroup(nullptr)
{
}

CResourceManager::~CResourceManager()
{
	// 이 안의 CSharedPtr<CSound>들 중에 ref count가
	// 0이 되는게 있다면 그 CSound*타입의 객체는 저절로
	// SharedPtr에서 Release를 호출해서 
	// CSound* 타입의 객체를 delete하려 해서
	// CSound 소멸자가 호출될 것이고 CSound 소멸자 안에서
	// Sound에 대해 FMOD의 release를 호출할 것임
	m_mapSound.clear();
	m_mapAnimationSequence.clear();
	m_mapTexture.clear();

	{
		auto	iter = m_mapChannelGroup.begin();
		auto	iterEnd = m_mapChannelGroup.end();

		for (; iter != iterEnd; ++iter)
		{
			iter->second->release();
		}

		m_mapChannelGroup.clear();
	}

	if (m_System)
	{
		m_System->close();
		m_System->release();
	}
}

bool CResourceManager::Init()
{
	FMOD_RESULT result = FMOD::System_Create(&m_System);

	if (result != FMOD_OK)
		return false;

	// System을 초기화한다.
	result = m_System->init(128, FMOD_INIT_NORMAL, nullptr);

	if (result != FMOD_OK)
		return false;

	// Master Channel Group을 얻어온다.
	result = m_System->getMasterChannelGroup(&m_MasterGroup);

	if (result != FMOD_OK)
		return false;

	m_mapChannelGroup.insert(std::make_pair("Master", m_MasterGroup));

	CreateSoundChannelGroup("BGM");
	CreateSoundChannelGroup("Effect");
	CreateSoundChannelGroup("UI");

	return true;
}

bool CResourceManager::LoadTexture(const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{
	CTexture* Texture = FindTexture(Name);

	if(Texture)
		return true;

	Texture = new CTexture;

	if (!Texture->LoadTexture(Name, FileName, PathName))
	{
		SAFE_RELEASE(Texture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

bool CResourceManager::LoadTextureFullPath(const std::string& Name,
	const TCHAR* FullPath)
{
	CTexture* Texture = FindTexture(Name);

	if (Texture)
		return true;

	Texture = new CTexture;

	if (!Texture->LoadTextureFullPath(Name, FullPath))
	{
		SAFE_RELEASE(Texture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

bool CResourceManager::LoadTexture(const std::string& Name, 
	const std::vector<std::wstring>& vecFileName, 
	const std::string& PathName)
{
	CTexture* Texture = FindTexture(Name);

	if (Texture)
		return true;

	Texture = new CTexture;

	if (!Texture->LoadTexture(Name, vecFileName, PathName))
	{
		SAFE_RELEASE(Texture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

void CResourceManager::SetTextureColorKey(const std::string& Name, 
	const unsigned char r, const unsigned char g,
	const unsigned char b, int Index)
{
	CTexture* Texture = FindTexture(Name);

	if (!Texture)
		return;

	Texture->SetColorKey(r, g, b, Index);
}

void CResourceManager::ReleaseTexture(const std::string& Name)
{
	auto iter = m_mapTexture.find(Name);

	// ResoureManager만 m_mapTexture에 갖고 참조하고 있다는 뜻
	if (iter->second->GetRefCount() == 1)
	{
		// SharedPtr의 소멸자 호출
		m_mapTexture.erase(iter);
	}

}

CTexture* CResourceManager::FindTexture(const std::string& Name)
{
	auto iter = m_mapTexture.find(Name);

	if (iter == m_mapTexture.end())
		return nullptr;

	return iter->second;
}

bool CResourceManager::CreateAnimationSequence(
	const std::string& SequenceName,
	const std::string& TextureName)
{
	CAnimationSequence* Sequence = FindAnimationSequence(SequenceName);

	if (Sequence)
		return true;

	Sequence = new CAnimationSequence;

	CTexture* Texture = FindTexture(TextureName);

	if (!Sequence->Init(SequenceName, Texture))
	{
		SAFE_DELETE(Sequence);
		return false;
	}

	m_mapAnimationSequence.insert(std::make_pair(SequenceName, Sequence));

	return true;
}

bool CResourceManager::CreateAnimationSequence(
	const std::string& SequenceName,
	const std::string& TextureName, 
	const TCHAR* FileName, const std::string& PathName)
{
	CAnimationSequence* Sequence = FindAnimationSequence(SequenceName);

	if (Sequence)
		return true;

	if (!LoadTexture(TextureName, FileName, PathName))
		return false;

	Sequence = new CAnimationSequence;

	CTexture* Texture = FindTexture(TextureName);

	if (!Sequence->Init(SequenceName, Texture))
	{
		SAFE_RELEASE(Sequence);
		return false;
	}

	m_mapAnimationSequence.insert(std::make_pair(SequenceName, Sequence));

	return true;
}

bool CResourceManager::CreateAnimationSequence(
	const std::string& SequenceName, 
	const std::string& TextureName, 
	const std::vector<std::wstring>& vecFileName, 
	const std::string& PathName)
{
	CAnimationSequence* Sequence = FindAnimationSequence(SequenceName);

	if (Sequence)
		return true;

	if (!LoadTexture(TextureName, vecFileName, PathName))
		return false;

	Sequence = new CAnimationSequence;

	CTexture* Texture = FindTexture(TextureName);

	if (!Sequence->Init(SequenceName, Texture))
	{
		SAFE_RELEASE(Sequence);
		return false;
	}

	m_mapAnimationSequence.insert(std::make_pair(SequenceName, Sequence));

	return true;
}

void CResourceManager::AddAnimationFrameData(
	const std::string& SequenceName, 
	const Vector2& StartPos, const Vector2& Size)
{
	CAnimationSequence* Sequence = FindAnimationSequence(SequenceName);

	if (!Sequence)
		return;

	Sequence->AddFrameData(StartPos, Size);
}

void CResourceManager::AddAnimationFrameData(
	const std::string& SequenceName, 
	float PosX, float PosY, float SizeX, float SizeY)
{
	CAnimationSequence* Sequence = FindAnimationSequence(SequenceName);

	if (!Sequence)
		return;

	Sequence->AddFrameData(PosX, PosY, SizeX, SizeY);
}

void CResourceManager::ReleaseAnimationSequence(
	const std::string& Name)
{
	auto	iter = m_mapAnimationSequence.find(Name);

	if (iter->second->GetRefCount() == 1)
		m_mapAnimationSequence.erase(iter);
}

CAnimationSequence* CResourceManager::FindAnimationSequence(const std::string& Name)
{
	auto iter = m_mapAnimationSequence.find(Name);

	if (iter == m_mapAnimationSequence.end())
		return nullptr;

	return iter->second;
}

bool CResourceManager::LoadSound(const std::string& GroupName, bool Loop, const std::string& Name, const char* FileName, const std::string& PathName)
{
	CSound* Sound = FindSound(Name);

	if (Sound)
		return true;

	FMOD::ChannelGroup* Group = FindSoundChannelGroup(GroupName);

	if (!Group)
		return false;

	Sound = new CSound;

	if (!Sound->LoadSound(m_System, Group, Loop, Name, FileName, PathName))
	{
		SAFE_RELEASE(Sound);
		return false;
	}

	m_mapSound.insert(std::make_pair(Name, Sound));

	return true;
}

bool CResourceManager::CreateSoundChannelGroup(const std::string& Name)
{
	FMOD::ChannelGroup* Group = FindSoundChannelGroup(Name);

	if (Group)
		return true;

	FMOD_RESULT result = m_System->createChannelGroup(Name.c_str(), &Group);

	if (result != FMOD_OK)
		return false;

	// 생성한 그룹을 마스터 그룹에 추가해준다.
	m_MasterGroup->addGroup(Group, false);

	m_mapChannelGroup.insert(std::make_pair(Name, Group));

	return true;
}

bool CResourceManager::SetVolume(int Volume)
{
	m_MasterGroup->setVolume(Volume / 100.f);

	return true;
}

bool CResourceManager::SetVolume(const std::string& GroupName, int Volume)
{
	FMOD::ChannelGroup* Group = FindSoundChannelGroup(GroupName);

	if (!Group)
		return false;

	Group->setVolume(Volume / 100.f);

	return true;
}

bool CResourceManager::SoundPlay(const std::string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return false;

	Sound->Play();

	return true;
}

bool CResourceManager::SoundStop(const std::string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return false;

	Sound->Stop();

	return true;
}

bool CResourceManager::SoundPause(const std::string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return false;

	Sound->Pause();

	return true;
}

bool CResourceManager::SoundResume(const std::string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return false;

	Sound->Resume();

	return true;
}

void CResourceManager::ReleaseSound(const std::string& Name)
{
	auto iter = m_mapSound.find(Name);

	if (iter->second->GetRefCount() == 1)
		m_mapSound.erase(iter);
}

FMOD::ChannelGroup* CResourceManager::FindSoundChannelGroup(const std::string& Name)
{
	auto iter = m_mapChannelGroup.find(Name);

	if (iter == m_mapChannelGroup.end())
		return nullptr;

	return iter->second;
}

CSound* CResourceManager::FindSound(const std::string& Name)
{
	auto iter = m_mapSound.find(Name);

	if (iter == m_mapSound.end())
		return nullptr;

	return iter->second;
}
