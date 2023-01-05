#pragma once

#include "runtime/core/math/math.h"
#include "runtime/core/math/math_defines.h"
#include "runtime/core/math/quaternion.h"
#include "runtime/core/meta/reflection/reflection.h"

#include <cassert>

namespace Piccolo
{
    REFLECTION_TYPE(Vector3)
    CLASS(Vector3, Fields)
    {
        REFLECTION_BODY(Vector3);

    public:
        float x {0.f};
        float y {0.f};
        float z {0.f};

    public:
        Vector3() = default;
        Vector3(float x_, float y_, float z_) : x {x_}, y {y_}, z {z_} {}

        explicit Vector3(float scaler) : x(scaler), y(scaler), z {scaler} {}

        explicit Vector3(const float coords[3]) : x {coords[0]}, y {coords[1]}, z {coords[2]} {}

        explicit Vector3(FVector3 & v) : x {v[0]}, y {v[1]}, z {v[2]} {}

        explicit Vector3(const FVector3& v) : x {v[0]}, y {v[1]}, z {v[2]} {}

        float operator[](size_t i) const
        {
            assert(i < 3);
            return *(&x + i);
        }

        float& operator[](size_t i)
        {
            assert(i < 3);
            return *(&x + i);
        }

        float*       ptr() { return &x; }
        const float* ptr() const { return &x; }

        FVector3 toFVector3() const
        {
            FVector3 data(x, y, z);
            return data;
        }

        void fromFVector3(const FVector3& v)
        {
            x = v[0];
            y = v[1];
            z = v[2];
        }

        float length() const { return std::hypot(x, y, z); }
        float squaredLength() const { return x * x + y * y + z * z; }

        void normalize()
        {
            float length = std::hypot(x, y, z);
            if (length > 0.f)
            {
                float inv_lengh = 1.0f / length;
                x *= inv_lengh;
                y *= inv_lengh;
                z *= inv_lengh;
            }
        }

        // special points
        static const Vector3 ZERO;
        static const Vector3 UNIT_X;
        static const Vector3 UNIT_Y;
        static const Vector3 UNIT_Z;
        static const Vector3 NEGATIVE_UNIT_X;
        static const Vector3 NEGATIVE_UNIT_Y;
        static const Vector3 NEGATIVE_UNIT_Z;
        static const Vector3 UNIT_SCALE;
    };
} // namespace Piccolo
