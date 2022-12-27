#include "runtime/function/controller/character_controller.h"

#include "runtime/core/base/macro.h"

#include "runtime/function/framework/component/motor/motor_component.h"
#include "runtime/function/framework/world/world_manager.h"
#include "runtime/function/global/global_context.h"
#include "runtime/function/physics/physics_scene.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsScene.h>
#include <Jolt/Physics/Character/CharacterBase.h>
#include <Jolt/Physics/Character/Character.h>
#include <Jolt/Physics/Character/CharacterVirtual.h>

namespace Piccolo
{
    CharacterController::CharacterController(const Capsule& capsule) : m_capsule(capsule)
    {
        m_rigidbody_shape                                    = RigidBodyShape();
        m_rigidbody_shape.m_geometry                         = PICCOLO_REFLECTION_NEW(Capsule);
        *static_cast<Capsule*>(m_rigidbody_shape.m_geometry) = m_capsule;

        m_rigidbody_shape.m_type = RigidBodyShapeType::capsule;

        Quaternion orientation;
        orientation.fromAngleAxis(Radian(Degree(90.f)), Vector3::UNIT_X);

        m_rigidbody_shape.m_local_transform = Transform(Vector3(0, 0, capsule.m_half_height + capsule.m_radius), orientation, Vector3::UNIT_SCALE);

        std::shared_ptr<PhysicsScene> physics_scene = g_runtime_global_context.m_world_manager->getCurrentActivePhysicsScene().lock();
        ASSERT(physics_scene);
        m_character = physics_scene->createCharacter();
    }

    CharacterController::~CharacterController()
    {
        if (m_character != nullptr)
        {
            delete m_character;
            m_character = nullptr;
        }
    }

    Vector3 CharacterController::move(const Vector3& current_position, const Vector3& displacement)
    {
        std::shared_ptr<PhysicsScene> physics_scene = g_runtime_global_context.m_world_manager->getCurrentActivePhysicsScene().lock();
        ASSERT(physics_scene);

        if(displacement.length() > 0.01)
        {
            Transform transform = m_rigidbody_shape.m_global_transform;
        }

        Transform transform = m_rigidbody_shape.m_global_transform;

        auto position = transform.m_position;
        auto rotation = transform.m_rotation;
        auto velocity = displacement * 100;

        JPH::Vec3 post_position = m_character->GetPosition();
        m_character->SetPositionAndRotation(JPH::Vec3(position.x, position.y, position.z), JPH::Quat(rotation.w, rotation.x, rotation.y, rotation.z));
        m_character->SetLinearVelocity(JPH::Vec3(velocity.x, velocity.y, velocity.z));
        m_character->PostSimulation(0.05f);

        Vector3 final_position = current_position + displacement;

        Transform final_transform = Transform(final_position, Quaternion::IDENTITY, Vector3::UNIT_SCALE);

        if (physics_scene->isOverlap(m_rigidbody_shape, final_transform.getMatrix()))
        {
            final_position = current_position;
        }

        return final_position;
    }
} // namespace Piccolo
