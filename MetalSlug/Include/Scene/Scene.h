#pragma once

#include "../GameInfo.h"
#include "../Object/GameObject.h"
#include "../UI/UIWindow.h"

class CScene
{
	friend class CSceneManager;

protected:
	CScene();
	virtual ~CScene();

private:
	class CSceneResource* m_Resource;
	class CSceneCollision* m_Collision;
	class CCamera* m_Camera;

public:
	CSceneResource* GetSceneResource()	const;
	CSceneCollision* GetSceneCollision()	const;
	class CCamera* GetCamera()	const;

private:
	// ObjList에 넣어서 Update, PostUpdate등을 해주는게 아니라
	// 따로 빼서 제일 먼저 Update, PostUpdate등을 처리해준다
	// 왜냐하면 Player의 갱신된 정보를 바탕으로 처리해야 할 것들이 있기 때문
	CSharedPtr<CGameObject>	m_Player;
	std::list<CSharedPtr<CGameObject>>	m_ObjList;
	// list보다 삽입, 삭제를 빠르게 할 수 있는
	// 동적 배열 사용. 화면 안에 있는 object들만 여기 담아서
	// Rendering 할 것임
	CGameObject** m_RenderArray;
	int m_RenderCount;
	int m_RenderCapacity;
	// 검색이 빨라야 하므로 unordered_map 사용
	std::unordered_map<std::string, CSharedPtr<CGameObject>> m_mapPrototype;
	CUIWindow** m_UIArray;
	int m_UICount;
	int m_UICapacity;

public:
	CGameObject* FindObject(const std::string& Name);
	void SetPlayer(const std::string& Name);
	// m_ObjList에서 Player를 찾아서 m_Player에 할당하고
	// m_ObjList에서 제거하는 함수
	void SetPlayer(CGameObject* Player);
	CGameObject* GetPlayer()	const
	{
		return m_Player;
	}

public:
	virtual bool Init();
	virtual bool Update(float DeltaTime);
	virtual bool PostUpdate(float DeltaTime);
	virtual bool Collision(float DeltaTime);
	virtual bool Render(HDC hDC);

public:
	// qsort를 사용할 것인데, qsort의 4번째 인자로 들어가는
	// compare 함수는 const void* 인자 2개를 받는 함수를
	// 넣어줘야 하므로 SortY와 SortZOrder를 다음과 같은
	// 인자타입들로 선언해줬다 그리고 qsort의 4번째 인자로
	// 들어가는 compare함수는 반드시 int로 return type을 가져야한다
	// 오름차순 정렬
	static int SortY(const void* Src, const void* Dest);
	// 내림차순 정렬
	static int SortZOrder(const void* Src, const void* Dest);

private:
	CGameObject* FindPrototype(const std::string& Name);

public:
	template <typename T>
	T* CreateObject(const std::string& Name,
		const Vector2& Pos = Vector2(0.f, 0.f),
		const Vector2& Size = Vector2(100.f, 100.f))
	{
		T* Obj = new T;

		Obj->SetScene(this);
		Obj->SetPos(Pos);
		Obj->SetSize(Size);
		Obj->SetName(Name);

		if (!Obj->Init())
		{
			SAFE_DELETE(Obj);
			return nullptr;
		}

		// 생성된 모든 객체는 Init전에 Start를 호출하고
		// m_ObjList(혹은 m_mapPrototype)으로 삽입됨
		// 여기서 Start를 해주면 안된다. 이유는 아래
		// CreateObject함수 위 주석에 작성해놓음
		// Obj->Start();

		// Obj가 push_back인자로 넘어갈 때 복사 T* Ptr을 인자로 받는 
		// 복사 생성자 한번 호출(ref count = 1),
		// vector 외부에 CSharedPtr이 내부에 있는 CSharedPtr로 복사 될 때
		// CSharedPtr<T> Ptr을 인자로 받는 복사 생성자 한 번 호출(ref count = 2)
		// 외부에 CSharedPtr 소멸되면서 소멸자 호출(Ref count = 1)
		m_ObjList.push_back(Obj);

		return Obj;
	}

	// MainScene::Init에서 prototype미리 만들어서
	// m_mapPrototype에 넣어두고(CreatePrototype에서 각
	// prototype의 Init을 하기 때문에 Collider도 가진채로
	// m_mapPrototype에 들어감) 나중에 총알이나 EffectHit를
	// 만들어야 할 때는 CreateObject로 m_mapPrototype에서
	// 복사해서 m_ObjList에 넣어서 실제로 Scene에 배치
	// Collider들의 m_EndFunction, m_BeginFunction 그리고
	// 애니메이션의 Notify는 복사되면 안되고 객체가 새로
	// 생성될 때마다 따로 설정해줘야 하기 때문에(인자에 this가
	// 들어가기 때문에) 각 GameObject들의 Start함수에서 
	// 그런것들을 오버로딩된 CreateObject 함수들에서
	// 해주고 있다.
	template <typename T>
	T* CreatePrototype(const std::string& Name)
	{
		T* Obj = new T;

		Obj->SetScene(this);
		Obj->SetName(Name);

		if (!Obj->Init())
		{
			SAFE_DELETE(Obj);
			return nullptr;
		}

		m_mapPrototype.insert(std::make_pair(Name, Obj));

		return Obj;
	}

	template <typename T>
	T* CreateObject(const std::string& Name, const std::string& PrototypeName,
		const Vector2& Pos = Vector2(0.f, 0.f),
		const Vector2& Size = Vector2(100.f, 100.f))
	{
		CGameObject* Prototype = FindPrototype(PrototypeName);

		if (!Prototype)
			return nullptr;

		T* Obj = (T*)Prototype->Clone();

		Obj->SetScene(this);
		Obj->SetPos(Pos);
		Obj->SetSize(Size);
		Obj->SetName(Name);
		// 여기서 Start를 해주면 안된다. 왜냐하면 CScene::Update에서
		// m_UIArray의 Window안에 있는 Button의 Update 호출 하는중
		// Start Button Click이 감지돼서 m_ButtonClickCallback호출해서
		// MainScene::Init호출하면 MainScene에서 필요한 
		// 각종 Object들(ex.Player)이 만들어져서 CScene의 m_UIArray에 추가될텐데 
		// 지금 이 상태는 m_NextScene에 MainScene가 만들어지긴 했지만 
		// ChangeScene()::Update안에서 m_Scene->Update(DeltaTime)
		// 코드를 동작중이고 ChangeScene()::Update안에 
		// ChangeScene()은 호출되지 않았으므로 지금 현재 Scene은
		// StartScene이다. 따라서 여기서 Start를 해버리면 현재는 StartScene
		// 임에도 불구하고 CPlayer::Start에서 Player의 이동하는
		// 키에 대한 callback함수가 등록돼버린다.
		// Obj->Start();

		m_ObjList.push_back(Obj);

		return Obj;
	}

	template <typename T>
	T* CreateUIWindow(const std::string& Name)
	{
		T* Window = new T;

		Window->SetName(Name);
		Window->SetScene(this);

		if (!Window->Init())
		{
			SAFE_DELETE(Window);
			return nullptr;
		}

		if (m_UICount == m_UICapacity)
		{
			m_UICapacity *= 2;

			CUIWindow** Array = new CUIWindow * [m_UICapacity];

			memcpy(Array, m_UIArray, sizeof(CUIWindow*) * m_UICount);

			SAFE_DELETE_ARRAY(m_UIArray);

			m_UIArray = Array;
		}

		m_UIArray[m_UICount] = Window;
		++m_UICount;

		return Window;
	}
};

