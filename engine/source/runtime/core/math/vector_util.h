#pragma once

#include "runtime/core/math/math.h"
#include "runtime/core/math/quaternion.h"
#include "runtime/core/math/vector2.h"
#include "runtime/core/math/vector3.h"
#include "runtime/core/math/vector4.h"

namespace Piccolo
{
    FVector2 reflect(const FVector2& point, const FVector2& normal);

    FVector2 lerp(const FVector2& lhs, const FVector2& rhs, float alpha);

    FVector2 mid_point(const FVector2& v1, const FVector2& v2);
} // namespace Piccolo
