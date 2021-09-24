
#include "Collision.h"
#include "ColliderBox.h"
#include "ColliderSphere.h"
#include "ColliderPixel.h"

bool CCollision::CollisionBoxToBox(CColliderBox* Src, CColliderBox* Dest)
{
	if (CollisionBoxToBox(Src->GetInfo(), Dest->GetInfo()))
	{
		return true;
	}
	return false;
}

bool CCollision::CollisionBoxToBox(const RectInfo& Src, const RectInfo& Dest)
{
	if (Src.Left > Dest.Right)
		return false;
	else if (Dest.Left > Src.Right)
		return false;
	else if (Src.Top > Dest.Bottom)
		return false;
	else if (Dest.Top > Src.Bottom)
		return false;

	return true;
}

bool CCollision::CollisionSphereToSphere(CColliderSphere* Src, CColliderSphere* Dest)
{
	if (CollisionSphereToSphere(Src->GetInfo(), Dest->GetInfo()))
	{
		return true;
	}

	return false;
}

bool CCollision::CollisionSphereToSphere(const SphereInfo& Src, const SphereInfo& Dest)
{
	float Dist = Distance(Src.Center, Dest.Center);

	return Dist <= Src.Radius + Dest.Radius;
}

bool CCollision::CollisionBoxToSphere(CColliderBox* Src, CColliderSphere* Dest)
{
	if (CollisionBoxToSphere(Src->GetInfo(), Dest->GetInfo()))
	{
		return true;
	}

	return false;
}

bool CCollision::CollisionBoxToSphere(const RectInfo& Src, const SphereInfo& Dest)
{
	// case1: ��������ŭ left, top, right, bottom �������� Ȯ��� �簢���� ���� ��
	// ���� �߽��� �� �ȿ� ���ٸ� �浹 
	if ((Src.Left <= Dest.Center.x && Dest.Center.x <= Src.Right) ||
		(Src.Top <= Dest.Center.y && Dest.Center.y <= Src.Bottom))
	{
		RectInfo RC = Src;
		RC.Left -= Dest.Radius;
		RC.Top -= Dest.Radius;
		RC.Right += Dest.Radius;
		RC.Bottom += Dest.Radius;

		// ���� ��������ŭ Ȯ��� �簢���ȿ� ���� �߽��� ��������
		if (RC.Left > Dest.Center.x)
			return false;

		else if (RC.Right < Dest.Center.x)
			return false;

		else if (Dest.Center.y < RC.Top)
			return false;

		else if (RC.Bottom < Dest.Center.y)
			return false;

		return true;
	}
	// case2: ���� �簢���� ������ �������� �浹�� ���� ���� �߽��� Ȯ���
	// �簢���� ������ �Ѿ �� ���;� �Ѵ�. ���� 2���� �� ����ε�
	// ���� �簢���� �������� �� �ȿ� �����Ѵٸ� �װ� ���� �浹 
	Vector2	Pos[4] = {};

	Pos[0] = Vector2(Src.Left, Src.Top);
	Pos[1] = Vector2(Src.Right, Src.Top);
	Pos[2] = Vector2(Src.Left, Src.Bottom);
	Pos[3] = Vector2(Src.Right, Src.Bottom);

	for (int i = 0; i < 4; ++i)
	{
		float Dist = Distance(Pos[i], Dest.Center);

		if (Dist <= Dest.Radius)
			return true;
	}

	return false;
}

bool CCollision::CollisionBoxToPixel(CColliderBox* Src,
	CColliderPixel* Dest)
{
	Src->SetHitPoint(-1.f, -1.f);
	Dest->SetHitPoint(-1.f, -1.f);

	int StartX, EndX;
	int StartY, EndY;

	RectInfo Info = Src->GetInfo();

	StartX = Info.Left < 0 ? 0 : (int)Info.Left;
	EndX = Info.Right > Dest->GetWidth() ? (int)Dest->GetWidth() - 1 : (int)Info.Right;

	StartY = Info.Top < 0 ? 0 : (int)Info.Top;
	EndY = Info.Bottom > Dest->GetHeight() ? (int)Dest->GetHeight() - 1 : (int)Info.Bottom;

	// �Ʒ� �� ����� CCollider�� ���
	// ���� player�� stage �浹 üũ�� ���� stage�� 
	// CollisionRectToPixel�� �θ��� m_bFloorCollision��
	// true�� ����� �״�� �ΰ� �����ϰ�
	// CCollisionManager::Collision(fDeltaTime)����
	// �浹 üũ�� ���� element(*iter1)�� Arabian�̸�
	// �� stage�� CollisionRectToPixel�� �θ��ٵ� �׶�
	// ColliderPixel�� m_bCamCollision, m_bFloorCollision��
	// �̹� true�� ���̴�. �浹 üũ�� �ϱ⵵ ���� ���� �ϳ���
	// true��°� �浹 üũ�� ������ �߻���Ŵ
	Src->SetCamCollision(false);
	Src->SetFloorCollision(false);

	int Width = (int)(Dest->GetWidth());
	

	for (int i = StartY; i <= EndY; i++)
	{
		for (int j = StartX; j <= EndX; j++)
		{
			int idx = i * Width + j;
			const PIXEL& pixel = Dest->GetPixel(idx);

			//// ī�޶� �浹ü
			if (pixel.r == 0 && pixel.g == 255 && pixel.b == 255)
			{
				//m_tHitPoint.x = j;
				//m_tHitPoint.y = i;

				// ī�޶� �浹ü�� �浹�ߴٸ� ���ÿ� �ٴ� �浹ü��
				// �浹���� ���̹Ƿ� ��� for loop�� �����ؼ� �ٴ��̶�
				// rect�� hit point�� ã�Ƴ��� �Ѵ�.
				// rect�� �浹 callback�Լ�(ex. CPlayer::Hit)����
				// �� hit point(�ٴ�)�� ��� ��� �ְ� �Ѵ�
				Src->SetCamCollision(true);
			}

			if (pixel.r == 255 && pixel.g == 0 && pixel.b == 255)
			{
				Src->SetHitPoint((float)j, (float)i);

				Src->SetFloorCollision(true);

				return true;
			}
		}
	}

	return Src->GetCamCollision() || Src->GetFloorCollision();
}

bool CCollision::CollisionSphereToPixel(CColliderSphere* Src, CColliderPixel* Dest)
{
	Src->SetHitPoint(-1.f, -1.f);
	Dest->SetHitPoint(-1.f, -1.f);

	int StartX, EndX;
	int StartY, EndY;

	SphereInfo Info = Src->GetInfo();

	RectInfo RcInfo = { Info.Center.x - Info.Radius, Info.Center.y - Info.Radius,
	Info.Center.x + Info.Radius, Info.Center.y + Info.Radius };

	StartX = RcInfo.Left < 0 ? 0 : (int)RcInfo.Left;
	EndX = RcInfo.Right > Dest->GetWidth() ? (int)Dest->GetWidth() - 1 : (int)RcInfo.Right;

	StartY = RcInfo.Top < 0 ? 0 : (int)RcInfo.Top;
	EndY = RcInfo.Bottom > Dest->GetHeight() ? (int)Dest->GetHeight() - 1 : (int)RcInfo.Bottom;

	// �Ʒ� �� ����� CCollider�� ���
	// ���� player�� stage �浹 üũ�� ���� stage�� 
	// CollisionRectToPixel�� �θ��� m_bFloorCollision��
	// true�� ����� �״�� �ΰ� �����ϰ�
	// CCollisionManager::Collision(fDeltaTime)����
	// �浹 üũ�� ���� element(*iter1)�� Arabian�̸�
	// �� stage�� CollisionRectToPixel�� �θ��ٵ� �׶�
	// ColliderPixel�� m_bCamCollision, m_bFloorCollision��
	// �̹� true�� ���̴�. �浹 üũ�� �ϱ⵵ ���� ���� �ϳ���
	// true��°� �浹 üũ�� ������ �߻���Ŵ
	Src->SetFloorCollision(false);
	int Width = (int)(Dest->GetWidth());

	for (int i = StartY; i <= EndY; i++)
	{
		for (int j = StartX; j <= EndX; j++)
		{
			int idx = i * Width + j;
			const PIXEL& pixel = Dest->GetPixel(idx);

			if (pixel.r == 255 && pixel.g == 0 && pixel.b == 255)
			{
				if (Distance(Vector2((float)j, (float)i), Vector2(Info.Center)) <= Info.Radius)
				{
					Src->SetHitPoint((float)j, (float)i);

					Src->SetFloorCollision(true);
					return true;
				}
			}
		}
	}

	return false;

}
 