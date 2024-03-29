
#include "CollisionManager.h"

CCollisionManager* CCollisionManager::m_Inst = nullptr;

CCollisionManager::CCollisionManager()
{
}

CCollisionManager::~CCollisionManager()
{
	auto iter = m_mapProfile.begin();
	auto iterEnd = m_mapProfile.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	m_mapProfile.clear();

}

bool CCollisionManager::Init()
{
	CreateProfile("Default", Channel_Object, true);
	CreateProfile("Player", Channel_Player, true);
	CreateProfile("Monster", Channel_Monster, true);
	CreateProfile("PlayerAttack", Channel_PlayerAttack, true);
	CreateProfile("MonsterAttack", Channel_MonsterAttack, true);
	CreateProfile("NPC", Channel_NPC, true);
	CreateProfile("Obstacle", Channel_Obstacle, true);
	CreateProfile("GenTrigger", Channel_GenTrigger, true);

	// 플레이어 충돌체끼리나 플레이어의 총알과
	// 플레이어가 충돌하면 안된다.
	SetCollisionState("Player", Channel_Player, ECollision_State::Ignore);
	SetCollisionState("Player", Channel_PlayerAttack, ECollision_State::Ignore);

	SetCollisionState("Monster", Channel_Monster, ECollision_State::Ignore);
	SetCollisionState("Monster", Channel_MonsterAttack, ECollision_State::Ignore);

	// 플레이어의 총알은 플레이어와, 플레이어의 총알,
	// 몬스터의 총알과 충돌하지 않도록 한다
	SetCollisionState("PlayerAttack", Channel_Player, ECollision_State::Ignore);
	SetCollisionState("PlayerAttack", Channel_PlayerAttack, ECollision_State::Ignore);
	SetCollisionState("PlayerAttack", Channel_MonsterAttack, ECollision_State::Ignore);
	SetCollisionState("PlayerAttack", Channel_Obstacle, ECollision_State::Ignore);

	SetCollisionState("MonsterAttack", Channel_Monster, ECollision_State::Ignore);
	SetCollisionState("MonsterAttack", Channel_PlayerAttack, ECollision_State::Ignore);
	SetCollisionState("MonsterAttack", Channel_MonsterAttack, ECollision_State::Ignore);
	SetCollisionState("MonsterAttack", Channel_Obstacle, ECollision_State::Ignore);

	SetCollisionState("Obstacle", Channel_Obstacle, ECollision_State::Ignore);
	SetCollisionState("Obstacle", Channel_PlayerAttack, ECollision_State::Ignore);
	SetCollisionState("Obstacle", Channel_MonsterAttack, ECollision_State::Ignore);

	SetCollisionState("GenTrigger", Channel_GenTrigger, ECollision_State::Ignore);
	SetCollisionState("GenTrigger", Channel_PlayerAttack, ECollision_State::Ignore);
	SetCollisionState("GenTrigger", Channel_MonsterAttack, ECollision_State::Ignore);
	SetCollisionState("GenTrigger", Channel_Monster, ECollision_State::Ignore);
	SetCollisionState("GenTrigger", Channel_Obstacle, ECollision_State::Ignore);

	SetCollisionState("NPC", Channel_NPC, ECollision_State::Ignore);
	SetCollisionState("NPC", Channel_MonsterAttack, ECollision_State::Ignore);
	SetCollisionState("NPC", Channel_Player, ECollision_State::Ignore);
	SetCollisionState("NPC", Channel_Monster, ECollision_State::Ignore);

	return true;
}

bool CCollisionManager::CreateProfile(
	const std::string& Name,
	ECollision_Channel Channel, bool Enable,
	ECollision_State State)
{
	CollisionProfile* Profile = FindProfile(Name);

	if (Profile)
		return false;

	Profile = new CollisionProfile;

	Profile->Name = Name;
	Profile->Channel = Channel;
	Profile->CollisionEnable = Enable;
	Profile->vecState.resize(Channel_Max);

	for (int i = 0; i < Channel_Max; ++i)
	{
		Profile->vecState[i] = State;
	}

	m_mapProfile.insert(std::make_pair(Name, Profile));

	return false;
}

bool CCollisionManager::SetCollisionState(
	const std::string& Name, ECollision_Channel Channel,
	ECollision_State State)
{
	CollisionProfile* Profile = FindProfile(Name);

	if (!Profile)
		return false;

	Profile->vecState[Channel] = State;

	return true;
}

CollisionProfile* CCollisionManager::FindProfile(const std::string& Name)
{
	auto iter = m_mapProfile.find(Name);

	if (iter == m_mapProfile.end())
		return nullptr;

	return iter->second;
}
