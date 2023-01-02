#include "runtime/core/math/vector_util.h"

namespace Piccolo
{
    FVector2 reflect(const FVector2& point, const FVector2& normal) { return point - (2 * point.dot(normal) * normal); }

    FVector2 lerp(const FVector2& lhs, const FVector2& rhs, float alpha) { return lhs + alpha * (rhs - lhs); }

    FVector2 mid_point(const FVector2& v1, const FVector2& v2) { return v1 + 0.5 * v2; }
} // namespace Piccolo
