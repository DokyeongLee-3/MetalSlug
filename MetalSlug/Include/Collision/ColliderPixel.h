
#pragma once
#include "Collider.h"

class CColliderPixel :
	public CCollider
{
	friend class CGameObject;

protected:
	CColliderPixel();
	CColliderPixel(const CColliderPixel& collider);
	virtual ~CColliderPixel();

protected:
	int	m_Width;
	int	m_Height;

	std::vector<PIXEL> m_vecPixel;
	std::wstring	m_FileName;

public:
	void SetExtent(int Width, int Height)
	{
		m_Width = Width;
		m_Height = Height;
	}
	
	int GetWidth()	const
	{
		return m_Width;
	}

	int GetHeight()	const
	{
		return m_Height;
	}

	const PIXEL& GetPixel(int Index)	const
	{
		return m_vecPixel[Index];
	}

public:
	// 들어오는 비트맵정보를 저장
	bool SetPixelInfo(const TCHAR* FileName,
		const std::string& PathKey = TEXTURE_PATH);

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CColliderPixel* Clone();
	virtual bool Collision(CCollider* Dest);
};

