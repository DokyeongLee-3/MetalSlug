
#include "Math.h"
#include "GameInfo.h"


Vector2::Vector2()	:
	x(0.f),
	y(0.f)
{
}

Vector2::Vector2(float _x, float _y)	:
	x(_x),
	y(_y)
{
}

Vector2::Vector2(const Vector2& v)	:
	x(v.x),
	y(v.y)
{
}

float RadianToDegree(float Radian)
{
	return Radian * 180.f / PI;
}

float DegreeToRadian(float Degree)
{
	return Degree * PI / 180.f;
}

float Distance(const Vector2& Src, const Vector2& Dest)
{
	Vector2 result = Src - Dest;

	return result.Length();
}

float GetAngle(const Vector2& Src, const Vector2& Dest)
{
	// 빗변 길이를 구한다.
	float c = Distance(Src, Dest);
	// 밑변 길이
	// Width가 음수가 나와서 Width / c가 음수가 되면
	// 90도보다 크다 -> Src의 x좌표보다 Dest의 x좌표가 작다
	// Width가 양수가 나와서 Width / c가 양수가 되면
	// 90도보다 작다 -> Src의 x좌표보다 Dest의 x좌표가 크다
	float Width = Dest.x - Src.x;

	// Angle은 cos(theta)값.
	float Angle = Width / c;

	// theta값(라디안)
	Angle = RadianToDegree(acosf(Angle));

	// 만약 Dest가 Src보다 위에 있다면 360도를 빼준다
	// 시계를 생각했을 때 src를 시계의 중심, dest를 시계침 끝이라고 생각해보자
	// 각도는 윈도우 좌표계와 삼각함수를 생각하면 시계침이 3시를 가르킬 때가
	// theta가 0이고 시계 방향으로 돌아가면 theta가 증가(윈도우 좌표계가
	// 아래로 내려갈 때 y좌표가 증가하니 원래 데카르트 좌표계와 y좌표만 반대니까
	// 그렇다) 그러면 예를 들어서 src와 dest의 각도가 5PI / 6(150도)일때랑
	// 7PI / 6(210도)일때랑 GetAngle에서 RadianToDegree(acosf(Angle))로 Angle을
	// 구했을 때 Angle이 똑같이 나온다 따라서 두 경우를 구별하기 위해
	// dest의 y좌표가 src의 y좌표보다 작다면 360 - Angle을 
	// 해줘서 두 경우를 구별해준다
	if (Dest.y < Src.y)
		Angle = 360.f - Angle;
	
	return Angle;
}
