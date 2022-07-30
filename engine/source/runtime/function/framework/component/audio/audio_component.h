#pragma once

#include "runtime/function/framework/component/component.h"

#include "runtime/resource/res_type/components/audio.h"

#include <vector>

namespace Piccolo
{
    REFLECTION_TYPE(AudioComponent)
    CLASS(AudioComponent : public Component, WhiteListFields)
    {
    public:
        AudioComponent() {};
        virtual ~AudioComponent() {};

        void postLoadResource(std::weak_ptr<GObject> parent_object) override;

        void tick(float delta_time) override;

    private:
        std::vector<AudioComponentRes> m_audios;
    };
}
