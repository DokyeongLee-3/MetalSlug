
#include "Arabian.h"
#include "../Scene/Scene.h"
#include "../Collision/ColliderBox.h"

CArabian::CArabian()
{
}

CArabian::CArabian(const CArabian& obj)	:
	CMonster(obj)
{
}

CArabian::~CArabian()
{
}

void CArabian::Start()
{
	CMonster::Start();
}

bool CArabian::Init()
{
	if(!CMonster::Init())
		return false;
}

void CArabian::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);
}

void CArabian::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

void CArabian::Collision(float DeltaTime)
{
	CMonster::Collision(DeltaTime);
}

void CArabian::Render(HDC hDC)
{
	CMonster::Render(hDC);
}

CArabian* CArabian::Clone()
{
	return new CArabian(*this);
}
