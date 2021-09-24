#pragma once

#include "../GameInfo.h"

class CSceneCollision
{
	friend class CScene;

private:
	CSceneCollision();
	~CSceneCollision();

private:
	class CScene* m_Scene;
	std::vector<class CCollider*> m_vecCollider;
	std::vector<class CUIWindow*> m_vecUIWindow;
	

public:
	void AddCollider(class CCollider* Collider);
	// CGameObject::Collision���� �� object���� m_ColliderList��
	// ������ �ִ� Collider���� SceneCollision�� m_vecCollider��
	// �ְ�, Scene::Collision���� SceneCollision::Collision ȣ�� 
	void Collision(float DeltaTime);
	void AddUIWindow(class CUIWindow* Window);

private:
	static int SortY(const void* Src, const void* Dest);
};
