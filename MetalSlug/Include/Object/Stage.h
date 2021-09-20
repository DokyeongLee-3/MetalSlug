#pragma once
#include "GameObject.h"

class CStage :
    public CGameObject
{
    friend class CScene;

protected:
    CStage();
    CStage(const CStage& obj);
    virtual ~CStage();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	// ȭ��ۿ� �ִ� ������Ʈ���� �ɷ����� �۾�(Culling)�� ���ش�
	virtual void PrevRender();
	virtual void Render(HDC hDC);
	virtual CGameObject* Clone();
};

