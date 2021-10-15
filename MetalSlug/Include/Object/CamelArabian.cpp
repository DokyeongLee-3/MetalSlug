
#include "CamelArabian.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/SceneCollision.h"

CCamelArabian::CCamelArabian()	:
	m_Target(nullptr),
	m_Changing(false),
	m_TopAnimation(nullptr),
	m_BottomAnimation(nullptr)
{
}

CCamelArabian::CCamelArabian(const CCamelArabian& obj)	:
	CMonster(obj)
{
}

CCamelArabian::~CCamelArabian()
{
	SAFE_DELETE(m_TopAnimation);
	SAFE_DELETE(m_BottomAnimation);
}

void CCamelArabian::Start()
{
}

bool CCamelArabian::Init()
{
	if (!CMonster::Init())
		return false;

	m_ObjType = EObject_Type::Monster;
	m_State = ECamelArabian_State::Idle;


	m_CharacterInfo.HP = 200;
	m_CharacterInfo.HPMax = 200;

	m_ZOrder = 6;
	m_MoveSpeed = 70.f;
	m_Pivot = Vector2(0.5f, 1.f);
	SetTarget((CPlayer*)m_Scene->GetPlayer());

	AddCamelArabianAnimation();

	return true;
}

void CCamelArabian::Update(float DeltaTime)
{
	if (!m_Start)
	{
		Start();
	}

	if (m_TopAnimation)
		m_TopAnimation->Update(DeltaTime);

	if (m_BottomAnimation)
		m_BottomAnimation->Update(DeltaTime);

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

void CCamelArabian::PostUpdate(float DeltaTime)
{
	m_Size = Vector2(0.f, 0.f);

	if (m_TopAnimation)
	{
		AnimationInfo* AnimInfo = m_TopAnimation->m_CurrentAnimation;

		const AnimationFrameData& FrameData =
			AnimInfo->Sequence->GetFrameData(AnimInfo->Frame);

		m_Size += FrameData.Size;
	}

	if (m_BottomAnimation)
	{
		AnimationInfo* AnimInfo = m_BottomAnimation->m_CurrentAnimation;

		const AnimationFrameData& FrameData =
			AnimInfo->Sequence->GetFrameData(AnimInfo->Frame);

		m_Size += FrameData.Size;
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

void CCamelArabian::Collision(float DeltaTime)
{
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		m_Scene->GetSceneCollision()->AddCollider(*iter);
	}
}

void CCamelArabian::Render(HDC hDC)
{
	if (m_TopAnimation)
	{
		AnimationInfo* AnimInfo = m_TopAnimation->m_CurrentAnimation;

		// CAnimation::Update���� m_CurrentAnimation�� Frame ��ȣ��
		// �ð��� ������ ���� ��� update���ֹǷ�, GetFrameData���ڿ���
		// ������ �´� Frame��ȣ�� �Ѱ��� ���̰�, GetFrameData��
		// CAnimationSequence�� m_vecFrameData���� Frame��ȣ�� �´�
		// FrameData�� �Ѱ��� ���̴�
		const AnimationFrameData& FrameData =
			AnimInfo->Sequence->GetFrameData(AnimInfo->Frame);

		Vector2 LT = m_RenderPos - m_Pivot * FrameData.Size + FrameData.Offset + m_Offset;
		LT.y -= PLAYER_BOTTOMHEIGHT;

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

	if (m_BottomAnimation)
	{
		AnimationInfo* AnimInfo = m_BottomAnimation->m_CurrentAnimation;

		// CAnimation::Update���� m_CurrentAnimation�� Frame ��ȣ��
		// �ð��� ������ ���� ��� update���ֹǷ�, GetFrameData���ڿ���
		// ������ �´� Frame��ȣ�� �Ѱ��� ���̰�, GetFrameData��
		// CAnimationSequence�� m_vecFrameData���� Frame��ȣ�� �´�
		// FrameData�� �Ѱ��� ���̴�
		const AnimationFrameData& FrameData =
			AnimInfo->Sequence->GetFrameData(AnimInfo->Frame);

		Vector2 LT = m_RenderPos - m_Pivot * FrameData.Size + FrameData.Offset + m_Offset;

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
}

CCamelArabian* CCamelArabian::Clone()
{
	return new CCamelArabian(*this);
}

void CCamelArabian::FiniteState()
{
}

void CCamelArabian::CollisionBegin(CCollider* Src, CCollider* Dest, float DeltaTime)
{
}

void CCamelArabian::CollisionStay(CCollider* Src, CCollider* Dest, float DeltaTime)
{
}

void CCamelArabian::CollisionEnd(CCollider* Src, CCollider* Dest, float DeltaTime)
{
}

void CCamelArabian::AddTopAnimation(const std::string& SequenceName, bool Loop, float PlayTime, float PlayScale, bool Reverse)
{
	if (!m_TopAnimation)
	{
		m_TopAnimation = new CAnimation;

		m_TopAnimation->m_Owner = this;
		m_TopAnimation->m_Scene = m_Scene;
	}

	m_TopAnimation->AddAnimation(SequenceName, Loop, PlayTime, PlayScale, Reverse);
}

void CCamelArabian::AddBottomAnimation(const std::string& SequenceName, bool Loop, float PlayTime, float PlayScale, bool Reverse)
{
	if (!m_BottomAnimation)
	{
		m_BottomAnimation = new CAnimation;

		m_BottomAnimation->m_Owner = this;
		m_BottomAnimation->m_Scene = m_Scene;
	}

	m_BottomAnimation->AddAnimation(SequenceName, Loop, PlayTime, PlayScale, Reverse);
}

void CCamelArabian::ChangeBottomAnimation(const std::string& Name)
{
	if (!m_BottomAnimation)
		return;

	m_BottomAnimation->ChangeAnimation(Name);
}

void CCamelArabian::ChangeTopAnimation(const std::string& Name)
{
	if (!m_TopAnimation)
		return;

	m_TopAnimation->ChangeAnimation(Name);
}

bool CCamelArabian::CheckTopCurrentAnimation(const std::string& Name)
{
	return m_TopAnimation->CheckCurrentAnimation(Name);
}

bool CCamelArabian::CheckBottomCurrentAnimation(const std::string& Name)
{
	return m_BottomAnimation->CheckCurrentAnimation(Name);
}

void CCamelArabian::AddCamelArabianAnimation()
{
	AddTopAnimation("CamelArabianIdleLeft", true, 0.8f);
	AddBottomAnimation("CamelRunLeft", true, 1.f);
}
