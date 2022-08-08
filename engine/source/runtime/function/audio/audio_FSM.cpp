#include "runtime/function/audio/audio_fsm.h"

namespace Piccolo
{
    void AudioFSM::update()
    {
        switch (m_currentState)
        {
            case AudioState::None: {
            }
            break;
            case AudioState::Initialize: {
            }
            break;
            case AudioState::ToPlay: {
            }
            break;
            case AudioState::Loading: {
            }
            break;
            case AudioState::Devirtualize: {
            }
            break;
            case AudioState::Playing: {
            }
            break;
            case AudioState::Stopping: {
            }
            break;
            case AudioState::Stopped: {
            }
            break;
            case AudioState::Virtualizing: {
            }
            break;
            case AudioState::Virtual: {
            }
            break;
        }
    }
} // namespace Piccolo
