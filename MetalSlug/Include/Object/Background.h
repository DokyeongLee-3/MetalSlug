#pragma once
#include "GameObject.h"

class CBackground :
    public CGameObject
{
	friend class CScene;

protected:
	CBackground();
	CBackground(const CBackground& obj);
	virtual ~CBackground();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	// ȭ��ۿ� �ִ� ������Ʈ���� �ɷ����� �۾�(Culling)�� ���ش�
	virtual void PrevRender();
	virtual void Render(HDC hDC);
	virtual CBackground* Clone();
};

