#pragma once
#include "runtime/function/framework/ecs/types.h"

#include <any>
#include <unordered_map>

namespace Piccolo
{

    class Event
    {
    public:
        Event() = delete;

        explicit Event(EventId type) : m_type(type) {}

        template<typename T>
        void setParam(EventId id, T value)
        {
            m_data[id] = value;
        }

        template<typename T>
        T getParam(EventId id)
        {
            return std::any_cast<T>(m_data[id]);
        }

        EventId getType() const { return m_type; }

    private:
        EventId                               m_type {};
        std::unordered_map<EventId, std::any> m_data {};
    };

} // namespace Piccolo