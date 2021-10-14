#pragma once

#include "../Ref.h"
#include "../Resource/Texture.h"
#include "../Animation/Animation.h"
#include "../Collision/Collider.h"

class CGameObject	:
	public CRef
{
	friend class CScene;

public:
	CGameObject();
	CGameObject(const CGameObject& obj);
	virtual ~CGameObject();

protected:
	// 본인이 어떤 scene에 속해있는지 가리키는 멤버
	class CScene* m_Scene;
	Vector2		m_PrevPos;
	Vector2		m_Pos;
	// 화면상의 위치(m_Pos는 World상의 위치)
	Vector2		m_RenderPos;
	Vector2		m_Size;
	Vector2		m_Pivot;
	// 한 프레임당 이동한 크기와 방향을 갖고 있는 벡터
	Vector2		m_Velocity;
	// 애니메이션은 종류마다 약간씩 발밑에 간격이 남아있는
	// 경우가 있기 때문에 이를 조정해주는 offset
	Vector2		m_Offset;
	// 이동 속도
	float		m_MoveSpeed;
	float		m_TimeScale;

	CSharedPtr<CTexture>	m_Texture;
	Vector2		m_ImageStart;
	std::list<CSharedPtr<CCollider>> m_ColliderList;
	// 카메라 영역 밖에 있다면 true
	bool		m_CameraCull;
	bool		m_Start;
	bool		m_BeforeRender;
	EObject_Type	m_ObjType;

	float		m_LifeTime;
	bool		m_PhysicsSimulate;
	float		m_GravityAccel;
	bool		m_IsGround;
	float		m_FallTime;
	float		m_FallStartY;
	bool		m_Jump;
	float		m_JumpVelocity;
	int m_ZOrder;

	bool m_DamageEnable;

public:
	void SetZOrder(int ZOrder)
	{
		m_ZOrder = ZOrder;
	}

	void SetLifeTime(float Time)
	{
		m_LifeTime = Time;
	}

	void SetVelocity(const Vector2& Velocity)
	{
		m_Velocity = Velocity;
	}

	void SetGravityAccel(float Accel)
	{
		m_GravityAccel = Accel;
	}

	void SetPhysicsSimulate(bool Physics)
	{
		m_PhysicsSimulate = Physics;
	}

	void SetJumpVelocity(float JumpVelocity)
	{
		m_JumpVelocity = JumpVelocity;
	}

	void Jump()
	{
		if (!m_Jump)
		{
			m_Jump = true;
			m_IsGround = false;

			m_FallStartY = m_Pos.y;
		}
	}

	void DamageEnable(bool Enable)
	{
		m_DamageEnable = Enable;
	}

	EObject_Type GetObjType()	const
	{
		return m_ObjType;
	}


	bool IsCull()	const
	{
		return m_CameraCull;
	}

public:
	CCollider* FindCollider(const std::string& Name);

protected:
	CAnimation* m_Animation;

public:
	// m_Animation이 nullptr이면 동적할당해주고
	// m_Animation의 m_Owner를 나 자신으로,
	// m_Animation의 m_Scene을 내가 속해있는 Scene로
	// 할당해주는 함수
	void CreateAnimation();
	// Animation클래스에서 AnimationInfo를 동적할당하고
	// 이미 CSceneResource에 load해둔 Sequence를 
	// AnimationInfo의 Sequence멤버에 할당해줘서
	// 완성된 AnimationInfo를 CAnimation의 멤버인
	// m_mapAnimation에 insert해주는 함수
	void AddAnimation(const std::string& SequenceName,
		bool Loop = true, float PlayTime = 1.f, float PlayScale = 1.f,
		bool Reverse = false);
	void SetAnimationPlayTime(const std::string& Name,
		float PlayTime);
	void SetAnimationPlayScale(const std::string& Name, 
		float PlayScale);
	void SetCurrentAnimation(const std::string& Name);
	void ChangeAnimation(const std::string& Name);
	bool CheckCurrentAnimation(const std::string& Name);
	void SetAnimationReverse(const std::string& Name,
		bool Reverse);
	void SetAnimationLoop(const std::string& Name,
		bool Loop);

public:
	int GetZOrder()	const
	{
		return m_ZOrder;
	}

	Vector2	GetOffset() const
	{
		return m_Offset;
	}

	Vector2 GetPos()	const
	{
		return m_Pos;
	}

	Vector2 GetPrevPos()	const
	{
		return m_PrevPos;
	}

	Vector2 GetSize()	const
	{
		return m_Size;
	}

	Vector2	GetPivot()	const
	{
		return m_Pivot;
	}

	Vector2	GetVelocity()	const
	{
		return m_Velocity;
	}

	float GetMoveSpeed()	const
	{
		return m_MoveSpeed;
	}

	float GetMoveSpeedFrame()	const
	{
		return m_Velocity.Length();
	}

	float GetBottom()	const;

public:
	void SetScene(class CScene* Scene);

	void SetOffset(const Vector2& Offset)
	{
		m_Offset = Offset;
	}

	void SetOffset(float x, float y)
	{
		m_Offset.x = x;
		m_Offset.y = y;
	}

	void SetPos(float x, float y)
	{
		m_Pos.x = x;
		m_Pos.y = y;
	}

	void SetPos(const Vector2& Pos)
	{
		m_Pos = Pos;
	}

	void SetSize(float x, float y)
	{
		m_Size.x = x;
		m_Size.y = y;
	}

	void SetSize(const Vector2& Size)
	{
		m_Size = Size;
	}

	void SetPivot(float x, float y)
	{
		m_Pivot.x = x;
		m_Pivot.y = y;
	}

	void SetPivot(const Vector2& Pivot)
	{
		m_Pivot = Pivot;
	}

	void SetMoveSpeed(float Speed)
	{
		m_MoveSpeed = Speed;
	}

	void SetTimeScale(float TimeScale)
	{
		m_TimeScale = TimeScale;
	}

	void Move(const Vector2& Dir);
	void Move(const Vector2& Dir, float Speed);

	void SetImageStart(float x, float y)
	{
		m_ImageStart.x = x;
		m_ImageStart.y = y;
	}

	void SetImageStart(const Vector2& ImageStart)
	{
		m_ImageStart = ImageStart;
	}

public:
	void SetTexture(CTexture* Texture)
	{
		m_Texture = Texture;

		if (m_Texture)
		{
			m_Size.x = (float)m_Texture->GetWidth();
			m_Size.y = (float)m_Texture->GetHeight();
		}
	}
	// 이미 SceneResource에 Texture가 load되어 m_mapTexture에 들어 있을때 쓰는 함수
	void SetTexture(const std::string& Name);
	// 이 아래 함수들은 SceneResource에 처음 load할 때 쓰는 함수들
	void SetTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	void SetTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	void SetTexture(const std::string& Name,
		const std::vector<std::wstring>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	void SetTextureColorKey(unsigned char r, unsigned char g,
		unsigned char b, int Index = 0);

public:
	// 만약 Object들이 Clone으로 만들어 진 녀석들이라면
	// m_BeginFunction이나 m_EndFunction을 단순히 원본 객체것을
	// 단순히 복사하면 안된다. m_BeginFunction이나 m_EndFunction을
	// 세팅해줄때 인자로 this를 넣어줬는데 이 this는 원본객체를
	// 의미하는 this이지, 복사되는 객체를 가리키는 this가 아니기
	// 때문이다.
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	// 화면밖에 있는 오브젝트들을 걸러내는 작업(Culling)을 해준다
	virtual void PrevRender();
	virtual void Render(HDC hDC);
	virtual CGameObject* Clone();


public:
	template <typename T>
	void SetAnimationEndNotify(const std::string SequenceName,
		T* Obj, void (T::* Func)())
	{
		m_Animation->SetEndNotify<T>(SequenceName,
			Obj, Func);
	}

	template <typename T>
	void AddAnimationNotify(const std::string& SequenceName,
		int Frame, T* Obj, void (T::* Func)())
	{
		m_Animation->AddNotify<T>(SequenceName, Frame,
			Obj, Func);
	}

	template <typename T>
	T* AddCollider(const std::string& Name)
	{
		T* Collider = new T;

		Collider->SetName(Name);
		Collider->SetOwner(this);
		Collider->SetScene(m_Scene);

		if (!Collider->Init())
		{
			SAFE_DELETE(Collider);
			return nullptr;
		}

		m_ColliderList.push_back(Collider);
		
		return Collider;
	}
};

