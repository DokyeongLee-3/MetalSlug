
#include "NumberWidget.h"
#include "UIWindow.h"

CNumberWidget::CNumberWidget() :
	m_Number(0)
{
}

CNumberWidget::CNumberWidget(const CNumberWidget& widget) :
	CUIImage(widget)
{
	m_Number = widget.m_Number;
}

CNumberWidget::~CNumberWidget()
{
}

bool CNumberWidget::Init()
{
	return true;
}

void CNumberWidget::Update(float DeltaTime)
{
	// 지금은 Credit이 9를 넘지 않도록 설정했으므로
	// 자릿수를 나누는 Logic은 당장 필요하지 않을 것
	//int Number = m_Number;

	//std::stack<int> NumberStack;

	//while (Number > 0)
	//{
	//	NumberStack.push(Number % 10);
	//	Number /= 10;
	//}

	//m_vecNumber.clear();

	//while (!NumberStack.empty())
	//{
	//	m_vecNumber.push_back(NumberStack.top());
	//	NumberStack.pop();
	//}
}

void CNumberWidget::PostUpdate(float DeltaTime)
{
	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth(m_Number);
		m_Size.y = (float)m_Texture->GetHeight(m_Number);
	}
}

void CNumberWidget::Collision(float DeltaTime)
{
}

void CNumberWidget::Render(HDC hDC)
{
	//if (m_vecNumber.empty())
	//{
	//	// 비어 있으면 0을 출력
	//	m_FrameIndex = 0;
	//	CUIImage::Render(hDC);
	//}


	Vector2 Pos = m_Pos + m_Owner->GetPos();

	m_Texture->Render(hDC, Pos, Vector2(0.f, 0.f), m_Size, m_Number);

}


CNumberWidget* CNumberWidget::Clone()
{
	return new CNumberWidget(*this);
}
