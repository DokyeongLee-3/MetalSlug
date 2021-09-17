
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

	// CButton::SetTexture에서 m_Size는 Texture의 Width, Height로
	// 설정되어 있는데, 이러면 Atlas방식처럼 여러 상태의 프레임이
	// 하나의 이미지에 들어있으면 CUIWidget::CollisionMouse에서
	// m_Size를 사용할 때 문제가 된다(실제 크기보다 Widget의 크기가
	// 크게 계산된다) 따라서 여기서 m_Size도 한 Frame의 크기에
	// 맞게 바꿔준다
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

			// 버튼을 눌렀다가 이제 막 뗀 경우
			else if (m_ButtonState == EButton_State::Click &&
				CInput::GetInst()->GetMouseUp())
			{
				// 버튼의 기능 동작
				if (m_ButtonClickCallback)
					m_ButtonClickCallback();

				m_ButtonState = EButton_State::MouseOn;
			}

			// 버튼위에서 계속 클릭하고 있는 경우
			else if (m_ButtonState == EButton_State::Click &&
				CInput::GetInst()->GetMousePush())
			{
				m_ButtonState = EButton_State::Click;
			}

			// 마우스가 버튼 위에 있기만 한 경우
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

	// 굳이 필요한가?
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


