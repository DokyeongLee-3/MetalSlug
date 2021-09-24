#pragma once
#include "GameObject.h"
class CObstacle :
    public CGameObject
{
	friend class CScene;

protected:
	CObstacle();
	CObstacle(const CObstacle& obj);
	virtual ~CObstacle();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	// ȭ��ۿ� �ִ� ������Ʈ���� �ɷ����� �۾�(Culling)�� ���ش�
	virtual void PrevRender();
	virtual void Render(HDC hDC);
	virtual CObstacle* Clone();
};

