
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

					// 이전 프레임에서도 충돌했고,
					// 이번 프레임에서도 계속 충돌중이고
					else if (Src->CheckCollisionList(Dest))
					{
						Src->CallCollisionStay(Dest, DeltaTime);
						Dest->CallCollisionStay(Src, DeltaTime);
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
