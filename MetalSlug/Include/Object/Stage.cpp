
#include "Stage.h"
#include "../Scene/Camera.h"
#include "../Scene/Scene.h"
#include "../GameManager.h"
#include "../Collision/ColliderPixel.h"
#include "../Collision/ColliderBox.h"

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
	m_ZOrder = 3;
	SetPivot(0.f, 0.f);
	m_ObjType = EObject_Type::Stage;

	m_PhysicsSimulate = false;

	SetTexture("Stage", TEXT("Stage/Stage1.bmp"));
	SetTextureColorKey(255, 255, 255);

	CColliderPixel* Pixel = AddCollider<CColliderPixel>("StagePixel");
	Pixel->SetPixelInfo(TEXT("Stage/Stage2_Collider.bmp"));
	Pixel->SetExtent((int)STAGE_WIDTH, (int)STAGE_HEIGHT);
	Pixel->SetCollisionProfile("Default");
	Pixel->SetStartPos(0.f, 0.f);

	CColliderBox* ArabianGen = AddCollider<CColliderBox>("GenPhase1");
	ArabianGen->SetExtent(60, 200);
	ArabianGen->SetOffset(Vector2(1450.f, 620.f));
	ArabianGen->SetCollisionProfile("GenTrigger");

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

		auto iter = m_ColliderList.begin();
		auto iterEnd = m_ColliderList.end();

		for (; iter != iterEnd; )
		{
			std::string Name = (*iter)->GetName();

			if (!(*iter)->IsActive())
			{
				iter = m_ColliderList.erase(iter);
				iterEnd = m_ColliderList.end();
				continue;
			}

			else if ((*iter)->GetEnable() && Name != "StagePixel")
			{
				(*iter)->Render(hDC);
			}

			++iter;
		}
	}
}

CStage* CStage::Clone()
{
	return new CStage(*this);
}
