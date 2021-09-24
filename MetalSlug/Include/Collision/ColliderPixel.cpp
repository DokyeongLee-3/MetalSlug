
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

	// rb�� read binary��� �ǹ��� mode
	fopen_s(&pFile, strPath, "rb");

	if (!pFile)
		return false;

	BITMAPFILEHEADER fh;
	// �̸� ���ǵ� BITMAPINFOHEADER ����ü�� 
	// bit ����, ���� ���� ����� �ְ�
	// fread�� �о���� ����� ��Ʈ���� �Ҵ�Ǿ� ����
	BITMAPINFOHEADER ih;

	fread(&fh, sizeof(fh), 1, pFile);
	fread(&ih, sizeof(ih), 1, pFile);

	m_Width = ih.biWidth;
	m_Height = ih.biHeight;

	m_vecPixel.clear();

	m_vecPixel.resize(m_Width * m_Height);

	// ���⼭ �о�� ��Ʈ�� ������ ����
	// ��Ʈ���� �� �Ʒ��������� �¿��� �� ��������
	// ���� �ö���鼭 ���� ���� �� �� �ִ�.
	// �� ��Ʈ�� ������ �ٴ��� ��Ʈ �������� �����Ѵ�.
	fread(&m_vecPixel[0], sizeof(PIXEL), m_vecPixel.size(), pFile);

	PPIXEL pPixelArr = new PIXEL[m_Width];

	// �� �Ʒ��� ���������ش�.
	for (int i = 0; i < m_Height / 2; ++i)
	{
		// ���� �ε����� �ȼ� �� �پ� ���� �����صд�.
		memcpy(pPixelArr, &m_vecPixel[i * m_Width],
			sizeof(PIXEL) * m_Width);
		// ����� �������� ��Į�ڸ���ó�� ���پ� �ٲ��ش�.
		memcpy(&m_vecPixel[i * m_Width],
			&m_vecPixel[(m_Height - i - 1) * m_Width],
			sizeof(PIXEL) * m_Width);
		// ���� �������� pPixelArr�� ����� �������� �ݴ�κп� ����
		memcpy(&m_vecPixel[(m_Height - i - 1) * m_Width], pPixelArr,
			sizeof(PIXEL) * m_Width);
	}

	// bmp�� ������ �� RGB�� �ƴ� BGR������ ����Ǿ� �ִ°� ���� ����
	// �׷��� ���⼭ bgr������ �о�� m_vecPixel�� �ȼ� ��������
	// �ٽ� rgb������ �ٲ��ش�
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
