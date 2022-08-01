#pragma once

#include <miniaudio.h>

#include <cstdint>

namespace Piccolo
{
    enum class AudioState : uint32_t
    {
        None = 0,
        Initialize,
        ToPlay,
        Loading,
        Devirtualize,
        Playing,
        Stopping,
        Stopped,
        Virtualizing,
        Virtual,
    };

    class AudioFSM
    {
    public:
        void       setState(AudioState state) { m_currentState = state; }
        AudioState getState() const { return m_currentState; }

        void update();

    private:
        AudioState m_currentState = None;
    };
} // namespace Piccolo
