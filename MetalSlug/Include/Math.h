#pragma once

#include <Windows.h>
#include <math.h>

#define PI	3.141592f

float RadianToDegree(float Radian);
float DegreeToRadian(float Degree);

// 좌표 의미하는 구조체
struct Vector2
{
	float x;
	float y;

public:
	Vector2();
	Vector2(float _x, float _y);
	Vector2(const Vector2& v);

public:
	const Vector2& operator = (const Vector2& v)
	{
		x = v.x;
		y = v.y;
		return *this;
	}

	const Vector2& operator = (const POINT& v)
	{
		x = (float)v.x;
		y = (float)v.y;
		return *this;
	}

	// + 연산자 오버로딩
	Vector2 operator + (const Vector2& v)	const
	{
		Vector2 result;
		result.x = x + v.x;
		result.y = y + v.y;
		return result;
	}

	Vector2 operator + (const POINT& v)	const
	{
		Vector2 result;
		result.x = x + (float)v.x;
		result.y = y + (float)v.y;
		return result;
	}

	Vector2 operator + (float f)	const
	{
		Vector2 result;
		result.x = x + f;
		result.y = y + f;
		return result;
	}

	Vector2 operator + (int i)	const
	{
		Vector2	result;
		result.x = x + (float)i;
		result.y = y + (float)i;
		return result;
	}

	// += 연산자 오버로딩
	const Vector2& operator += (const Vector2& v)
	{
		x += v.x;
		y += v.y;

		return *this;
	}

	const Vector2& operator += (const POINT& v)
	{
		x += v.x;
		y += v.y;

		return *this;
	}

	const Vector2& operator += (float f)
	{
		x += f;
		y += f;
		return *this;
	}

	const Vector2& operator += (int i)
	{
		x += i;
		y += i;
		return *this;
	}

	Vector2 operator - (const Vector2& v) const
	{
		Vector2 result;
		result.x = x - v.x;
		result.y = y - v.y;
		return result;
	}

	Vector2 operator - (const POINT& v)	const
	{
		Vector2	result;
		result.x = x - v.x;
		result.y = y - v.y;
		return result;
	}

	Vector2 operator - (float f)	const
	{
		Vector2	result;
		result.x = x - f;
		result.y = y - f;
		return result;
	}

	Vector2 operator - (int i)	const
	{
		Vector2	result;
		result.x = x - i;
		result.y = y - i;
		return result;
	}

	const Vector2& operator -= (const Vector2& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}
	const Vector2& operator -= (const POINT& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	const Vector2& operator -= (float f)
	{
		x -= f;
		y -= f;
		return *this;
	}

	const Vector2& operator -= (int i)
	{
		x -= i;
		y -= i;
		return *this;
	}

	// *
	Vector2 operator * (const Vector2& v)	const
	{
		Vector2	result;
		result.x = x * v.x;
		result.y = y * v.y;
		return result;
	}

	Vector2 operator * (const POINT& v)	const
	{
		Vector2	result;
		result.x = x * v.x;
		result.y = y * v.y;
		return result;
	}

	Vector2 operator * (float f)	const
	{
		Vector2	result;
		result.x = x * f;
		result.y = y * f;
		return result;
	}

	Vector2 operator * (int i)	const
	{
		Vector2	result;
		result.x = x * i;
		result.y = y * i;
		return result;
	}

	// *=
	const Vector2& operator *= (const Vector2& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}

	const Vector2& operator *= (const POINT& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}

	const Vector2& operator *= (float f)
	{
		x *= f;
		y *= f;
		return *this;
	}

	const Vector2& operator *= (int i)
	{
		x *= i;
		y *= i;
		return *this;
	}

	// /
	Vector2 operator / (const Vector2& v)	const
	{
		Vector2	result;
		result.x = x / v.x;
		result.y = y / v.y;
		return result;
	}

	Vector2 operator / (const POINT& v)	const
	{
		Vector2	result;
		result.x = x / v.x;
		result.y = y / v.y;
		return result;
	}

	Vector2 operator / (float f)	const
	{
		Vector2	result;
		result.x = x / f;
		result.y = y / f;
		return result;
	}

	Vector2 operator / (int i)	const
	{
		Vector2	result;
		result.x = x / i;
		result.y = y / i;
		return result;
	}

	// /=
	const Vector2& operator /= (const Vector2& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}

	const Vector2& operator /= (const POINT& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}

	const Vector2& operator /= (float f)
	{
		x /= f;
		y /= f;
		return *this;
	}

	const Vector2& operator /= (int i)
	{
		x /= i;
		y /= i;
		return *this;
	}

	float Length()	const
	{
		return sqrtf(x * x + y * y);
	}

	// 벡터의 크기를 1로 만드는 정규화
	// 만약에 45도 우측 아래 대각선으로 거리 1만큼 총알을 날리고 싶다면
	// 총알의 Vector2 m_Dir은 x = 1, y = 1하고 정규화 시켜줘야
	// x방향으로 1/root(2), y방향으로 1/root(2)만큼 가서
	// 대각선의 길이 1만큼 날아간다. 만약 정규화를 시켜주지 않으면
	// 대각선 길이 root(1^2 + 1^2) = 만큼 날아가서 내가 날리고 싶은 거리보다
	// 실제로 더 많이 날아가게 된다. 따라서 항상 Vector2로 방향 설정을 하고 나서
	// 정규화를 시켜줘야 한다
	void Normalize()
	{
		float SelfLength = Length();
		x /= SelfLength;
		y /= SelfLength;
	}

};

float Distance(const Vector2& Src, const Vector2& Dest);
float GetAngle(const Vector2& Src, const Vector2& Dest);