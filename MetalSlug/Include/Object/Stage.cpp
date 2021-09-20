
#include "Stage.h"

CStage::CStage()
{
}

CStage::CStage(const CStage& obj)	:
	CGameObject(obj)
{
}

CStage::~CStage()
{
}

void CStage::Start()
{
}

bool CStage::Init()
{
	if (!CGameObject::Init())
		return false;

	m_Pos = Vector2(0.f, 0.f);
	SetPivot(0.f, 0.f);

	SetTexture("Stage", TEXT("Stage/Stage1.bmp"));

	return true;
}

void CStage::Update(float DeltaTime)
{
}

void CStage::PostUpdate(float DeltaTime)
{
}

void CStage::Collision(float DeltaTime)
{
}

void CStage::PrevRender()
{
}

void CStage::Render(HDC hDC)
{
}

CGameObject* CStage::Clone()
{
	return new CStage(*this);
}
