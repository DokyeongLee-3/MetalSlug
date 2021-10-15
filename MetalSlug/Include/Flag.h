#pragma once


enum EKeyState
{
	KeyState_Down,
	KeyState_Push,
	KeyState_Up,
	KeyState_Max
};

enum class ETexture_Type
{
	Atlas,
	Frame
};

enum class ECollider_Type
{
	Box,
	Sphere,
	Pixel,
	Point
};

enum ECollision_Channel
{
	Channel_Object,
	Channel_Player,
	Channel_Monster,
	Channel_PlayerAttack,
	Channel_MonsterAttack,
	Channel_NPC,
	Channel_Obstacle,
	Channel_GenTrigger,
	Channel_Max
};

enum class ECollision_State
{
	Ignore,
	Collision
};

enum class EButton_State
{
	Normal,
	MouseOn,
	Click,
	Disable,
	End
};

enum class EObject_Type
{
	GameObject,
	Player,
	Monster,
	Bullet,
	Effect,
	Obstacle,
	Stage
};

enum class EArabian_State
{
	Shuffle,
	Trace,
	Attack,
	Jump
};

enum class ECamelArabian_State
{
	Idle,
	Run,
	Attack
};

enum class ENPC_State
{
	Idle,
	Escape,
	Walk,
	Give,
	Salute,
	Run
};