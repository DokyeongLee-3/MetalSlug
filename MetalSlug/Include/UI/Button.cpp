
#include "Button.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Input.h"

CButton::CButton()
{
	m_ButtonState = EButton_State::Normal;
}

CButton::CButton(const CButton& widget)
{
}

CButton::~CButton()
{
}

void CButton::SetTexture(const std::string& Name)
{
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();

		for (int i = 0; i < (int)EButton_State::End; ++i)
		{
			m_FrameData[i].Size = m_Size;
		}
	}
}

void CButton::SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, FileName);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();

		for (int i = 0; i < (int)EButton_State::End; ++i)
		{
			m_FrameData[i].Size = m_Size;
		}
	}
}

void CButton::SetTextureFullPath(const std::string& Name,
	const TCHAR* FullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, FullPath);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();

		for (int i = 0; i < (int)EButton_State::End; ++i)
		{
			m_FrameData[i].Size = m_Size;
		}
	}
}

void CButton::SetTexture(const std::string& Name, 
	const std::vector<std::wstring>& vecFileName,
	const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, vecFileName, PathName);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();

		for (int i = 0; i < (int)EButton_State::End; ++i)
		{
			m_FrameData[i].Size.x = (float)m_Texture->GetWidth(i);
			m_FrameData[i].Size.y = (float)m_Texture->GetHeight(i);
		}
	}
}

void CButton::SetTextureColorKey(unsigned char r,
	unsigned char g, unsigned char b, int Index)
{
	if (m_Texture)
	{
		m_Texture->SetColorKey(r, g, b, Index);
	}
}

void CButton::SetFrameData(EButton_State State,
	const Vector2& Start, const Vector2& Size)
{
	m_FrameData[(int)State].StartPos = Start;
	m_FrameData[(int)State].Size = Size;

	// CButton::SetTexture���� m_Size�� Texture�� Width, Height��
	// �����Ǿ� �ִµ�, �̷��� Atlas���ó�� ���� ������ ��������
	// �ϳ��� �̹����� ��������� CUIWidget::CollisionMouse����
	// m_Size�� ����� �� ������ �ȴ�(���� ũ�⺸�� Widget�� ũ�Ⱑ
	// ũ�� ���ȴ�) ���� ���⼭ m_Size�� �� Frame�� ũ�⿡
	// �°� �ٲ��ش�
	m_Size = Size;
}

bool CButton::Init()
{
	return true;
}

void CButton::Update(float DeltaTime)
{
	if (m_ButtonState != EButton_State::Disable)
	{
		if (m_MouseHovered)
		{
			if (CInput::GetInst()->GetMouseDown())
			{
				m_ButtonState = EButton_State::Click;
			}

			// ��ư�� �����ٰ� ���� �� �� ���
			else if (m_ButtonState == EButton_State::Click &&
				CInput::GetInst()->GetMouseUp())
			{
				// ��ư�� ��� ����
				if (m_ButtonClickCallback)
					m_ButtonClickCallback();

				m_ButtonState = EButton_State::MouseOn;
			}

			// ��ư������ ��� Ŭ���ϰ� �ִ� ���
			else if (m_ButtonState == EButton_State::Click &&
				CInput::GetInst()->GetMousePush())
			{
				m_ButtonState = EButton_State::Click;
			}

			// ���콺�� ��ư ���� �ֱ⸸ �� ���
			else
			{
				m_ButtonState = EButton_State::MouseOn;

				if (m_ButtonMouseOnCallback)
					m_ButtonMouseOnCallback();
			}
		}

		else
		{
			m_ButtonState = EButton_State::Normal;
		}
	}

	// ���� �ʿ��Ѱ�?
	m_Size = m_FrameData[(int)m_ButtonState].Size;
}

void CButton::PostUpdate(float DeltaTime)
{
}

void CButton::Collision(float DeltaTime)
{
}

void CButton::Render(HDC hDC)
{
	if (m_Texture)
	{
		Vector2 Pos = m_Pos + m_Owner->GetPos();

		if (m_Texture->GetTextureType() == ETexture_Type::Atlas)
		{
			m_Texture->Render(hDC, Pos, 
				m_FrameData[(int)m_ButtonState].StartPos,
				m_FrameData[(int)m_ButtonState].Size);
		}

		else
		{
			m_Texture->Render(hDC, Pos, Vector2(0.f, 0.f),
				m_Size, (int)m_ButtonState);
		}


	}
}

void CButton::CollisionMouseHoveredCallback(float DeltaTime)
{
	CUIWidget::CollisionMouseHoveredCallback(DeltaTime);
}

void CButton::CollisionMouseReleaseCallback(float DeltaTime)
{
	CUIWidget::CollisionMouseReleaseCallback(DeltaTime);
}


