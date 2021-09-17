
#include "SceneCollision.h"
#include "../Collision/Collider.h"
#include "../UI/UIWidget.h"
#include "../Input.h"
#include "Scene.h"
#include "Camera.h"

CSceneCollision::CSceneCollision()
{
	m_vecCollider.reserve(500);
	m_vecUIWindow.reserve(10);
	m_SelectWidget = nullptr;
	m_MouseHoveredWidget = nullptr;
	m_MouseCollision = nullptr;
}

CSceneCollision::~CSceneCollision()
{
}

void CSceneCollision::AddCollider(CCollider* Collider)
{
	m_vecCollider.push_back(Collider);
}

void CSceneCollision::AddUIWindow(CUIWindow* Window)
{
	m_vecUIWindow.push_back(Window);
}

void CSceneCollision::CollisionMouse(float DeltaTime)
{
	Vector2 MousePos = CInput::GetInst()->GetMousePos();
	Vector2 MouseWorldPos = MousePos + m_Scene->GetCamera()->GetPos();

	int WidgetCount = 0;

	// 존재하는 모든 윈도우에서 갖고 있는 위젯의 수를 합친다
	size_t WindowCount = m_vecUIWindow.size();
	for (size_t i = 0; i < WindowCount; ++i)
	{
		WidgetCount += m_vecUIWindow[i]->GetWidgetCount();
	}

	std::vector<CUIWidget*> vecWidget;
	vecWidget.resize(WidgetCount);

	int WidgetOffset = 0;

	if (m_SelectWidget)
	{
		vecWidget[WidgetOffset] = m_SelectWidget;
		++WidgetOffset;
	}

	for (size_t i = 0; i < WindowCount; ++i)
	{
		int Count = m_vecUIWindow[i]->GetWidgetCount();

		for (int j = 0; j < Count; ++j)
		{
			CUIWidget* Widget = m_vecUIWindow[i]->GetWidget(j);

			if (Widget != m_SelectWidget)
			{
				vecWidget[WidgetOffset] = Widget;
				++WidgetOffset;
			}
		}
	}

	// 마우스와 UI간의 충돌 로직
	// 하나라도 충돌이 됐다면 그 뒤에 Widget은 
	// 충돌 여부를 확인할 필요가 없다.
	bool EnableCollision = false;

	for (int i = 0; i < WidgetCount; ++i)
	{
		if (vecWidget[i]->CollisionMouse(MousePos, DeltaTime))
		{
			if (m_MouseHoveredWidget && m_MouseHoveredWidget != vecWidget[i])
			{
				m_MouseHoveredWidget->CollisionMouseReleaseCallback(DeltaTime);
			}

			if (m_MouseCollision)
			{
				m_MouseCollision->SetMouseCollision(false);
				m_MouseCollision = nullptr;
			}

			m_MouseHoveredWidget = vecWidget[i];

			EnableCollision = true;

			break;
		}
	}

	// 만약 위에서 아무 Widget과 충돌하지 않았다면
	// Object들과 충돌을 검사한다
	if (!EnableCollision)
	{	
		// 혹시 위에서 마우스와 충돌한 Widget이 아직 
		// m_MouseHoveredWidget에 남아있다면 Release해준다
		if (m_MouseHoveredWidget)
		{
			m_MouseHoveredWidget->CollisionMouseReleaseCallback(DeltaTime);
			m_MouseHoveredWidget = nullptr;
		}

		if (m_SelectWidget)
			m_SelectWidget = nullptr;
		

		// 마우스와 Object들간의 충돌을 검사한다
		// 화면에 나오는 충돌체들만 검사한다
		size_t Size = m_vecCollider.size();

		if (Size > 1)
		{
			qsort(&m_vecCollider[0], (size_t)Size, sizeof(CCollider*),
				CSceneCollision::SortY);

			bool MouseCollision = false;

			for (size_t i = 0; i < Size; ++i)
			{
				if (m_vecCollider[i]->CollisionMouse(MouseWorldPos))
				{
					// 지금 m_vecCollider[i]와 충돌했는데 그전에 
					// 충돌한 충돌체가 m_MouseCollision에 남아있다면
					// 그 충돌체의 m_MouseCollision을 false로 만든다
					// Collider::m_MouseCollision는 충돌체를 녹색으로 그릴지
					// 빨간색으로 그릴지 판단하는데 사용된다
					if (m_MouseCollision)
						m_MouseCollision->SetMouseCollision(false);

					m_MouseCollision = m_vecCollider[i];
					m_MouseCollision->SetMouseCollision(true);

					MouseCollision = true;
				}
			}
			
			if (!MouseCollision)
			{
				// 이전 프레임들에서 마우스와 오브젝트 충돌체간의
				// 충돌이 있었으나 이번 프레임엔 충돌이 일어나지
				// 않을 경우에는 SceneCollision::m_MouseCollision과
				// CCollider::m_MouseCollision을 충돌하지 않은 상태로
				// 만들어준다
				if (m_MouseCollision)
				{
					m_MouseCollision->SetMouseCollision(false);
					m_MouseCollision = nullptr;
				}
			}
		}
	}
}

int CSceneCollision::SortY(const void* Src, const void* Dest)
{
	CCollider* SrcObj = *(CCollider**)Src;
	CCollider* DestObj = *(CCollider**)Dest;

	float SrcY = SrcObj->GetBottom();
	float DestY = DestObj->GetBottom();

	if (SrcY < DestY)
		return -1;

	else if (SrcY > DestY)
		return 1;

	return 0;
}

void CSceneCollision::Collision(float DeltaTime)
{
	size_t Size = m_vecCollider.size();

	if (Size > 1)
	{
		for (size_t i = 0; i < Size - 1; ++i)
		{
			CCollider* Src = m_vecCollider[i];

			for (size_t j = i + 1; j < Size; ++j)
			{
				CCollider* Dest = m_vecCollider[j];

				if (Src->GetOwner() == Dest->GetOwner())
					continue;

				CollisionProfile* SrcProfile = Src->GetProfile();
				CollisionProfile* DestProfile = Dest->GetProfile();

				// 예를 들어서 Src가 Player이고 Dest가 Monster의 총알일때
				// 둘의 충돌 여부를 판단하는 상황이라고 생각해보자
				// SrcProfile의 Channel은 Channel_Player,
				// DestProfile의 Channel은 Channel_MonsterAttack일 것이다
				// 사전에 서로 충돌을 하게 만들어놨다면 SrcProfile의 
				// vecState에 4번 index(Channel_MonsterAttack를 의미)와, 
				// DestProfile의 vecState에 1번 index(Channel_Player를 의미)에는
				// 각각 Collision으로 되어 있을 것이다.
				ECollision_State SrcState = SrcProfile->vecState[DestProfile->Channel];
				ECollision_State DestState = DestProfile->vecState[SrcProfile->Channel];

				if (SrcState == ECollision_State::Ignore ||
					DestState == ECollision_State::Ignore)
					continue;

				if (Src->Collision(Dest))
				{
					// 이전에 충돌되고 있었는게 아닌지를 판단한다
					// 즉 처음 충돌하는 경우
					if (!Src->CheckCollisionList(Dest))
					{
						// 각자 자신의 m_CollisionList에 상대방 추가 
						Src->AddCollisionList(Dest);
						Dest->AddCollisionList(Src);

						Src->CallCollisionBegin(Dest, DeltaTime);
						Dest->CallCollisionBegin(Src, DeltaTime);
					}
				}

				// 직전 프레임까지 충돌하던 중이었는지 판단
				else if (Src->CheckCollisionList(Dest))
				{
					Src->DeleteCollisionList(Dest);
					Dest->DeleteCollisionList(Src);

					Src->CallCollisionEnd(Dest, DeltaTime);
					Dest->CallCollisionEnd(Src, DeltaTime);
				}
			}
		}
	}

	// 매번 새로 CGameObject::Collision에서 SceneCollision의
	// m_vecCollider에 push하고 여기서 clear한다
	m_vecCollider.clear();
	m_vecUIWindow.clear();
}
