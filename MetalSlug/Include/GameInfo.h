#pragma once


#include <Windows.h>
#include "resource.h" 

#include <list>
#include <vector>
#include <stack>
#include <functional>
#include <unordered_map>
#include <crtdbg.h>
#include <string>

#include "SharedPtr.h"
#include "Flag.h"
#include "Math.h"
#include "fmod.hpp"

#pragma comment(lib, "msimg32.lib")

#pragma comment(lib, "fmod64_vc.lib")

#define KEYCOUNT_MAX	256

#define ROOT_PATH		"RootPath"
#define TEXTURE_PATH	"TexturePath"
#define SOUND_PATH		"SoundPath"


#define SAFE_DELETE(p)	if(p)	{ delete p; p = nullptr; }
#define SAFE_DELETE_ARRAY(p)	if(p) {delete[] p; p = nullptr;}
#define SAFE_RELEASE(p)	if(p)	{p->Release(); p = nullptr;}


#define WORLD_WIDTH	13300.f
#define WORLD_HEIGHT 960.f

#define GRAVITY 9.8f

#define PLAYER_TOPWIDTH		100
#define PLAYER_TOPHEIGHT	78
#define PLAYER_BOTTOMWIDTH	70
#define PLAYER_BOTTOMHEIGHT	53
#define ARABIAN_WIDTH 130
#define ARABIAN_HEIGHT 113
#define CAMEL_SIZE 150

#define STAGE_WIDTH 13300.f
#define STAGE_HEIGHT 960.f


struct Resolution
{
	int Width;
	int Height;
};

struct RectInfo
{
	float Left;
	float Top;
	float Right;
	float Bottom;
};

struct SphereInfo
{
	Vector2 Center;
	float Radius;
};

struct BulletInfo
{
	RectInfo	rc;
	float		Distance;
};

struct AnimationFrameData
{
	Vector2 StartPos;
	Vector2 Size;
	Vector2 Offset;

	AnimationFrameData() :
		StartPos(0.f, 0.f),
		Offset(0.f, 0.f)
	{
	}
};

struct CharacterInfo
{
	int Attack;
	int Armor;
	int HP;
	int HPMax;
	int MP;
	int MPMax;
	int Level;
	int Exp;
	int Gold;
	float AttackSpeed;
	float AttackDistance;
};

struct CollisionProfile
{
	std::string			Name;
	ECollision_Channel	Channel;
	// 이걸 False로 만들면 이 채널의 충돌체들은
	// 모두 비활성화
	bool				CollisionEnable;

	// Channel 수만큼 벡터에 Collision_State를 넣고
	// 충돌을 원하는 채널만 Collision으로 설정하고
	// 나머지는 Ignore로 설정
	// 예를 들어서 이 CollsionProfile을 갖는 Collider들은
	// Player의 총알만 충돌하고 나머지는 전부 충돌하고
	// 싶지 않다면 vector에 4번 인덱스에만 Collision을 넣고
	// 나머지는 Ignore을 넣으면 된다.
	std::vector<ECollision_State> vecState;
};

// bmp는 저장할 때 RGB가 아닌 BGR순으로 저장되어 있는걸 잊지말자!!
typedef struct _tagPixel
{
	unsigned char r;
	unsigned char g;
	unsigned char b;

}PIXEL, * PPIXEL;
