#pragma once

#include "runtime/function/framework/ecs/system.h"

#include <cassert>
#include <memory>
#include <unordered_map>

namespace Piccolo
{
    class SystemManager
    {
    public:
        template<typename T>
        std::shared_ptr<T> registerSystem()
        {
            const char* type_name = typeid(T).name();

            assert(m_systems.find(type_name) == m_systems.end() && "Registering system more than once.");

            auto system = std::make_shared<T>();
            m_systems.insert({type_name, system});
            return system;
        }

        template<typename T>
        void setSignature(Signature signature)
        {
            const char* type_name = typeid(T).name();

            assert(m_systems.find(type_name) != m_systems.end() && "System used before registered.");

            m_signatures.insert({type_name, signature});
        }

        void entityDestroyed(Entity entity)
        {
            for (auto const& pair : m_systems)
            {
                auto const& system = pair.second;
                system->m_entities.erase(entity);
            }
        }

        void entitySignatureChanged(Entity entity, Signature entitySignature)
        {
            for (auto const& pair : m_systems)
            {
                auto const& type            = pair.first;
                auto const& system          = pair.second;
                auto const& systemSignature = m_signatures[type];

                if ((entitySignature & systemSignature) == systemSignature)
                    system->m_entities.insert(entity);
                else
                    system->m_entities.erase(entity);
            }
        }

    private:
        std::unordered_map<const char*, Signature>               m_signatures {};
        std::unordered_map<const char*, std::shared_ptr<System>> m_systems {};
    };
} // namespace Piccolo
