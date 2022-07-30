#include "runtime/function/audio/audio_system.h"

#include "runtime/core/base/macro.h"

// #if defined(__GNUC__) && defined(__MACH__)
// #define MA_NO_RUNTIME_LINKING
// #endif
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

namespace Piccolo
{
    void AudioSystem::initialize()
    {
        ma_result result = ma_engine_init(NULL, &m_engine);
        if (result != MA_SUCCESS)
        {
            LOG_ERROR("Failed to initialize audio engine.");
            exit(1);
        }
        else
        {
            LOG_INFO("initialize audio engine.");
        }
    }

    void AudioSystem::update()
    {
    }

    void AudioSystem::finalize()
    {
        LOG_INFO("finalize audio engine.");
        ma_engine_uninit(&m_engine);
    }

    void AudioSystem::dataCallback(ma_device* device, void* output, const void* input, ma_uint32 frameCount) {}
} // namespace Piccolo
