#include "runtime/function/framework/component/particle/flock_component.h"

#include "runtime/resource/asset_manager/asset_manager.h"
#include "runtime/resource/res_type/data/material.h"

#include "runtime/function/framework/component/transform/transform_component.h"
#include "runtime/function/framework/object/object.h"
#include "runtime/function/global/global_context.h"

#include "runtime/function/render/render_swap_context.h"
#include "runtime/function/render/render_system.h"

namespace Piccolo
{
    void FlockComponent::postLoadResource(std::weak_ptr<GObject> parent_object)
    {
        // TODO : generate more mesh or polygon for particles
    }

    void FlockComponent::tick(float delta_time)
    {
        for (auto flock : m_flocks)
        {
            flock->tick(delta_time);
        }
    }
} // namespace Piccolo