#pragma once

#include <miniaudio.h>

#include <unordered_map>
#include <list>
#include <cstdint>

namespace Piccolo
{
    class AudioSystem
    {
    public:
        AudioSystem() = default;
        ~AudioSystem() = default;

        void initialize();
        void update();
        void finalize();

        void dataCallback(ma_device* device, void* output, const void* input, ma_uint32 frameCount);

    private:
        std::unordered_map<uint32_t, uint32_t> m_audio_map;

        ma_decoder_config m_decoder_config;
        ma_uint32 m_decoder_count;
        ma_device_config m_device_config;
        ma_device m_device;
        ma_engine m_engine;
    };
}
