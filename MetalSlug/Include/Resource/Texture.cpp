
#include "Texture.h"
#include "../PathManager.h"
#include "../GameManager.h"

CTexture::CTexture()
{
}

CTexture::~CTexture()
{
	size_t Size = m_vecTextureInfo.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecTextureInfo[i]);
	}
}

bool CTexture::LoadTexture(const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{
	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	TCHAR FullPath[MAX_PATH] = {};

	if (Info)
		lstrcpy(FullPath, Info->Path);
	lstrcat(FullPath, FileName);

	return LoadTextureFullPath(Name, FullPath);
}

bool CTexture::LoadTextureFullPath(const std::string& Name,
	const TCHAR* FullPath)
{
	SetName(Name);

	TextureInfo* Info = new TextureInfo;

	// 1. memoryDC 만들기(그리기 도구)
	Info->hDC = CreateCompatibleDC(CGameManager::GetInst()->GetWindowDC());

	// 2. 어떤 그림을 그릴지에 대한 정보를 LoadImage로 얻어와서
	// HBITMAP타입의 변수에 저장하기
	Info->hBmp = (HBITMAP)LoadImage(CGameManager::GetInst()->GetWindowInstance(),
		FullPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	// 3. memoryDC라는 그리기 도구에다가 어떤 그림을 
	// 그릴지에 대한 정보(HBITMAP 타입 변수)를 
	// SelectObject로 설정해주기
	Info->hPrev = (HBITMAP)SelectObject(Info->hDC, Info->hBmp);

	// 4. 비트맵 정보(ex.가로 / 세로 크기)를 GetObject로 얻어와서 BITMAP타입 변수에 저장하기
	GetObject(Info->hBmp, sizeof(BITMAP), &Info->BmpInfo);

	m_vecTextureInfo.push_back(Info);

	if (m_vecTextureInfo.size() == 1)
		m_TextureType = ETexture_Type::Atlas;

	else
		m_TextureType = ETexture_Type::Frame;

	return true;
}

bool CTexture::LoadTexture(const std::string& Name,
	const std::vector<std::wstring>& vecFileName,
	const std::string& PathName)
{
	const PathInfo* Info = CPathManager::GetInst()->FindPath(Name);

	size_t Size = vecFileName.size();

	for (size_t i = 0; i < Size; ++i)
	{
		TCHAR FullPath[MAX_PATH] = {};

		if (Info)
			lstrcpy(FullPath, Info->Path);
		lstrcat(FullPath, vecFileName[i].c_str());

		if (!LoadTexture(Name, FullPath))
			return false;
	}

	if (m_vecTextureInfo.size() == 1)
		m_TextureType = ETexture_Type::Atlas;

	else
		m_TextureType = ETexture_Type::Frame;

	return true;
}

void CTexture::Render(HDC hDC, const Vector2& WindowPos,
	const Vector2& ImgPos, const Vector2& Size,
	int Index)
{
	if (m_vecTextureInfo[Index]->ColorKeyEnable)
	{
		// 1번 인자 : 출력 대상 DC
		// 2번 인자 : 출력할 윈도우 상 x 좌표
		// 3번 인자 : 출력할 윈도우 상 y 좌표
		// 4번 인자 : 출력할 윈도우 상에서 width
		// 5번 인자 : 출력할 윈도우 상에서 height
		// 6번 인자 : 출력할 이미지의 DC
		// 7번 인자 : 출력할 이미지 상에서 시작 x 좌표
		// 8번 인자 : 출력할 이미지 상에서 시작 y 좌표
		// 9번 인자 : 출력할 이미지 가로 크기
		// 10번 인자 : 출력할 이미지 세로 크기
		// 11번 인자 : ColorKey
		TransparentBlt(hDC, (int)WindowPos.x, (int)WindowPos.y
			, (int)Size.x, (int)Size.y,
			m_vecTextureInfo[Index]->hDC,
			(int)ImgPos.x, (int)ImgPos.y,
			(int)Size.x, (int)Size.y,
			m_vecTextureInfo[Index]->ColorKey);
	}

	else
	{
		BitBlt(hDC, (int)WindowPos.x, (int)WindowPos.y,
			(int)Size.x, (int)Size.y,
			m_vecTextureInfo[Index]->hDC,
			(int)ImgPos.x, (int)ImgPos.y,
			SRCCOPY);
	}


}
