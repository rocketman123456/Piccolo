#pragma once
#include "runtime/function/framework/component/component.h"
#include "runtime/resource/res_type/components/flock.h"
#include "runtime/function/render/render_object.h"

namespace Piccolo
{
    REFLECTION_TYPE(FlockComponent)
    CLASS(FlockComponent : public Component, WhiteListFields)
    {
        REFLECTION_BODY(FlockComponent)
    public:
        FlockComponent() {};
        virtual ~FlockComponent() {};

        void postLoadResource(std::weak_ptr<GObject> parent_object) override;

        const std::vector<GameObjectPartDesc>& getRawMeshes() const { return m_raw_meshes; }

        void tick(float delta_time) override;

    private:
        META(Enable)
        BasicFlockRes m_flock_res;

        std::vector<GameObjectPartDesc> m_raw_meshes;
    };
}
