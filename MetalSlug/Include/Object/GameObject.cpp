
#include "GameObject.h"
#include "../GameManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/SceneCollision.h"
#include "../Resource/AnimationSequence.h"
#include "../Scene/Camera.h"

CGameObject::CGameObject()	:
	m_Scene(nullptr),
	m_MoveSpeed(200.f),
	m_TimeScale(1.f),
	m_Animation(nullptr),
	m_CameraCull(false)
{
}

CGameObject::CGameObject(const CGameObject& obj)	:
	CRef(obj)
{
	m_Scene = obj.m_Scene;

	if (obj.m_Animation)
		m_Animation = obj.m_Animation->Clone();

	m_Animation->m_Owner = this;

	m_ColliderList.clear();

	auto iter = obj.m_ColliderList.begin();
	auto iterEnd = obj.m_ColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		CCollider* Collider = (*iter)->Clone();

		Collider->SetOwner(this);
		Collider->SetScene(m_Scene);

		m_ColliderList.push_back(Collider);
	}

	m_PrevPos = obj.m_PrevPos;
	m_Pos = obj.m_Pos;
	m_Size = obj.m_Size;
	m_Pivot = obj.m_Pivot;
	m_Velocity = obj.m_Velocity;
	m_Offset = obj.m_Offset;
	m_MoveSpeed = obj.m_MoveSpeed;
	m_TimeScale = obj.m_TimeScale;
	m_Texture = obj.m_Texture;
	m_ImageStart = obj.m_ImageStart;
}

CGameObject::~CGameObject()
{
	SAFE_DELETE(m_Animation);

	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();

	// CCollider::m_CollisionList�� ���� �ִ�
	// ��� �ٸ� �浹ü�鿡�� ���� �浹ü�� �� ���쵵�� �ϴ°�
	// ���� �� �Ѿ��� �÷��̾�� �浹�ؼ� Destroy�� ��Ȳ����
	// �Ѿ� �浹ü�� CBullet�� m_ColliderList��, �浹�� �÷��̾���
	// �浹ü�� m_CollisionList �̷��� �α����� �����ϰ� �����Ƿ�
	// ref count = 2 �̴�. �׷��� Bullet�� Destroy�ϸ� CBullet��
	// m_ColliderList�� ref count�� 1 ���ҽ�Ű���� �÷��̾���
	// Collider�� m_CollisionList�� �ϳ� �����ϰ� �ִ� �͵�
	// ������� �ϱ� ������ ���⼭ �׷� �͵��� �����ش�
	for (; iter != iterEnd; ++iter)
	{
		(*iter)->ClearCollisionList();
	}
}

CCollider* CGameObject::FindCollider(const std::string& Name)
{
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == Name)
			return *iter;
	}
	return nullptr;

}

void CGameObject::CreateAnimation()
{
	if (!m_Animation)
	{
		m_Animation = new CAnimation;

		m_Animation->m_Owner = this;
		m_Animation->m_Scene = m_Scene;
	}
}

void CGameObject::AddAnimation(
	const std::string& SequenceName,
	bool Loop, float PlayTime, float PlayScale,
	bool Reverse)
{
	if (!m_Animation)
	{
		m_Animation = new CAnimation;

		m_Animation->m_Owner = this;
		m_Animation->m_Scene = m_Scene;
	}

	m_Animation->AddAnimation(SequenceName, Loop, PlayTime, PlayScale, Reverse);
}

void CGameObject::SetAnimationPlayTime
(const std::string& Name, float PlayTime)
{
	if (!m_Animation)
		return;

	m_Animation->SetPlayTime(Name, PlayTime);
}

void CGameObject::SetAnimationPlayScale(
	const std::string& Name, float PlayScale)
{
	if (!m_Animation)
		return;

	m_Animation->SetPlayScale(Name, PlayScale);
}

void CGameObject::SetCurrentAnimation(
	const std::string& Name)
{
	if (!m_Animation)
		return;

	m_Animation->SetCurrentAnimation(Name);
}

void CGameObject::ChangeAnimation(
	const std::string& Name)
{
	if (!m_Animation)
		return;

	m_Animation->ChangeAnimation(Name);
}

bool CGameObject::CheckCurrentAnimation(const std::string& Name)
{
	return m_Animation->CheckCurrentAnimation(Name);
}

void CGameObject::SetAnimationReverse(
	const std::string& Name, bool Reverse)
{
	if (!m_Animation)
		return;

	m_Animation->SetReverse(Name, Reverse);
}

void CGameObject::SetAnimationLoop(
	const std::string& Name, bool Loop)
{
	if (!m_Animation)
		return;

	m_Animation->SetLoop(Name, Loop);
}

float CGameObject::GetBottom() const
{
	Vector2 Size = m_Size;

	if (m_Animation)
	{
		AnimationInfo* Info = m_Animation->m_CurrentAnimation;

		const AnimationFrameData& FrameData = Info->Sequence->GetFrameData(Info->Frame);

		Size = FrameData.Size;
	}

	return	m_Pos.y + (1.f - m_Pivot.y) * Size.y + m_Offset.y;
}

void CGameObject::SetScene(CScene* Scene)
{
	m_Scene = Scene;

	if (m_Animation)
		m_Animation->m_Scene = Scene;
}


void CGameObject::Move(const Vector2& Dir)
{
	Vector2 CurrentMove = Dir * m_MoveSpeed * CGameManager::GetInst()->GetDeltaTime() * m_TimeScale;
	m_Velocity += CurrentMove;
	m_Pos += CurrentMove;
}

void CGameObject::Move(const Vector2& Dir, float Speed)
{
	Vector2 CurrentMove = Dir * Speed * CGameManager::GetInst()->GetDeltaTime() * m_TimeScale;
	m_Velocity += CurrentMove;
	m_Pos += CurrentMove;
}

void CGameObject::SetTexture(const std::string& Name)
{
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CGameObject::SetTexture(const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, FileName, PathName);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CGameObject::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, FullPath);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CGameObject::SetTexture(const std::string& Name,
	const std::vector<std::wstring>& vecFileName,
	const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name,
		vecFileName, PathName);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CGameObject::SetTextureColorKey(unsigned char r, 
	unsigned char g, unsigned char b, int Index)
{
	if (m_Texture)
		m_Texture->SetColorKey(r, g, b, Index);
}

void CGameObject::Start()
{
	m_Start = true;
}

bool CGameObject::Init()
{
	return true;
}

void CGameObject::Update(float DeltaTime)
{
	// Start�Լ��� ��ϵ� Key�鿡 ���� �ݹ��Լ����� ����ϴ�
	// �͵��� ��������Ƿ�(ex. CPlayer::Start�� Player��
	// �̵��ϴ� Ű�� �ݹ� ����ϴ� �ڵ���� ��Ƴ���),
	// �ѹ��� ���ָ� �ȴ�. �׷��� m_Start�� false�϶���
	// �ѹ��� ���ش�.
	if (!m_Start)
	{
		Start();
	}

	if (m_Animation)
		m_Animation->Update(DeltaTime);

	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; )
	{
		if (!(*iter)->IsActive())
		{
			iter = m_ColliderList.erase(iter);
			iterEnd = m_ColliderList.end();
			continue;
		}

		// ��Ȱ��ȭ �Ǿ��ִ� �浹ü�� Update���� �ʴ´�
		else if ((*iter)->GetEnable())
		{
			(*iter)->Update(DeltaTime);
		}

		++iter;
	}
}

void CGameObject::PostUpdate(float DeltaTime)
{
	if (m_Animation)
	{
		AnimationInfo* AnimInfo = m_Animation->m_CurrentAnimation;

		const AnimationFrameData& FrameData =
			AnimInfo->Sequence->GetFrameData(AnimInfo->Frame);

		m_Size = FrameData.Size;
	}

	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; )
	{
		if (!(*iter)->IsActive())
		{
			iter = m_ColliderList.erase(iter);
			iterEnd = m_ColliderList.end();
			continue;
		}

		// ��Ȱ��ȭ �Ǿ��ִ� �浹ü�� PostUpdate���� �ʴ´�
		else if ((*iter)->GetEnable())
		{
			(*iter)->PostUpdate(DeltaTime);
		}

		++iter;
	}
}

void CGameObject::Collision(float DeltaTime)
{
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		m_Scene->GetSceneCollision()->AddCollider(*iter);
	}
}

void CGameObject::PrevRender()
{
	CCamera* Camera = m_Scene->GetCamera();

	m_RenderPos = m_Pos - Camera->GetPos();

	Vector2 Size = m_Size;

	if (m_Animation)
	{
		AnimationInfo* AnimInfo = m_Animation->m_CurrentAnimation;

		const AnimationFrameData& FrameData = AnimInfo->Sequence->GetFrameData(AnimInfo->Frame);

		Size = FrameData.Size;
	}

	// ī�޶� ��ǥ���� LeftTop��ǥ(World��ǥ �ƴ�!)
	Vector2 LT = m_RenderPos - m_Pivot * Size + m_Offset;

	m_CameraCull = false;

	if (LT.x + Size.x <= 0.f)
		m_CameraCull = true;

	else if (LT.x >= Camera->GetResolution().x)
		m_CameraCull = true;

	else if (LT.y + Size.y <= 0.f)
		m_CameraCull = true;

	else if (LT.y >= Camera->GetResolution().y)
		m_CameraCull = true;
}

void CGameObject::Render(HDC hDC)
{
	if (m_Animation)
	{
		AnimationInfo* AnimInfo = m_Animation->m_CurrentAnimation;

		// CAnimation::Update���� m_CurrentAnimation�� Frame ��ȣ��
		// �ð��� ������ ���� ��� update���ֹǷ�, GetFrameData���ڿ���
		// ������ �´� Frame��ȣ�� �Ѱ��� ���̰�, GetFrameData��
		// CAnimationSequence�� m_vecFrameData���� Frame��ȣ�� �´�
		// FrameData�� �Ѱ��� ���̴�
		const AnimationFrameData& FrameData =
			AnimInfo->Sequence->GetFrameData(AnimInfo->Frame);

		Vector2 LT = m_RenderPos - m_Pivot * FrameData.Size + m_Offset;

		if (AnimInfo->Sequence->GetTextureType() == ETexture_Type::Atlas)
		{
			AnimInfo->Sequence->GetTexture()->Render(hDC, LT,
				FrameData.StartPos, FrameData.Size);
		}

		// Frame���
		else
		{
			AnimInfo->Sequence->GetTexture()->Render(hDC, LT,
				FrameData.StartPos, FrameData.Size, AnimInfo->Frame);
		}

	}

	else
	{
		Vector2 LT = m_RenderPos - m_Pivot * m_Size + m_Offset;

		if (m_Texture)
		{
			m_Texture->Render(hDC, LT, m_ImageStart, m_Size);
		}

		else
		{
			Rectangle(hDC, (int)m_Pos.x, (int)m_Pos.y,
				(int)(m_Pos.x + m_Size.x), (int)(m_Pos.y + m_Size.y));
		}
	}

	// ����׸���� �浹ü ����� �׷��� ����
	// #ifdef _DEBUG���� define�� Collider�� �ڽ� Ŭ��������
	// Render�Լ��� ���ǵǾ��� ����
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; )
	{
		if (!(*iter)->IsActive())
		{
			iter = m_ColliderList.erase(iter);
			iterEnd = m_ColliderList.end();
			continue;
		}

		else if ((*iter)->GetEnable())
		{
			(*iter)->Render(hDC);
		}

		++iter;
	}

	m_PrevPos = m_Pos;
	m_Velocity = Vector2(0.f, 0.f);
}

CGameObject* CGameObject::Clone()
{
	return nullptr;
}
