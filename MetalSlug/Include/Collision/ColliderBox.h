#pragma once
#include "Collider.h"
class CColliderBox :
    public CCollider
{
	friend class CGameObject;

protected:
	CColliderBox();
	CColliderBox(const CColliderBox& collider);
	virtual ~CColliderBox();

protected:
	float	m_Width;
	float	m_Height;
	// World��ǥ ����
	RectInfo	m_Info;
	bool m_CamCollision;
	bool m_FloorCollision;

public:
	void SetExtent(float Width, float Height)
	{
		m_Width = Width;
		m_Height = Height;
	}

	RectInfo GetInfo()	const
	{
		return m_Info;
	}

	virtual float GetBottom()	const
	{
		return m_Info.Bottom;
	}

	void SetCamCollision(bool Collision)
	{
		m_CamCollision = Collision;
	}

	bool GetCamCollision()	const
	{
		return m_CamCollision;
	}

	void SetFloorCollision(bool Collision)
	{
		m_FloorCollision = Collision;
	}

	bool GetFloorCollision()	const
	{
		return m_FloorCollision;
	}

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CColliderBox* Clone();
	virtual bool Collision(CCollider* Dest);
};

