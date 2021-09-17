#pragma once

#include "../Ref.h"

class CSound :
    public CRef
{
    friend class CResourceManager;

private:
    CSound();
    ~CSound();

private:
    std::string m_Name;
    // Sound들을 총괄하는 객체
    FMOD::System* m_System;
    FMOD::Sound* m_Sound;
    // sound가 속한 group(ex. BGM, UI, Effect...)
    FMOD::ChannelGroup* m_Group;
    // Sound Load가 완료됐으면 m_Channel에서 재생,
    // 지금 재생중인지 멈춰있는지 확인,
    // Pause 되어 있는지 확인을 확인할 수 있다
    FMOD::Channel* m_Channel;
    // 지금 sound가 재생중인지 아닌지
    bool m_Play;
    bool m_Loop;

public:
    bool IsPlay()	const
    {
        return m_Play;
    }

    bool IsLoop()	const
    {
        return m_Loop;
    }

public:
    bool LoadSound(FMOD::System* System, FMOD::ChannelGroup* Group,
        bool Loop, const std::string& Name,
        const char* FileName,
        const std::string& PathName = SOUND_PATH);

    void Play();
    void Stop();
    void Pause();
    void Resume();
};

