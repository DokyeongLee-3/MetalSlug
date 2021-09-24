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
	// MainScene에서 각종 object들이 쓸 AnimationSequence들을
	// 미리 로드 해놓고 Object들이 나중에 필요할 때
	// 찾아서 쓰도록 한다
	void LoadAnimationSequence();
	// 하늘, 사막 풍경 등 Background만드는 함수
	void LoadBackground();
};

