#pragma once

#include "runtime/core/math/math.h"
#include "runtime/core/math/quaternion.h"
#include "runtime/core/math/vector2.h"
#include "runtime/core/math/vector3.h"
#include "runtime/core/math/vector4.h"

namespace Piccolo
{
    // ---------------------------------------------------------------------
    // ---------------------------------------------------------------------
    // ---------------------------------------------------------------------

    FVector2 reflect(const FVector2& point, const FVector2& normal);

    FVector2 lerp(const FVector2& lhs, const FVector2& rhs, float alpha);

    FVector2 midPoint(const FVector2& v1, const FVector2& v2);

    void makeFloor(FVector2& origin, const FVector2& cmp);

    void makeCeil(FVector2& origin, const Vector2& cmp);

    FVector2 perpendicular(const FVector2& origin);

    bool isNearZero(const FVector2& origin);

    bool isNaN(const FVector2& origin);

    float distance(const FVector2& a, const FVector2& b);

    float squaredDistance(const FVector2& a, const FVector2& b);

    // ---------------------------------------------------------------------
    // ---------------------------------------------------------------------
    // ---------------------------------------------------------------------

    FVector3 reflect(const FVector3& point, const FVector3& normal);

    FVector3 lerp(const FVector3& lhs, const FVector3& rhs, float alpha);

    FVector3 midPoint(const FVector3& v1, const FVector3& v2);

    void makeFloor(FVector3& origin, const FVector3& cmp);

    void makeCeil(FVector3& origin, const FVector3& cmp);

    bool isNearZero(const FVector3& origin);

    bool isNaN(const FVector3& origin);

    float distance(const FVector3& a, const FVector3& b);

    float squaredDistance(const FVector3& a, const FVector3& b);

    FVector3 project(const FVector3& vec, const FVector3& normal);

    FVector3 absoluteCopy(const FVector3& vec);

    FVector3 clamp(const FVector3& v, const FVector3& min, const FVector3& max);

    float getMaxElement(const FVector3& v);

    FQuaternion getRotationTo(const FVector3& dest, const FVector3& fallback_axis);

    Radian angleBetween(const FVector3& from, const FVector3& dest);

    // ---------------------------------------------------------------------
    // ---------------------------------------------------------------------
    // ---------------------------------------------------------------------

    FVector4 lerp(const FVector4& lhs, const FVector4& rhs, float alpha);

    FVector4 midPoint(const FVector4& v1, const FVector4& v2);

    void makeFloor(FVector4& origin, const FVector4& cmp);

    void makeCeil(FVector4& origin, const FVector4& cmp);

    bool isNearZero(const FVector4& origin);

    bool isNaN(const FVector4& origin);

    float distance(const FVector4& a, const FVector4& b);

    float squaredDistance(const FVector4& a, const FVector4& b);

    FVector4 absoluteCopy(const FVector4& vec);

    FVector4 clamp(const FVector4& v, const FVector4& min, const FVector4& max);
} // namespace Piccolo
