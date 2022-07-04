#pragma once
#include "runtime/core/math/transform.h"
#include "runtime/core/meta/reflection/reflection.h"

namespace Piccolo
{
    REFLECTION_TYPE(BasicFlockRes)
    CLASS(BasicFlockRes, Fields)
    {
        REFLECTION_BODY(BasicFlockRes);

    public:
        Transform   m_transform;
        std::string m_name;
        Vector3     m_min_pos;
        Vector3     m_max_pos;
        int         m_count;
    };
} // namespace Piccolo