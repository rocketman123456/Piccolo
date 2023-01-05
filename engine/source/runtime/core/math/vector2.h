#pragma once

#include "runtime/core/math/math.h"
#include "runtime/core/math/math_defines.h"
#include "runtime/core/meta/reflection/reflection.h"

#include <cassert>
#include <cmath>

namespace Piccolo
{
    REFLECTION_TYPE(Vector2)
    CLASS(Vector2, Fields)
    {
        REFLECTION_BODY(Vector2);

    public:
        float x {0.f}, y {0.f};

    public:
        Vector2() = default;
        Vector2(float x_, float y_) : x(x_), y(y_) {}

        explicit Vector2(float scaler) : x(scaler), y(scaler) {}

        explicit Vector2(const float v[2]) : x(v[0]), y(v[1]) {}

        // explicit Vector2(float* const r) : x(r[0]), y(r[1]) {}

        explicit Vector2(FVector2 & v) : x(v[0]), y(v[1]) {}

        explicit Vector2(const FVector2& v) : x(v[0]), y(v[1]) {}

        float*       ptr() { return &x; }
        const float* ptr() const { return &x; }

        float operator[](size_t i) const
        {
            assert(i < 2);
            return (i == 0 ? x : y);
        }

        float& operator[](size_t i)
        {
            assert(i < 2);
            return (i == 0 ? x : y);
        }

        FVector2 toFVector2() const
        {
            FVector2 data(x, y);
            return data;
        }

        void fromFVector2(const FVector2& v)
        {
            x = v[0];
            y = v[1];
        }

        float normalize()
        {
            float lengh = std::hypot(x, y);
            if (lengh > 0.0f)
            {
                float inv_length = 1.0f / lengh;
                x *= inv_length;
                y *= inv_length;
            }
            return lengh;
        }

        float length() const { return std::hypot(x, y); }
        float squaredLength() const { return x * x + y * y; }

        // special points
        static const Vector2 ZERO;
        static const Vector2 UNIT_X;
        static const Vector2 UNIT_Y;
        static const Vector2 NEGATIVE_UNIT_X;
        static const Vector2 NEGATIVE_UNIT_Y;
        static const Vector2 UNIT_SCALE;
    };

} // namespace Piccolo
