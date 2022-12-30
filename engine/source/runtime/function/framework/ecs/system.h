#pragma once
#include "runtime/function/framework/ecs/types.h"

#include <set>

namespace Piccolo
{
    class System
    {
    public:
        std::set<Entity> m_entities;
    };
} // namespace Piccolo
