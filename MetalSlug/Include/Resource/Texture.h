#pragma once

#include "../Ref.h"

struct TextureInfo
{
	HDC hDC;
	HBITMAP	hBmp;
	HBITMAP	hPrev;
	BITMAP	BmpInfo;
	bool ColorKeyEnable;
	unsigned int ColorKey;

	TextureInfo() :
		hDC(0),
		hBmp(0),
		hPrev(0),
		BmpInfo{},
		ColorKeyEnable(false),
		ColorKey(0)
	{

	}
	~TextureInfo()
	{
		SelectObject(hDC, hPrev);

		DeleteObject(hBmp);

		DeleteDC(hDC);
	}

};


class CTexture	:
	public CRef
{

public:
	CTexture();
	~CTexture();

private:
	// 애니메이션이 atlas 방식인지, frame 방식인지
	ETexture_Type m_TextureType;
	// 애니메이션을 위해서 텍스쳐를 여러개 담고 있을 수 있게 한다
	std::vector<TextureInfo*>	m_vecTextureInfo;

public:
	ETexture_Type GetTextureType()	const
	{
		return m_TextureType;
	}

	unsigned int GetWidth(int idx = 0)
	{
		return (unsigned int)m_vecTextureInfo[idx]->BmpInfo.bmWidth;
	}

	unsigned int GetHeight(int idx = 0)
	{
		return (unsigned int)m_vecTextureInfo[idx]->BmpInfo.bmHeight;
	}

	void SetColorKey(const unsigned char r, const unsigned char g,
		const unsigned char b, int Index = 0)
	{
		m_vecTextureInfo[Index]->ColorKeyEnable = true;
		m_vecTextureInfo[Index]->ColorKey = RGB(r, g, b);
	}
public:
	bool LoadTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name,
		const TCHAR* FullPath);
	bool LoadTexture(const std::string& Name,
		const std::vector<std::wstring>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);

public:
	void Render(HDC hDC, const Vector2& WindowPos,
		const Vector2& ImgPos, const Vector2& Size,
		int Index = 0);

};

