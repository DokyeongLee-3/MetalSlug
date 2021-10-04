
#include "Scene.h"
#include "SceneResource.h"
#include "SceneCollision.h"
#include "Camera.h"

CScene::CScene()
{
	m_Resource = new CSceneResource;
	m_Collision = new CSceneCollision;

	m_Collision->m_Scene = this;

	m_RenderCount = 0;
	m_RenderCapacity = 100;
	m_RenderArray = new CGameObject * [m_RenderCapacity];

	m_UICount = 0;
	m_UICapacity = 10;
	m_UIArray = new CUIWindow * [m_UICapacity];

	m_Camera = new CCamera;

	m_Camera->Init();
}

CScene::~CScene()
{
	SAFE_DELETE(m_Camera);
	SAFE_DELETE_ARRAY(m_RenderArray);

	for (int i = 0; i < m_UICount; ++i)
	{
		SAFE_RELEASE(m_UIArray[i]);
	}
	SAFE_DELETE_ARRAY(m_UIArray);

	m_ObjList.clear();

	m_mapPrototype.clear();
	

	// MainScene�� ����ǰ� ������ CSharedPtr<CPlayer>Ÿ����
	// m_Player�� delete�Ǿ�� �ϴµ�, m_ObjList���� 
	// ���������ֱ� ������ �ڵ����� delete���� �ʴ´�.
	// m_Player�� ���� ������ �����ε��� ���� ȣ���ϸ�
	// CSharedPtr���� ������ m_Ptr(CPlayer*Ÿ��)�� 
	// ���� �ִ��� release�ϴµ� ���⼭ m_Ptr�� 
	// ref count�� 1�̶� release�ϸ� 0�� �ż�
	// delete�ǰ� m_Ptr���� nullptr�� �Ҵ�ȴ�
	m_Player = nullptr;

	SAFE_DELETE(m_Collision);
	SAFE_DELETE(m_Resource);
}

CSceneResource* CScene::GetSceneResource() const
{
	return m_Resource;
}

CSceneCollision* CScene::GetSceneCollision() const
{
	return m_Collision;
}

CCamera* CScene::GetCamera() const
{
	return m_Camera;
}

CGameObject* CScene::FindObject(const std::string& Name)
{
	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == Name)
			return (*iter);
	}

	return nullptr;
}

void CScene::SetPlayer(const std::string& Name)
{
	CGameObject* Player = FindObject(Name);

	SetPlayer(Player);
}

void CScene::SetPlayer(CGameObject* Player)
{
	m_Player = Player;

	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (*iter == Player)
		{
			m_ObjList.erase(iter);
			break;
		}
	}
}

bool CScene::Init()
{
	// CreateObject���� �����Ǵ� object���� init�� ȣ���ϹǷ�
	// ���� ���⼭ m_ObjList�ȿ� object���� init�� ȣ�� �� �ʿ䰡 ����
	return true;
}

bool CScene::Update(float DeltaTime)
{
	// Player�� ��ġ�� Ȯ���ϰ� ���� �Ÿ� ������ �ؾ��ϴ�
	// ���Ͱ� �����Ƿ� Player�� ���� Update������
	if(m_Player)
		m_Player->Update(DeltaTime);

	{
		auto	iter = m_ObjList.begin();
		auto	iterEnd = m_ObjList.end();

		for (; iter != iterEnd;)
		{
			if (!(*iter)->IsActive())
			{
				iter = m_ObjList.erase(iter);
				iterEnd = m_ObjList.end();
				continue;
			}

			(*iter)->Update(DeltaTime * (*iter)->m_TimeScale);
			++iter;
		}
	}

	{
		for (int i = 0; i < m_UICount;)
		{
			if (!m_UIArray[i]->IsActive())
			{
				--m_UICount;

				for (int j = i; j < m_UICount; ++j)
				{
					m_UIArray[j] = m_UIArray[j + 1];
				}

				continue;
			}

			else if (!m_UIArray[i]->GetVisibility())
			{
				++i;
				continue;
			}

			m_UIArray[i]->Update(DeltaTime);
			++i;
		}
	}

	return false;
}

bool CScene::PostUpdate(float DeltaTime)
{
	if (m_Player)
		m_Player->PostUpdate(DeltaTime);

	{
		auto	iter = m_ObjList.begin();
		auto	iterEnd = m_ObjList.end();

		for (; iter != iterEnd;)
		{
			if (!(*iter)->IsActive())
			{

				iter = m_ObjList.erase(iter);
				iterEnd = m_ObjList.end();
				continue;
			}

			(*iter)->PostUpdate(DeltaTime * (*iter)->m_TimeScale);
			++iter;
		}
	}

	{
		for (int i = 0; i < m_UICount;)
		{
			if (!m_UIArray[i]->IsActive())
			{
				--m_UICount;

				for (int j = i; j < m_UICount; ++j)
				{
					m_UIArray[j] = m_UIArray[j + 1];
				}

				continue;
			}

			else if (!m_UIArray[i]->GetVisibility())
			{
				++i;
				continue;
			}

			m_UIArray[i]->PostUpdate(DeltaTime);
			++i;
		}
	}

	// ī�޶��� Update�� Render �ϱ� ��, Player�� 
	// PostUpdate�� �ϰ� �� �Ŀ� ���ش�
	m_Camera->Update(DeltaTime);

	return false;
}

bool CScene::Collision(float DeltaTime)
{
	if (m_Player)
		m_Player->Collision(DeltaTime);

	{
		auto	iter = m_ObjList.begin();
		auto	iterEnd = m_ObjList.end();

		for (; iter != iterEnd;)
		{
			if (!(*iter)->IsActive())
			{
				iter = m_ObjList.erase(iter);
				iterEnd = m_ObjList.end();
				continue;
			}

			(*iter)->Collision(DeltaTime * (*iter)->m_TimeScale);
			++iter;
		}
	}

	/*
	{
		if (m_UICount >= 2)
		{
			// ZOrder�� ���� CUIWindow�� �����Ѵ�.
			qsort(m_UIArray, (size_t)m_UICount, sizeof(CUIWindow*),
				CScene::SortZOrder);
		}

		for (int i = 0; i < m_UICount; )
		{
			if (!m_UIArray[i]->IsActive())
			{
				--m_UICount;

				for (int j = i; j < m_UICount; ++j)
				{
					m_UIArray[j] = m_UIArray[j + 1];
				}

				continue;
			}
			
			else if (!m_UIArray[i]->GetVisibility())
			{
				++i;
				continue;
			}

			// UIWindow�� Widget���� ZOrder�� ���� ����
			m_UIArray[i]->Collision(DeltaTime);
			// CSceneCollision::m_vecUIWindow �迭�� push
			m_Collision->AddUIWindow(m_UIArray[i]);

			++i;
		}
	}
	*/


	m_Collision->Collision(DeltaTime);

	return false;
}

bool CScene::Render(HDC hDC)
{	
	if(m_Player)
		m_Player->PrevRender();

	{
		std::list<CSharedPtr<CGameObject>>::iterator	iter = m_ObjList.begin();
		std::list<CSharedPtr<CGameObject>>::iterator	iterEnd = m_ObjList.end();

		for (; iter != iterEnd; )
		{
			if (!(*iter)->IsActive())
			{
				iter = m_ObjList.erase(iter);
				iterEnd = m_ObjList.end();
				continue;
			}

			(*iter)->PrevRender();

			// ȭ�� �ȿ� �����ϴ� GameObject���
			if (!(*iter)->IsCull())
			{
				if (m_RenderCount == m_RenderCapacity)
				{
					m_RenderCapacity *= 2;

					CGameObject** Array = new CGameObject* [m_RenderCapacity];

					memcpy(Array, m_RenderArray, sizeof(CGameObject*) * m_RenderCount);

					delete[] m_RenderArray;

					m_RenderArray = Array;
				}

				m_RenderArray[m_RenderCount] = *iter;
				++m_RenderCount;
			}
			++iter;
		}
	}

	if (m_RenderCount == m_RenderCapacity)
	{
		m_RenderCapacity *= 2;

		CGameObject** Array = new CGameObject * [m_RenderCapacity];

		memcpy(Array, m_RenderArray, sizeof(CGameObject*) * m_RenderCount);

		delete[] m_RenderArray;

		m_RenderArray = Array;
	}

	if (m_Player)
	{
		if (!m_Player->IsCull())
		{
			m_RenderArray[m_RenderCount] = m_Player;
			++m_RenderCount;
		}
	}

	qsort(m_RenderArray, (size_t)m_RenderCount, sizeof(CGameObject*),
		CScene::SortObjectZOrder);

	for (int i = 0; i < m_RenderCount; ++i)
	{
		m_RenderArray[i]->Render(hDC);
	}

	// rendering �� ������ m_RenderArray�� clear�ϰ�
	// ó������ ������ؼ� ó������ ä�쵵�� �ϱ�
	m_RenderCount = 0;

	// object���� ���� �ڵ�鿡�� ���� ����ϰ�
	// UI���� �� Object���� ���� �� �ֵ���
	// �� ������ ����ϱ� ���ؼ� �Ʒ��� ���� ��
	{
		for (int i = 0; i < m_UICount;)
		{
			if (!m_UIArray[i]->IsActive())
			{
				--m_UICount;

				for (int j = i; j < m_UICount; ++j)
				{
					m_UIArray[j] = m_UIArray[j + 1];
				}

				continue;
			}
			++i;
		}

		for (int i = m_UICount - 1; i >= 0;)
		{
			if (!m_UIArray[i]->GetVisibility())
			{
				--i;
				continue;
			}

			m_UIArray[i]->Render(hDC);
			--i;
		}
	}

	return false;
}

// SortY�� ���� ���ڴ� �����ؾ��� container�� element����
// ������ Ÿ���̴�. m_RenderArray�� element�� Ÿ����
// CGameObject*�̰�, �̰��� ������ Ÿ���� CGameObject** �̴�
int CScene::SortY(const void* Src, const void* Dest)
{
	CGameObject* SrcObj = *(CGameObject**)Src;
	CGameObject* DestObj = *(CGameObject**)Dest;

	// �߹� ��ǥ�� Bottom���� ���Ѵ�
	float SrcY = SrcObj->GetBottom();
	float DestY = DestObj->GetBottom();

	if (SrcY < DestY)
		return -1;

	else if (SrcY > DestY)
		return 1;

	return 0;
}

int CScene::SortZOrder(const void* Src, const void* Dest)
{
	CUIWindow* SrcObj = *(CUIWindow**)Src;
	CUIWindow* DestObj = *(CUIWindow**)Dest;

	int SrcZ = SrcObj->GetZOrder();
	int DestZ = DestObj->GetZOrder();

	if (SrcZ > DestZ)
		return -1;

	else if (SrcZ < DestZ)
		return 1;

	return 0;
}

int CScene::SortObjectZOrder(const void* Src, const void* Dest)
{
	CGameObject* SrcObj = *(CGameObject**)Src;
	CGameObject* DestObj = *(CGameObject**)Dest;

	int SrcZ = SrcObj->GetZOrder();
	int DestZ = DestObj->GetZOrder();

	if (SrcZ < DestZ)
		return -1;

	else if (SrcZ > DestZ)
		return 1;

	return 0;
}

CGameObject* CScene::FindPrototype(const std::string& Name)
{
	auto iter = m_mapPrototype.find(Name);

	if (iter == m_mapPrototype.end())
		return nullptr;

	return iter->second;
}
