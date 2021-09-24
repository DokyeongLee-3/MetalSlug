
#include "Stage.h"
#include "../Scene/Camera.h"
#include "../Scene/Scene.h"
#include "../GameManager.h"
#include "../Collision/ColliderPixel.h"

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
	m_Pos = Vector2(0.f, 0.f);
	SetPivot(0.f, 0.f);

	m_PhysicsSimulate = false;

	SetTexture("Stage", TEXT("Stage/Stage1.bmp"));
	SetTextureColorKey(255, 255, 255);

	CColliderPixel* Pixel = AddCollider<CColliderPixel>("StagePixel");
	Pixel->SetPixelInfo(TEXT("Stage/Stage2_Collider.bmp"));
	Pixel->SetExtent((int)STAGE_WIDTH, (int)STAGE_HEIGHT);
	Pixel->SetCollisionProfile("Default");


	return true;
}

void CStage::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CStage::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CStage::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CStage::PrevRender()
{
}

void CStage::Render(HDC hDC)
{
	if (m_Texture)
	{
		Vector2 LT = m_Pos - m_Size * m_Pivot;
		Vector2 CamPos = m_Scene->GetCamera()->GetPos();

		// 화면의 LT 위치에 World크기로
		// 원본DC에 그려진 것의 CamPos 
		// 위치부터 추출하여 그린다
		m_Texture->Render(hDC, LT, CamPos, 
			Vector2((float)CGameManager::GetInst()->GetResolution().Width,
				(float)CGameManager::GetInst()->GetResolution().Height));
	}
}

CStage* CStage::Clone()
{
	return new CStage(*this);
}
