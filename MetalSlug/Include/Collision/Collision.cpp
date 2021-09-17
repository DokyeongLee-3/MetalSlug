
#include "Collision.h"
#include "ColliderBox.h"
#include "ColliderSphere.h"

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
 