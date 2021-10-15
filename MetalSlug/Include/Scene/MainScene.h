#pragma once

#include "Scene.h"

class CMainScene :
    public CScene
{
	friend class CSceneManager;
	friend class CPlayer;

private:
	CMainScene();
	virtual ~CMainScene();

public:
	virtual bool Init();

private:
	// MainScene���� ���� object���� �� AnimationSequence����
	// �̸� �ε� �س��� Object���� ���߿� �ʿ��� ��
	// ã�Ƽ� ������ �Ѵ�
	void LoadAnimationSequence();
	// �ϴ�, �縷 ǳ�� �� Background����� �Լ�
	void LoadBackground();
	void LoadSound();
	void LoadPlayerAnimationSequence();
	void LoadMonsterAnimationSequence();
	void LoadNPCAnimationSequence();
	// ��� �ö� �� �ִ� ���������� Load�ϴ� �Լ�
	void LoadObstacle();
	
	void CreateArabian();
	// �� ó�� Gen�浹ü(Front&Back Obstacle ������ �ִ�)�� ������ ��
	// ȣ��Ǵ� �Լ�
	void GenPhase1();

	void GenPhase2();
};

