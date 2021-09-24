
#include "ColliderPixel.h"
#include "ColliderSphere.h"
#include "../Object/GameObject.h"
#include "Collision.h"
#include "../PathManager.h"
#include "ColliderBox.h"

CColliderPixel::CColliderPixel()
{
	m_Type = ECollider_Type::Pixel;
}

CColliderPixel::CColliderPixel(const CColliderPixel& collider)
{
	m_Width = collider.m_Width;
	m_Height = collider.m_Height;
}

CColliderPixel::~CColliderPixel()
{
}

bool CColliderPixel::SetPixelInfo(const TCHAR* FileName, const std::string& PathKey)
{
	m_FileName = FileName;

	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathKey);

	TCHAR FullPath[MAX_PATH] = {};

	if (Info)
		lstrcpy(FullPath, Info->Path);
	lstrcat(FullPath, FileName);

	FILE* pFile = NULL;

	char strPath[MAX_PATH] = {};
	WideCharToMultiByte(CP_ACP, 0, FullPath, lstrlen(FullPath),
		strPath, lstrlen(FullPath), NULL, NULL);

	// rb는 read binary라는 의미의 mode
	fopen_s(&pFile, strPath, "rb");

	if (!pFile)
		return false;

	BITMAPFILEHEADER fh;
	// 미리 정의된 BITMAPINFOHEADER 구조체에 
	// bit 가로, 세로 수가 멤버로 있고
	// fread로 읽어오면 멤버에 비트수가 할당되어 있음
	BITMAPINFOHEADER ih;

	fread(&fh, sizeof(fh), 1, pFile);
	fread(&ih, sizeof(ih), 1, pFile);

	m_Width = ih.biWidth;
	m_Height = ih.biHeight;

	m_vecPixel.clear();

	m_vecPixel.resize(m_Width * m_Height);

	// 여기서 읽어온 비트맵 정보를 보면
	// 비트맵의 맨 아래에서부터 좌에서 우 방향으로
	// 위로 올라오면서 읽힌 것을 알 수 있다.
	// 즉 비트맵 파일은 바닥의 비트 정보부터 저장한다.
	fread(&m_vecPixel[0], sizeof(PIXEL), m_vecPixel.size(), pFile);

	PPIXEL pPixelArr = new PIXEL[m_Width];

	// 위 아래를 반전시켜준다.
	for (int i = 0; i < m_Height / 2; ++i)
	{
		// 현재 인덱스의 픽셀 한 줄씩 따로 저장해둔다.
		memcpy(pPixelArr, &m_vecPixel[i * m_Width],
			sizeof(PIXEL) * m_Width);
		// 가운데를 기준으로 데칼코마니처럼 한줄씩 바꿔준다.
		memcpy(&m_vecPixel[i * m_Width],
			&m_vecPixel[(m_Height - i - 1) * m_Width],
			sizeof(PIXEL) * m_Width);
		// 따로 저장해준 pPixelArr을 가운데를 기준으로 반대부분에 저장
		memcpy(&m_vecPixel[(m_Height - i - 1) * m_Width], pPixelArr,
			sizeof(PIXEL) * m_Width);
	}

	// bmp는 저장할 때 RGB가 아닌 BGR순으로 저장되어 있는걸 잊지 말자
	// 그래서 여기서 bgr순으로 읽어온 m_vecPixel에 픽셀 정보들을
	// 다시 rgb순으로 바꿔준다
	for (int i = 0; i < m_vecPixel.size(); ++i)
	{
		unsigned char tmp = m_vecPixel[i].r;
		m_vecPixel[i].r = m_vecPixel[i].b;
		m_vecPixel[i].b = tmp;
	}

	delete[] pPixelArr;

	fclose(pFile);

	return true;
}

bool CColliderPixel::Init()
{
	if (!CCollider::Init())
		return false;

	return true;
}

void CColliderPixel::Update(float DeltaTime)
{
	CCollider::Update(DeltaTime);
}

void CColliderPixel::PostUpdate(float DeltaTime)
{
	CCollider::PostUpdate(DeltaTime);

	Vector2	Pos = m_Owner->GetPos();
}

void CColliderPixel::Render(HDC hDC)
{
	CCollider::Render(hDC);
}

CColliderPixel* CColliderPixel::Clone()
{
	return new CColliderPixel(*this);
}

bool CColliderPixel::Collision(CCollider* Dest)
{
	switch (Dest->GetColliderType())
	{
	case ECollider_Type::Box:
		return CCollision::CollisionBoxToPixel((CColliderBox*)Dest, this);
	case ECollider_Type::Sphere:
		return CCollision::CollisionSphereToPixel((CColliderSphere*)Dest, this);
	case ECollider_Type::Point:
		break;
	default:
		break;
	}

	return false;
}
