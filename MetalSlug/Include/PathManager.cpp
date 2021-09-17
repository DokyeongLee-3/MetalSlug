#include "PathManager.h"

CPathManager* CPathManager::m_Inst = nullptr;

CPathManager::CPathManager()
{
}

CPathManager::~CPathManager()
{
	auto iter = m_mapPath.begin();
	auto iterEnd = m_mapPath.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

}

bool CPathManager::Init()
{
	// Bin 폴더까지의 경로를 RootPath로 지정하고
	// Bin 폴더까지의 경로를 얻어온다
	TCHAR Path[MAX_PATH] = {};

	// 현재 실행파일이 있는 경로 + 실행파일.exe 까지해서
	// 전체 경로를 얻어온다
	GetModuleFileName(0, Path, MAX_PATH);

	int Length = lstrlen(Path);

	for (int i = Length - 1; i > 0; --i)
	{
		// 역슬래쉬같은 특수문자는 두번 써줘야 인식가능
		if (Path[i] == '\\')
		{
			memset(&Path[i + 1], 0, sizeof(TCHAR) * Length - i - 1);
			break;
		}
	}

	PathInfo* Info = new PathInfo;

	lstrcpy(Info->Path, Path);

#ifdef UNICODE
	// 유니코드 문자열을 멀티바이트 문자열로 변환한다
	// 4번째 인자로 -1을 주면 3번째 인자 전체 길이 만큼 변환한다는 의미
	// 만약 5번째 인자로 nullptr을 주면, 3번째 인자를 멀티바이트로 변환하는데
	// 필요한 바이트수를 리턴해준다(뒤에 나머지 인자들은 0으로)
	int ConvertLength = WideCharToMultiByte(CP_ACP, 0, Path, -1, nullptr, 0, 0, 0);

	WideCharToMultiByte(CP_ACP, 0, Path, -1, Info->PathMultibyte, ConvertLength, 0, 0);
#else
	strcpy_s(Info->PathMultibyte, Path);
#endif	//UNICODE

	m_mapPath.insert(std::make_pair(ROOT_PATH, Info));

	// RootPath에 Texture를 붙여서 TexturePath도 생성
	AddPath(TEXTURE_PATH, TEXT("Texture\\"));
	AddPath(SOUND_PATH, TEXT("Sound\\"));

	return true;
}

bool CPathManager::AddPath(const std::string& Name,
	const TCHAR* Path, const std::string& BaseName)
{
	if (FindPath(Name))
		return false;

	const PathInfo* BaseInfo = FindPath(BaseName);

	if (!BaseInfo)
		return false;

	PathInfo* Info = new PathInfo;

	lstrcpy(Info->Path, BaseInfo->Path);
	lstrcat(Info->Path, Path);

#ifdef UNICODE
	int ConvertLength = WideCharToMultiByte(CP_ACP, 0, Info->Path,
		-1, nullptr, 0, 0, 0);

	WideCharToMultiByte(CP_ACP, 0, Info->Path, -1, Info->PathMultibyte,
		ConvertLength, 0, 0);

#else
	strcpy_s(Info->PathMultiByte, Info->Path);
#endif	// UNICODE


	m_mapPath.insert(std::make_pair(Name, Info));

	return true;
}

const PathInfo* CPathManager::FindPath(const std::string& Name)
{
	auto iter = m_mapPath.find(Name);

	if (iter == m_mapPath.end())
		return nullptr;

	return iter->second;
}
