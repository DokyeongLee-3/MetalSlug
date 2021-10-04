
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
	

	// MainScene이 종료되고 나서는 CSharedPtr<CPlayer>타입인
	// m_Player는 delete되어야 하는데, m_ObjList에서 
	// 빠져나와있기 때문에 자동으로 delete되지 않는다.
	// m_Player에 대입 연산자 오버로딩된 것을 호출하면
	// CSharedPtr에서 기존에 m_Ptr(CPlayer*타입)에 
	// 갖고 있던걸 release하는데 여기서 m_Ptr은 
	// ref count가 1이라서 release하면 0이 돼서
	// delete되고 m_Ptr에는 nullptr이 할당된다
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
	// CreateObject에서 생성되는 object들의 init을 호출하므로
	// 굳이 여기서 m_ObjList안에 object들의 init을 호출 할 필요가 없다
	return true;
}

bool CScene::Update(float DeltaTime)
{
	// Player의 위치를 확인하고 나서 거리 조절을 해야하는
	// 몬스터가 있으므로 Player를 먼저 Update해주자
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

	// 카메라의 Update는 Render 하기 전, Player의 
	// PostUpdate를 하고 난 후에 해준다
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
			// ZOrder에 따라 CUIWindow를 정렬한다.
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

			// UIWindow내 Widget들을 ZOrder에 따라 정렬
			m_UIArray[i]->Collision(DeltaTime);
			// CSceneCollision::m_vecUIWindow 배열에 push
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

			// 화면 안에 존재하는 GameObject라면
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

	// rendering 다 했으면 m_RenderArray를 clear하고
	// 처음부터 덮어쓰기해서 처음부터 채우도록 하기
	m_RenderCount = 0;

	// object들을 위의 코드들에서 먼저 출력하고
	// UI들은 그 Object들을 덮을 수 있도록
	// 그 다음에 출력하기 위해서 아래에 쓰는 것
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

// SortY가 받은 인자는 정렬해야할 container에 element들의
// 포인터 타입이다. m_RenderArray의 element의 타입은
// CGameObject*이고, 이것의 포인터 타입은 CGameObject** 이다
int CScene::SortY(const void* Src, const void* Dest)
{
	CGameObject* SrcObj = *(CGameObject**)Src;
	CGameObject* DestObj = *(CGameObject**)Dest;

	// 발밑 좌표인 Bottom값을 구한다
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
