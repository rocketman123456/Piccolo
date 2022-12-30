#pragma once

#include "runtime/function/framework/ecs/types.h"

#include <array>
#include <cassert>
#include <queue>

namespace Piccolo
{
    class EntityManager
    {
    public:
        EntityManager()
        {
            for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
            {
                m_available_entities.push(entity);
            }
        }

        Entity createEntity()
        {
            assert(m_living_entity_count < MAX_ENTITIES && "Too many entities in existence.");

            Entity id = m_available_entities.front();
            m_available_entities.pop();
            ++m_living_entity_count;

            return id;
        }

        void destroyEntity(Entity entity)
        {
            assert(entity < MAX_ENTITIES && "Entity out of range.");

            m_signatures[entity].reset();
            m_available_entities.push(entity);
            --m_living_entity_count;
        }

        void setSignature(Entity entity, Signature signature)
        {
            assert(entity < MAX_ENTITIES && "Entity out of range.");

            m_signatures[entity] = signature;
        }

        Signature getSignature(Entity entity)
        {
            assert(entity < MAX_ENTITIES && "Entity out of range.");

            return m_signatures[entity];
        }

    private:
        std::queue<Entity>                  m_available_entities {};
        std::array<Signature, MAX_ENTITIES> m_signatures {};
        uint32_t                            m_living_entity_count {};
    };
} // namespace Piccolo
