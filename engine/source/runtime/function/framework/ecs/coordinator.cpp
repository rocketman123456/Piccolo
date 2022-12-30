#include "runtime/function/framework/ecs/coordinator.h"

namespace Piccolo
{
    void Coordinator::init()
    {
        m_component_manager = std::make_unique<ComponentManager>();
        m_entity_manager    = std::make_unique<EntityManager>();
        m_system_manager    = std::make_unique<SystemManager>();
    }

    Entity Coordinator::createEntity() { return m_entity_manager->createEntity(); }

    void Coordinator::destroyEntity(Entity entity)
    {
        m_entity_manager->destroyEntity(entity);
        m_component_manager->entityDestroyed(entity);
        m_system_manager->entityDestroyed(entity);
    }
} // namespace Piccolo