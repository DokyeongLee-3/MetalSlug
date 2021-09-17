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
	// ObjList�� �־ Update, PostUpdate���� ���ִ°� �ƴ϶�
	// ���� ���� ���� ���� Update, PostUpdate���� ó�����ش�
	// �ֳ��ϸ� Player�� ���ŵ� ������ �������� ó���ؾ� �� �͵��� �ֱ� ����
	CSharedPtr<CGameObject>	m_Player;
	std::list<CSharedPtr<CGameObject>>	m_ObjList;
	// list���� ����, ������ ������ �� �� �ִ�
	// ���� �迭 ���. ȭ�� �ȿ� �ִ� object�鸸 ���� ��Ƽ�
	// Rendering �� ����
	CGameObject** m_RenderArray;
	int m_RenderCount;
	int m_RenderCapacity;
	// �˻��� ����� �ϹǷ� unordered_map ���
	std::unordered_map<std::string, CSharedPtr<CGameObject>> m_mapPrototype;
	CUIWindow** m_UIArray;
	int m_UICount;
	int m_UICapacity;

public:
	CGameObject* FindObject(const std::string& Name);
	void SetPlayer(const std::string& Name);
	// m_ObjList���� Player�� ã�Ƽ� m_Player�� �Ҵ��ϰ�
	// m_ObjList���� �����ϴ� �Լ�
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
	// qsort�� ����� ���ε�, qsort�� 4��° ���ڷ� ����
	// compare �Լ��� const void* ���� 2���� �޴� �Լ���
	// �־���� �ϹǷ� SortY�� SortZOrder�� ������ ����
	// ����Ÿ�Ե�� ��������� �׸��� qsort�� 4��° ���ڷ�
	// ���� compare�Լ��� �ݵ�� int�� return type�� �������Ѵ�
	// �������� ����
	static int SortY(const void* Src, const void* Dest);
	// �������� ����
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

		// ������ ��� ��ü�� Init���� Start�� ȣ���ϰ�
		// m_ObjList(Ȥ�� m_mapPrototype)���� ���Ե�
		// ���⼭ Start�� ���ָ� �ȵȴ�. ������ �Ʒ�
		// CreateObject�Լ� �� �ּ��� �ۼ��س���
		// Obj->Start();

		// Obj�� push_back���ڷ� �Ѿ �� ���� T* Ptr�� ���ڷ� �޴� 
		// ���� ������ �ѹ� ȣ��(ref count = 1),
		// vector �ܺο� CSharedPtr�� ���ο� �ִ� CSharedPtr�� ���� �� ��
		// CSharedPtr<T> Ptr�� ���ڷ� �޴� ���� ������ �� �� ȣ��(ref count = 2)
		// �ܺο� CSharedPtr �Ҹ�Ǹ鼭 �Ҹ��� ȣ��(Ref count = 1)
		m_ObjList.push_back(Obj);

		return Obj;
	}

	// MainScene::Init���� prototype�̸� ����
	// m_mapPrototype�� �־�ΰ�(CreatePrototype���� ��
	// prototype�� Init�� �ϱ� ������ Collider�� ����ä��
	// m_mapPrototype�� ��) ���߿� �Ѿ��̳� EffectHit��
	// ������ �� ���� CreateObject�� m_mapPrototype����
	// �����ؼ� m_ObjList�� �־ ������ Scene�� ��ġ
	// Collider���� m_EndFunction, m_BeginFunction �׸���
	// �ִϸ��̼��� Notify�� ����Ǹ� �ȵǰ� ��ü�� ����
	// ������ ������ ���� ��������� �ϱ� ������(���ڿ� this��
	// ���� ������) �� GameObject���� Start�Լ����� 
	// �׷��͵��� �����ε��� CreateObject �Լ��鿡��
	// ���ְ� �ִ�.
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
		// ���⼭ Start�� ���ָ� �ȵȴ�. �ֳ��ϸ� CScene::Update����
		// m_UIArray�� Window�ȿ� �ִ� Button�� Update ȣ�� �ϴ���
		// Start Button Click�� �����ż� m_ButtonClickCallbackȣ���ؼ�
		// MainScene::Initȣ���ϸ� MainScene���� �ʿ��� 
		// ���� Object��(ex.Player)�� ��������� CScene�� m_UIArray�� �߰����ٵ� 
		// ���� �� ���´� m_NextScene�� MainScene�� ��������� ������ 
		// ChangeScene()::Update�ȿ��� m_Scene->Update(DeltaTime)
		// �ڵ带 �������̰� ChangeScene()::Update�ȿ� 
		// ChangeScene()�� ȣ����� �ʾ����Ƿ� ���� ���� Scene��
		// StartScene�̴�. ���� ���⼭ Start�� �ع����� ����� StartScene
		// �ӿ��� �ұ��ϰ� CPlayer::Start���� Player�� �̵��ϴ�
		// Ű�� ���� callback�Լ��� ��ϵŹ�����.
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

