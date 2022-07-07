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
    std::list<std::shared_ptr<Boid>>  Boid::m_visible_friends = {};
    int                               Flock::s_count          = 0;
    std::list<std::shared_ptr<Flock>> Flock::s_flocks         = {};

    Boid::Boid(int id, std::shared_ptr<Flock> flock, Vector3 min_pos, Vector3 max_pos, MeshComponentRes& mesh) :
        m_id(id), m_min(min_pos), m_max(max_pos), m_mesh(mesh)
    {
        // generate random position
        m_pos = Vector3::ZERO;

        // flip positions for greater randomness

        // generate random velocity in x y plane

        // flip velocities for greater randomness

        // compute speed based on velocity

        // zero out orientation

        // init other values
    }

    Vector3 Boid::cruising(void)
    {

    }

    Vector3 Boid::fleeEnemies(void)
    {

    }

    Vector3 Boid::keepDistance(void)
    {

    }

    Vector3 Boid::matchHeading(void)
    {

    }

    int Boid::seeEnemies(int flock_id)
    {

    }

    int Boid::seeFriends(std::shared_ptr<Boid> first_boid)
    {

    }

    Vector3 Boid::steerToCenter(void)
    {
    }

    void Boid::worldBound(void)
    {
    }

    void Boid::flockIt(int flock_id, std::shared_ptr<Boid> first_boid, float deltaTime)
    {
        Vector3 acc;

        // Step 1:  Update our position.
        // Update our position based on the velocity
        // vector we computed last time around.
        m_oldpos = m_pos;
        m_pos += 0.001 * Vector3::UNIT_Z;

        // Step 2:  SeeFriends.
        // Determine if we can see any of our flockmates.

        // Step 3:  Flocking behavior.
        // Do we see any of our flockmates?  If yes, it's time to implement
        // the first Three Rules (they don't matter if we can't see anybody)

        // Step 4:  Implement Rule #1 (Separation).
        // Try to maintain our desired separation distance from our nearest flockmate.

        // Step 5:  Implement Rule #2 (Alignment).
        // Try to move the same way our nearest flockmate does.

        // Step 6:  Implement Rule #3 (Cohesion).
        // Try to move towards the center of the flock.

        // Step 6:  The Fourth Rule (enemies)
        // If we're supposed to react to enemy flocks, determine
        // if there are any then avoid them if we can.

        // Step 7:  Cruising.
        // Flockmates or not, enemies or not, figure out
        // how fast we'd move if it were all up to us.

        // Step 8:  Constrain acceleration
        // If our acceleration change is more than we allow, constrain it

        // definitely too much...constrain to maximum change
    }

    Flock::Flock()
    {
        m_id = s_count;
        s_count++;
    }

    Flock::~Flock()
    {
        m_id = 0;
        s_count--;
    }

    void Flock::tick(float delta_time)
    {
        for (auto boid : m_boids)
        {
            boid->flockIt(m_id, m_boids.front(), delta_time);
        }
    }

    void Flock::addTo(std::shared_ptr<Boid> boid) { m_boids.push_front(boid); }

    void Flock::removeFrom(std::shared_ptr<Boid> boid) { m_boids.remove(boid); }

    void FlockComponent::postLoadResource(std::weak_ptr<GObject> parent_object)
    {
        for (int i = 0; i < m_flock_res.m_count; ++i)
        {
            for (auto sub_mesh : m_mesh_res.m_sub_meshes)
            {
                SubMeshRes mesh;
                mesh.m_material     = sub_mesh.m_material;
                mesh.m_obj_file_ref = sub_mesh.m_obj_file_ref;
                mesh.m_transform    = sub_mesh.m_transform;
                m_mesh_instance.m_sub_meshes.push_back(mesh);
            }

            m_parent_object = parent_object;

            std::shared_ptr<AssetManager> asset_manager = g_runtime_global_context.m_asset_manager;
            ASSERT(asset_manager);

            m_raw_meshes.resize(m_mesh_instance.m_sub_meshes.size());

            size_t raw_mesh_count = 0;
            for (const SubMeshRes& sub_mesh : m_mesh_instance.m_sub_meshes)
            {
                GameObjectPartDesc& meshComponent = m_raw_meshes[raw_mesh_count];
                meshComponent.m_mesh_desc.m_mesh_file =
                    asset_manager->getFullPath(sub_mesh.m_obj_file_ref).generic_string();

                meshComponent.m_material_desc.m_with_texture = sub_mesh.m_material.empty() == false;

                if (meshComponent.m_material_desc.m_with_texture)
                {
                    MaterialRes material_res;
                    asset_manager->loadAsset(sub_mesh.m_material, material_res);

                    meshComponent.m_material_desc.m_base_color_texture_file =
                        asset_manager->getFullPath(material_res.m_base_colour_texture_file).generic_string();
                    meshComponent.m_material_desc.m_metallic_roughness_texture_file =
                        asset_manager->getFullPath(material_res.m_metallic_roughness_texture_file).generic_string();
                    meshComponent.m_material_desc.m_normal_texture_file =
                        asset_manager->getFullPath(material_res.m_normal_texture_file).generic_string();
                    meshComponent.m_material_desc.m_occlusion_texture_file =
                        asset_manager->getFullPath(material_res.m_occlusion_texture_file).generic_string();
                    meshComponent.m_material_desc.m_emissive_texture_file =
                        asset_manager->getFullPath(material_res.m_emissive_texture_file).generic_string();
                }

                auto object_space_transform = sub_mesh.m_transform.getMatrix();

                meshComponent.m_transform_desc.m_transform_matrix = object_space_transform;

                ++raw_mesh_count;
            }
        }
    }

    void FlockComponent::tick(float delta_time)
    {
        for (auto flock : m_flocks)
        {
            flock->tick(delta_time);
        }
    }
} // namespace Piccolo