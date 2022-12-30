#pragma once
#include "runtime/function/framework/ecs/types.h"

#include <any>
#include <unordered_map>

namespace Piccolo
{
    // TODO: Make these easier to define and use (macro?)
    // TODO: Add some kind of enforcement/automation that a SetParam type and a GetParam type match

    namespace Events::Window
    {
        const EventId QUIT    = "Events::Window::QUIT"_hash;
        const EventId RESIZED = "Events::Window::RESIZED"_hash;
        const EventId INPUT   = "Events::Window::INPUT"_hash;
    } // namespace Events::Window

    namespace Events::Window::Input
    {
        const ParamId INPUT = "Events::Window::Input::INPUT"_hash;
    }

    namespace Events::Window::Resized
    {
        const ParamId WIDTH  = "Events::Window::Resized::WIDTH"_hash;
        const ParamId HEIGHT = "Events::Window::Resized::HEIGHT"_hash;
    } // namespace Events::Window::Resized

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