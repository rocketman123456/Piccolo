#include "runtime/core/math/vector_util.h"

namespace Piccolo
{
    // ---------------------------------------------------------------------
    // ---------------------------------------------------------------------
    // ---------------------------------------------------------------------

    FVector2 reflect(const FVector2& point, const FVector2& normal) { return point - (2 * point.dot(normal) * normal); }

    FVector2 lerp(const FVector2& lhs, const FVector2& rhs, float alpha) { return lhs + alpha * (rhs - lhs); }

    FVector2 midPoint(const FVector2& v1, const FVector2& v2) { return v1 + 0.5 * v2; }

    void makeFloor(FVector2& origin, const FVector2& cmp)
    {
        if (cmp[0] < origin[0])
            origin[0] = cmp[0];
        if (cmp[1] < origin[1])
            origin[1] = cmp[1];
    }

    void makeCeil(FVector2& origin, const Vector2& cmp)
    {
        if (cmp[0] > origin[0])
            origin[0] = cmp[0];
        if (cmp[1] > origin[1])
            origin[1] = cmp[1];
    }

    FVector2 perpendicular(const FVector2& origin) { return FVector2(-origin[1], origin[0]); }

    bool isNearZero(const FVector2& origin) { return origin.squaredNorm() < (Float_EPSILON * Float_EPSILON); }

    bool isNaN(const FVector2& origin) { return Math::isNan(origin[0]) || Math::isNan(origin[1]); }

    float distance(const FVector2& a, const FVector2& b) { return (a - b).norm(); }

    float squaredDistance(const FVector2& a, const FVector2& b) { return (a - b).squaredNorm(); }

    // ---------------------------------------------------------------------
    // ---------------------------------------------------------------------
    // ---------------------------------------------------------------------

    FVector3 reflect(const FVector3& point, const FVector3& normal) { return point - (2 * point.dot(normal) * normal); }

    FVector3 lerp(const FVector3& lhs, const FVector3& rhs, float alpha) { return lhs + alpha * (rhs - lhs); }

    FVector3 midPoint(const FVector3& v1, const FVector3& v2) { return v1 + 0.5 * v2; }

    void makeFloor(FVector3& origin, const FVector3& cmp)
    {
        if (cmp[0] < origin[0])
            origin[0] = cmp[0];
        if (cmp[1] < origin[1])
            origin[1] = cmp[1];
        if (cmp[2] < origin[2])
            origin[2] = cmp[2];
    }

    void makeCeil(FVector3& origin, const FVector3& cmp)
    {
        if (cmp[0] > origin[0])
            origin[0] = cmp[0];
        if (cmp[1] > origin[1])
            origin[1] = cmp[1];
        if (cmp[2] > origin[2])
            origin[2] = cmp[2];
    }

    bool isNearZero(const FVector3& origin) { return origin.squaredNorm() < (Float_EPSILON * Float_EPSILON); }

    bool isNaN(const FVector3& origin) { return Math::isNan(origin[0]) || Math::isNan(origin[1]) || Math::isNan(origin[2]); }

    float distance(const FVector3& a, const FVector3& b) { return (a - b).norm(); }

    float squaredDistance(const FVector3& a, const FVector3& b) { return (a - b).squaredNorm(); }

    FVector3 project(const FVector3& vec, const FVector3& normal) { return vec - (vec.dot(normal) * normal); }

    FVector3 absoluteCopy(const FVector3& vec) { return FVector3(fabsf(vec[0]), fabsf(vec[1]), fabsf(vec[2])); }

    FVector3 clamp(const FVector3& v, const FVector3& min, const FVector3& max)
    {
        return FVector3(Math::clamp(v[0], min[0], max[0]), Math::clamp(v[1], min[1], max[1]), Math::clamp(v[2], min[2], max[2]));
    }

    float getMaxElement(const FVector3& v) { return Math::getMaxElement(v[0], v[1], v[2]); }

    FQuaternion getRotationTo(const FVector3& from, const FVector3& dest, const FVector3& fallback_axis = FVector3::Zero())
    {
        // Based on Stan Melax's article in Game Programming Gems
        FQuaternion q;
        // Copy, since cannot modify local
        FVector3 v0 = from;
        FVector3 v1 = dest;
        v0.normalize();
        v1.normalize();

        // q = FQuaternion::FromTwoVectors(v0, v1);

        float d = v0.dot(v1);
        // If dot == 1, vectors are the same
        if (d >= 1.0f)
        {
            return FQuaternion::Identity();
        }
        if (d < (1e-6f - 1.0f))
        {
            if (fallback_axis != FVector3::Zero())
            {
                // rotate 180 degrees about the fall back axis
                q = FQuaternion(FAngleAxis(Radian(Math_PI).valueRadians(), fallback_axis));
            }
            else
            {
                // Generate an axis
                FVector3 axis = UNIT_X_3.cross(v0);
                if (isNearZero(axis)) // pick another if collinear
                    axis = UNIT_Y_3.cross(v0);
                axis.normalize();
                q = FQuaternion(FAngleAxis(Radian(Math_PI).valueRadians(), axis));
            }
        }
        else
        {
            float s    = Math::sqrt((1 + d) * 2);
            float invs = 1 / s;

            FVector3 c = v0.cross(v1);
            q.x()      = c[0] * invs;
            q.y()      = c[1] * invs;
            q.z()      = c[2] * invs;
            q.w()      = s * 0.5f;
            q.normalize();
        }
        return q;
    }

    Radian angleBetween(const FVector3& from, const FVector3& dest)
    {
        float len_product = from.norm() * dest.norm();

        // Divide by zero check
        if (len_product < 1e-6f)
            len_product = 1e-6f;

        float f = from.dot(dest) / len_product;

        f = Math::clamp(f, (float)-1.0, (float)1.0);
        return Math::acos(f);
    }

    // ---------------------------------------------------------------------
    // ---------------------------------------------------------------------
    // ---------------------------------------------------------------------

    FVector4 lerp(const FVector4& lhs, const FVector4& rhs, float alpha) { return lhs + alpha * (rhs - lhs); }

    FVector4 midPoint(const FVector4& v1, const FVector4& v2) { return v1 + 0.5 * v2; }

    void makeFloor(FVector4& origin, const FVector4& cmp)
    {
        if (cmp[0] < origin[0])
            origin[0] = cmp[0];
        if (cmp[1] < origin[1])
            origin[1] = cmp[1];
        if (cmp[2] < origin[2])
            origin[2] = cmp[2];
        if (cmp[3] < origin[3])
            origin[3] = cmp[3];
    }

    void makeCeil(FVector4& origin, const FVector4& cmp)
    {
        if (cmp[0] > origin[0])
            origin[0] = cmp[0];
        if (cmp[1] > origin[1])
            origin[1] = cmp[1];
        if (cmp[2] > origin[2])
            origin[2] = cmp[2];
        if (cmp[3] > origin[3])
            origin[3] = cmp[3];
    }

    bool isNearZero(const FVector4& origin) { return origin.squaredNorm() < (Float_EPSILON * Float_EPSILON); }

    bool isNaN(const FVector4& origin) { return Math::isNan(origin[0]) || Math::isNan(origin[1]) || Math::isNan(origin[2]) || Math::isNan(origin[3]); }

    float distance(const FVector4& a, const FVector4& b) { return (a - b).norm(); }

    float squaredDistance(const FVector4& a, const FVector4& b) { return (a - b).squaredNorm(); }

    FVector4 absoluteCopy(const FVector4& vec) { return FVector4(fabsf(vec[0]), fabsf(vec[1]), fabsf(vec[2]), fabsf(vec[3])); }

    FVector4 clamp(const FVector4& v, const FVector4& min, const FVector4& max)
    {
        return FVector4(Math::clamp(v[0], min[0], max[0]), Math::clamp(v[1], min[1], max[1]), Math::clamp(v[2], min[2], max[2]), Math::clamp(v[3], min[3], max[3]));
    }
} // namespace Piccolo
