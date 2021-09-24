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
	// CGameObject::Collision에서 각 object들이 m_ColliderList에
	// 가지고 있는 Collider들을 SceneCollision의 m_vecCollider에
	// 넣고, Scene::Collision에서 SceneCollision::Collision 호출 
	void Collision(float DeltaTime);
	void AddUIWindow(class CUIWindow* Window);

private:
	static int SortY(const void* Src, const void* Dest);
};
