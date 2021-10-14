
#include "UIImage.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CUIImage::CUIImage()    :
    m_FrameIndex(0),
    m_Animation(nullptr)
{
}

CUIImage::CUIImage(const CUIImage& widget)  :
    CUIWidget(widget)
{
    m_Texture = widget.m_Texture;
    m_FrameIndex = 0;
}

CUIImage::~CUIImage()
{
    SAFE_DELETE(m_Animation);
}

void CUIImage::CreateAnimation()
{
    if (!m_Animation)
    {
        m_Animation = new CAnimation;

        m_Animation->m_OwnerUI = this;
        m_Animation->m_Scene = m_Scene;
    }
}

void CUIImage::AddAnimation(const std::string& SequenceName, bool Loop, float PlayTime, float PlayScale, bool Reverse)
{
    if (!m_Animation)
    {
        m_Animation = new CAnimation;

        m_Animation->m_OwnerUI = this;
        m_Animation->m_Scene = m_Scene;
    }

    m_Animation->AddAnimation(SequenceName, Loop, PlayTime, PlayScale, Reverse);
}

void CUIImage::ChangeAnimation(const std::string& Name)
{
	if (!m_Animation)
		return;

	m_Animation->ChangeAnimation(Name);

}

void CUIImage::SetPlayTime(const std::string& Name, float PlayTime)
{
    if (!m_Animation)
        return;

    m_Animation->SetPlayTime(Name, PlayTime);
}

void CUIImage::SetLoop(const std::string& Name, bool Loop)
{
    if (!m_Animation)
        return;

    m_Animation->SetLoop(Name, Loop);
}


void CUIImage::SetTexture(const std::string& Name)
{
    CTexture* Texture = m_Scene->GetSceneResource()->FindTexture(Name);

    if (Texture)
    {
        m_Size.x = (float)Texture->GetWidth();
        m_Size.y = (float)Texture->GetHeight();
    }

}

void CUIImage::SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
    m_Scene->GetSceneResource()->LoadTexture(Name, 
        FileName, PathName);

    m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

    if (m_Texture)
    {
        m_Size.x = (float)m_Texture->GetWidth();
        m_Size.y = (float)m_Texture->GetHeight();
    }

}

void CUIImage::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
    m_Scene->GetSceneResource()->LoadTextureFullPath(Name,
        FullPath);

    m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

    if (m_Texture)
    {
        m_Size.x = (float)m_Texture->GetWidth();
        m_Size.y = (float)m_Texture->GetHeight();
    }
}

void CUIImage::SetTexture(const std::string& Name,
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

void CUIImage::SetTextureColorKey(unsigned char r, unsigned char g, unsigned char b, int Index)
{
    if (m_Texture)
        m_Texture->SetColorKey(r, g, b, Index);
}

bool CUIImage::Init()
{
    return true;
}

void CUIImage::Update(float DeltaTime)
{
    if (m_Animation)
    {
        m_Animation->Update(DeltaTime);
    }
}

void CUIImage::PostUpdate(float DeltaTime)
{
    if (m_Animation)
    {
        AnimationInfo* AnimInfo = m_Animation->m_CurrentAnimation;

        const AnimationFrameData& FrameData =
            AnimInfo->Sequence->GetFrameData(AnimInfo->Frame);

        m_Size = FrameData.Size;
    }
}

void CUIImage::Collision(float DeltaTime)
{
}

void CUIImage::Render(HDC hDC)
{
    if (m_Animation)
    {
        AnimationInfo* AnimInfo = m_Animation->m_CurrentAnimation;

        // CAnimation::Update에서 m_CurrentAnimation의 Frame 번호를
        // 시간이 지남에 따라 계속 update해주므로, GetFrameData인자에는
        // 순서에 맞는 Frame번호를 넘겨줄 것이고, GetFrameData는
        // CAnimationSequence의 m_vecFrameData에서 Frame번호에 맞는
        // FrameData를 넘겨줄 것이다
        const AnimationFrameData& FrameData =
            AnimInfo->Sequence->GetFrameData(AnimInfo->Frame);

        if (AnimInfo->Sequence->GetTextureType() == ETexture_Type::Atlas)
        {
            AnimInfo->Sequence->GetTexture()->Render(hDC, m_Pos,
                FrameData.StartPos, FrameData.Size);
        }

        // Frame방식
        else
        {
            AnimInfo->Sequence->GetTexture()->Render(hDC, m_Pos,
                FrameData.StartPos, FrameData.Size, AnimInfo->Frame);
        }

    }

    else if (m_Texture)
    {
        std::string Name = m_Name;

        // 내가 속해있는 CUIWindow로부터 상대적으로
        // 떨어져 있는 만큼으로 CUIImage의 위치 설정
        Vector2 Pos = m_Pos + m_Owner->GetPos();
        
        m_Texture->Render(hDC, Pos, Vector2(0.f, 0.f), m_Size);
    }
}

void CUIImage::Render(const Vector2& Pos, HDC hDC)
{
    if (m_Texture)
    {
        Vector2 ImagePos;
        Vector2 Size = m_Size;

        // UIImage가 애니메이션이 있을 경우(ex. Mouse)
        if (!m_vecFrameData.empty())
        {
            ImagePos = m_vecFrameData[m_FrameIndex].StartPos;
            Size = m_vecFrameData[m_FrameIndex].Size;

            if (m_Texture->GetTextureType() == ETexture_Type::Frame)
            {
                m_Texture->Render(hDC, Pos + m_Offset, ImagePos, Size, m_FrameIndex);
            }

            else
            {
                m_Texture->Render(hDC, Pos + m_Offset, ImagePos, Size);
            }
        }

        // 애니메이션이 없을 경우
        // ex) DamageFont
        else
        {
            if (m_Texture->GetTextureType() == ETexture_Type::Frame)
            {
                m_Texture->Render(hDC, Pos + m_Offset, ImagePos, Size, m_FrameIndex);
            }

            else
            {
                m_Texture->Render(hDC, Pos + m_Offset, ImagePos, Size);
            }
        }
    }
}
