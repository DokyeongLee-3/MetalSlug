
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
	// case1: 반지름만큼 left, top, right, bottom 방향으로 확장된 사각형이 있을 때
	// 원의 중심이 그 안에 들어간다면 충돌 
	if ((Src.Left <= Dest.Center.x && Dest.Center.x <= Src.Right) ||
		(Src.Top <= Dest.Center.y && Dest.Center.y <= Src.Bottom))
	{
		RectInfo RC = Src;
		RC.Left -= Dest.Radius;
		RC.Top -= Dest.Radius;
		RC.Right += Dest.Radius;
		RC.Bottom += Dest.Radius;

		// 원의 반지름만큼 확장된 사각형안에 원의 중심이 들어오는지
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
	// case2: 원이 사각형의 꼭지점 방향으로 충돌할 때는 원의 중심이 확장된
	// 사각형의 영역을 넘어서 더 들어와야 한다. 따라서 2번이 그 경우인데
	// 원래 사각형의 꼭지점이 원 안에 존재한다면 그것 역시 충돌 
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

	// 아래 두 멤버는 CCollider의 멤버
	// 만약 player와 stage 충돌 체크를 위해 stage가 
	// CollisionRectToPixel를 부르고 m_bFloorCollision을
	// true로 만들고 그대로 두고 리턴하고
	// CCollisionManager::Collision(fDeltaTime)에서
	// 충돌 체크할 다음 element(*iter1)가 Arabian이면
	// 또 stage가 CollisionRectToPixel를 부를텐데 그때
	// ColliderPixel의 m_bCamCollision, m_bFloorCollision가
	// 이미 true일 것이다. 충돌 체크를 하기도 전에 둘중 하나라도
	// true라는건 충돌 체크에 오류를 발생시킴
	Src->SetCamCollision(false);
	Src->SetFloorCollision(false);

	int Width = (int)(Dest->GetWidth());
	

	for (int i = StartY; i <= EndY; i++)
	{
		for (int j = StartX; j <= EndX; j++)
		{
			int idx = i * Width + j;
			const PIXEL& pixel = Dest->GetPixel(idx);

			//// 카메라 충돌체
			if (pixel.r == 0 && pixel.g == 255 && pixel.b == 255)
			{
				//m_tHitPoint.x = j;
				//m_tHitPoint.y = i;

				// 카메라 충돌체랑 충돌했다면 동시에 바닥 충돌체랑
				// 충돌했을 것이므로 계속 for loop를 돌게해서 바닥이랑
				// rect의 hit point를 찾아내게 한다.
				// rect의 충돌 callback함수(ex. CPlayer::Hit)에서
				// 그 hit point(바닥)에 계속 닿아 있게 한다
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

	// 아래 두 멤버는 CCollider의 멤버
	// 만약 player와 stage 충돌 체크를 위해 stage가 
	// CollisionRectToPixel를 부르고 m_bFloorCollision을
	// true로 만들고 그대로 두고 리턴하고
	// CCollisionManager::Collision(fDeltaTime)에서
	// 충돌 체크할 다음 element(*iter1)가 Arabian이면
	// 또 stage가 CollisionRectToPixel를 부를텐데 그때
	// ColliderPixel의 m_bCamCollision, m_bFloorCollision가
	// 이미 true일 것이다. 충돌 체크를 하기도 전에 둘중 하나라도
	// true라는건 충돌 체크에 오류를 발생시킴
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
 