#pragma once
#include "runtime/core/math/math_defines.h"
#include "runtime/core/math/matrix4.h"
#include "runtime/core/math/quaternion.h"
#include "runtime/core/math/vector3.h"
#include "runtime/core/meta/reflection/reflection.h"

namespace Piccolo
{
    REFLECTION_TYPE(Transform)
    CLASS(Transform, Fields)
    {
        REFLECTION_BODY(Transform);

    public:
        Vector3    m_position {Vector3::ZERO};
        Vector3    m_scale {Vector3::UNIT_SCALE};
        Quaternion m_rotation {Quaternion::IDENTITY};

        Transform() = default;
        Transform(const Vector3& position, const Quaternion& rotation, const Vector3& scale) : m_position {position}, m_scale {scale}, m_rotation {rotation} {}

        FTransform3 getMatrix() const;
    };
} // namespace Piccolo
