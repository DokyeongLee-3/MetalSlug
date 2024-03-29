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
	// 화면밖에 있는 오브젝트들을 걸러내는 작업(Culling)을 해준다
	virtual void PrevRender();
	virtual void Render(HDC hDC);
	virtual CBackground* Clone();
};

