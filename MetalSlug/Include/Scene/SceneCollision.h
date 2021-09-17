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
	// 마우스와 충돌한 Widget
	class CUIWidget* m_MouseHoveredWidget;
	class CUIWidget* m_SelectWidget;
	// Mouse와 충돌한 충돌체. 마우스와 충돌했다면
	// 그 충돌체가 갖고 있는 bool m_MouseCollision을 true로
	// 만들어줘서 충돌체 테두리를 빨간색 Pen으로 그리도록
	// 하기 위해서 Mouse와 충돌한 충돌체를 알 필요가 있다
	class CCollider* m_MouseCollision;
	
public:
	void SetSelectWidget(class CUIWidget* Widget)
	{
		m_SelectWidget = Widget;
	}

	void ClearMouseCollision(class CCollider* Collider)
	{
		if (m_MouseCollision == Collider)
			m_MouseCollision = nullptr;
	}

public:
	void AddCollider(class CCollider* Collider);
	// CGameObject::Collision에서 각 object들이 m_ColliderList에
	// 가지고 있는 Collider들을 SceneCollision의 m_vecCollider에
	// 넣고, Scene::Collision에서 SceneCollision::Collision 호출 
	void Collision(float DeltaTime);
	void AddUIWindow(class CUIWindow* Window);
	void CollisionMouse(float DeltaTime);

private:
	static int SortY(const void* Src, const void* Dest);
};
