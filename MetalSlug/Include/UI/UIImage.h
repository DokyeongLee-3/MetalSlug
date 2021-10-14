#pragma once
#include "UIWidget.h"
#include "../Resource/Texture.h"
#include "../Animation/Animation.h"

class CUIImage :
    public CUIWidget
{
public:
    CUIImage();
    CUIImage(const CUIImage& widget);
    virtual ~CUIImage();

protected:
    CSharedPtr<CTexture>    m_Texture;
    CAnimation* m_Animation;
    std::vector<AnimationFrameData> m_vecFrameData;
    int m_FrameIndex;
    Vector2 m_Offset;

public:
    void CreateAnimation();
    void AddAnimation(const std::string& SequenceName,
        bool Loop = true, float PlayTime = 1.f, float PlayScale = 1.f,
        bool Reverse = false);
    void ChangeAnimation(const std::string& Name);
    void SetPlayTime(const std::string& Name,
        float PlayTime);
    void SetLoop(const std::string& Name, bool Loop);

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

    void SetTexture(const std::string& Name);
    void SetTexture(const std::string& Name, const TCHAR* FileName,
        const std::string& PathName = TEXTURE_PATH);
    void SetTextureFullPath(const std::string& Name, const TCHAR* FullPath);
    void SetTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName,
        const std::string& PathName = TEXTURE_PATH);
    void SetTextureColorKey(unsigned char r,
        unsigned char g, unsigned char b, int Index = 0);

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Collision(float DeltaTime);
    virtual void Render(HDC hDC);
    virtual void Render(const Vector2& Pos, HDC hDC);

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
};

