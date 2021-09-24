#pragma once

#include "Scene.h"

class CMainScene :
    public CScene
{
	friend class CSceneManager;

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
};

