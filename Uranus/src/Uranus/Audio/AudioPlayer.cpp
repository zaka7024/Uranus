#include "urpch.h"
#include "AudioPlayer.h"

namespace Uranus {

    static irrklang::ISoundEngine* engine;

    void AudioPlayer::Init()
    {
        // start up the engine
        engine = irrklang::createIrrKlangDevice();
    }

    void AudioPlayer::Play(const AudioSourceComponent& as)
    {
        if(!as.SoundFile.empty())
            engine->play2D(as.SoundFile.string().c_str(), true);
    }
}