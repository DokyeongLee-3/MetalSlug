
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

				// ���� �� Src�� Player�̰� Dest�� Monster�� �Ѿ��϶�
				// ���� �浹 ���θ� �Ǵ��ϴ� ��Ȳ�̶�� �����غ���
				// SrcProfile�� Channel�� Channel_Player,
				// DestProfile�� Channel�� Channel_MonsterAttack�� ���̴�
				// ������ ���� �浹�� �ϰ� �������ٸ� SrcProfile�� 
				// vecState�� 4�� index(Channel_MonsterAttack�� �ǹ�)��, 
				// DestProfile�� vecState�� 1�� index(Channel_Player�� �ǹ�)����
				// ���� Collision���� �Ǿ� ���� ���̴�.
				ECollision_State SrcState = SrcProfile->vecState[DestProfile->Channel];
				ECollision_State DestState = DestProfile->vecState[SrcProfile->Channel];

				if (SrcState == ECollision_State::Ignore ||
					DestState == ECollision_State::Ignore)
					continue;

				if (Src->Collision(Dest))
				{
					// ������ �浹�ǰ� �־��°� �ƴ����� �Ǵ��Ѵ�
					// �� ó�� �浹�ϴ� ���
					if (!Src->CheckCollisionList(Dest))
					{
						// ���� �ڽ��� m_CollisionList�� ���� �߰� 
						Src->AddCollisionList(Dest);
						Dest->AddCollisionList(Src);

						Src->CallCollisionBegin(Dest, DeltaTime);
						Dest->CallCollisionBegin(Src, DeltaTime);
					}

					// ���� �����ӿ����� �浹�߰�,
					// �̹� �����ӿ����� ��� �浹���̰�
					else if (Src->CheckCollisionList(Dest))
					{
						Src->CallCollisionStay(Dest, DeltaTime);
						Dest->CallCollisionStay(Src, DeltaTime);
					}
				}

				// ���� �����ӱ��� �浹�ϴ� ���̾����� �Ǵ�
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

	// �Ź� ���� CGameObject::Collision���� SceneCollision��
	// m_vecCollider�� push�ϰ� ���⼭ clear�Ѵ�
	m_vecCollider.clear();
	m_vecUIWindow.clear();
}
