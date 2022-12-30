#pragma once

#include "runtime/function/framework/event/event.h"

#include <functional>
#include <list>
#include <unordered_map>

namespace Piccolo
{
    class EventManager
    {
    public:
        void AddListener(EventId eventId, std::function<void(Event&)> const& listener) { m_listeners[eventId].push_back(listener); }

        void sendEvent(Event& event)
        {
            uint32_t type = event.getType();

            for (auto const& listener : m_listeners[type])
            {
                listener(event);
            }
        }

        void sendEvent(EventId eventId)
        {
            Event event(eventId);

            for (auto const& listener : m_listeners[eventId])
            {
                listener(event);
            }
        }

    private:
        std::unordered_map<EventId, std::list<std::function<void(Event&)>>> m_listeners;
    };
} // namespace Piccolo
