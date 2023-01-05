#pragma once

#include "runtime/core/math/math.h"
#include "runtime/core/math/vector3.h"
#include "runtime/core/meta/reflection/reflection.h"

namespace Piccolo
{
    REFLECTION_TYPE(Vector4)
    CLASS(Vector4, Fields)
    {
        REFLECTION_BODY(Vector4);

    public:
        float x {0.f}, y {0.f}, z {0.f}, w {0.f};

    public:
        Vector4() = default;
        Vector4(float x_, float y_, float z_, float w_) : x {x_}, y {y_}, z {z_}, w {w_} {}
        Vector4(const Vector3& v3, float w_) : x {v3.x}, y {v3.y}, z {v3.z}, w {w_} {}

        explicit Vector4(float coords[4]) : x {coords[0]}, y {coords[1]}, z {coords[2]}, w {coords[3]} {}

        explicit Vector4(const FVector4& v) : x {v[0]}, y {v[1]}, z {v[2]}, w {v[3]} {}

        float operator[](size_t i) const
        {
            assert(i < 4);
            return *(&x + i);
        }

        float& operator[](size_t i)
        {
            assert(i < 4);
            return *(&x + i);
        }

        /// Pointer accessor for direct copying
        float* ptr() { return &x; }
        /// Pointer accessor for direct copying
        const float* ptr() const { return &x; }

        FVector4 toFVector4() const
        {
            FVector4 data(x, y, z, w);
            return data;
        }

        void fromFVector4(const FVector4& v)
        {
            x = v[0];
            y = v[1];
            z = v[2];
            w = v[3];
        }

        // special
        static const Vector4 ZERO;
        static const Vector4 UNIT_SCALE;
    };

} // namespace Piccolo
