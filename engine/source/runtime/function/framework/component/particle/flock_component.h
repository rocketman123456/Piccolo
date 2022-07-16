#pragma once
#include "runtime/function/framework/component/component.h"
#include "runtime/function/render/render_object.h"
#include "runtime/resource/res_type/components/flock.h"
#include "runtime/resource/res_type/components/mesh.h"
#include "runtime/function/framework/component/particle/flock_particle.h"

#include <list>
#include <vector>

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

        void tick(float delta_time) override;

    private:
        META(Enable)
        BasicFlockRes m_flock_res;

        // TODO : find mesh component in same go, and update mesh transform
        // META(Enable)
        // MeshComponentRes m_mesh_res;

        // META(Enable)
        // MeshComponentRes m_mesh_instance;

        std::vector<GameObjectPartDesc>     m_raw_meshes;
        std::vector<std::shared_ptr<Boid>>  m_boids;
        std::vector<std::shared_ptr<Flock>> m_flocks;
    };
} // namespace Piccolo
