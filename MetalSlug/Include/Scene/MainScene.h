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
	// MainScene에서 각종 object들이 쓸 AnimationSequence들을
	// 미리 로드 해놓고 Object들이 나중에 필요할 때
	// 찾아서 쓰도록 한다
	void LoadAnimationSequence();
	// 하늘, 사막 풍경 등 Background만드는 함수
	void LoadBackground();
	void LoadSound();
	void LoadPlayerAnimationSequence();
	void LoadMonsterAnimationSequence();
	void LoadNPCAnimationSequence();
	// 밟고 올라갈 수 있는 지형지물을 Load하는 함수
	void LoadObstacle();
	
	void CreateArabian();
	// 맨 처음 Gen충돌체(Front&Back Obstacle 직전에 있는)랑 만났을 때
	// 호출되는 함수
	void GenPhase1();

	void GenPhase2();
};

