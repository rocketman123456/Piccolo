#pragma once

#include "runtime/core/meta/reflection/reflection.h"

namespace Piccolo
{
    REFLECTION_TYPE(AudioComponentRes)
    CLASS(AudioComponentRes, Fields)
    {
        REFLECTION_BODY(AudioComponentRes);

    public:
        AudioComponentRes() = default;
        ~AudioComponentRes() = default;

    public:
        std::string m_audio_url;
    };
}
