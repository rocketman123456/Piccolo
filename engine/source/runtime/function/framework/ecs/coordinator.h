#pragma once

#include "runtime/function/framework/ecs/component_manager.h"
#include "runtime/function/framework/ecs/entity_manager.h"
#include "runtime/function/framework/ecs/event_manager.h"
#include "runtime/function/framework/ecs/system_manager.h"

#include <memory>

namespace Piccolo
{
    class Coordinator
    {
    public:
        void init()
        {
            m_component_manager = std::make_unique<ComponentManager>();
            m_entity_manager    = std::make_unique<EntityManager>();
            m_event_manager     = std::make_unique<EventManager>();
            m_system_manager    = std::make_unique<SystemManager>();
        }

        // Entity methods
        Entity createEntity() { return m_entity_manager->createEntity(); }

        void destroyEntity(Entity entity)
        {
            m_entity_manager->destroyEntity(entity);
            m_component_manager->entityDestroyed(entity);
            m_system_manager->entityDestroyed(entity);
        }

        // Component methods
        template<typename T>
        void registerComponent()
        {
            m_component_manager->RegisterComponent<T>();
        }

        template<typename T>
        void addComponent(Entity entity, T component)
        {
            m_component_manager->AddComponent<T>(entity, component);

            auto signature = m_entity_manager->getSignature(entity);
            signature.set(m_component_manager->GetComponentType<T>(), true);
            m_entity_manager->setSignature(entity, signature);

            m_system_manager->entitySignatureChanged(entity, signature);
        }

        template<typename T>
        void removeComponent(Entity entity)
        {
            m_component_manager->RemoveComponent<T>(entity);

            auto signature = m_entity_manager->getSignature(entity);
            signature.set(m_component_manager->GetComponentType<T>(), false);
            m_entity_manager->setSignature(entity, signature);

            m_system_manager->entitySignatureChanged(entity, signature);
        }

        template<typename T>
        T& getComponent(Entity entity)
        {
            return m_component_manager->GetComponent<T>(entity);
        }

        template<typename T>
        ComponentType getComponentType()
        {
            return m_component_manager->GetComponentType<T>();
        }

        // System methods
        template<typename T>
        std::shared_ptr<T> registerSystem()
        {
            return m_system_manager->RegisterSystem<T>();
        }

        template<typename T>
        void setSystemSignature(Signature signature)
        {
            m_system_manager->SetSignature<T>(signature);
        }

        // // Event methods
        // void addEventListener(EventId eventId, std::function<void(Event&)> const& listener) { mEventManager->AddListener(eventId, listener); }
        // void sendEvent(Event& event) { mEventManager->sendEvent(event); }
        // void sendEvent(EventId eventId) { mEventManager->sendEvent(eventId); }

    private:
        std::unique_ptr<ComponentManager> m_component_manager;
        std::unique_ptr<EntityManager>    m_entity_manager;
        std::unique_ptr<EventManager>     m_event_manager;
        std::unique_ptr<SystemManager>    m_system_manager;
    };
} // namespace Piccolo
