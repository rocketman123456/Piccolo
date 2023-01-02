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

        explicit Vector2(float* const r) : x(r[0]), y(r[1]) {}

        explicit Vector2(FVector2& v) : x(v[0]), y(v[1]) {}

        explicit Vector2(const FVector2& v) : x(v[0]), y(v[1]) {}

        float* ptr() { return &x; }

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

        FVector2 toFVector2()
        {
            FVector2 data(x, y);
            return data;
        }

        void fromFVector2(const FVector2& v)
        {
            x = v[0];
            y = v[1];
        }

        float normalise()
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

        float getX() const { return x; }
        float getY() const { return y; }

        void setX(float value) { x = value; }
        void setY(float value) { y = value; }

        // /** Returns true if the vector's scalar components are all greater
        // that the ones of the vector it is compared against.
        // */
        // bool operator<(const Vector2& rhs) const { return x < rhs.x && y < rhs.y; }

        // /** Returns true if the vector's scalar components are all smaller
        // that the ones of the vector it is compared against.
        // */
        // bool operator>(const Vector2& rhs) const { return x > rhs.x && y > rhs.y; }

        // /** Sets this vector's components to the minimum of its own and the
        // ones of the passed in vector.
        // @remarks
        // 'Minimum' in this case means the combination of the lowest
        // value of x, y and z from both vectors. Lowest is taken just
        // numerically, not magnitude, so -1 < 0.
        // */
        // void makeFloor(const Vector2& cmp)
        // {
        //     if (cmp.x < x)
        //         x = cmp.x;
        //     if (cmp.y < y)
        //         y = cmp.y;
        // }

        // /** Sets this vector's components to the maximum of its own and the
        // ones of the passed in vector.
        // @remarks
        // 'Maximum' in this case means the combination of the highest
        // value of x, y and z from both vectors. Highest is taken just
        // numerically, not magnitude, so 1 > -3.
        // */
        // void makeCeil(const Vector2& cmp)
        // {
        //     if (cmp.x > x)
        //         x = cmp.x;
        //     if (cmp.y > y)
        //         y = cmp.y;
        // }

        // /** Generates a vector perpendicular to this vector (eg an 'up' vector).
        // @remarks
        // This method will return a vector which is perpendicular to this
        // vector. There are an infinite number of possibilities but this
        // method will guarantee to generate one of them. If you need more
        // control you should use the Quaternion class.
        // */
        // Vector2 perpendicular(void) const { return Vector2(-y, x); }

        // /** Calculates the 2 dimensional cross-product of 2 vectors, which results
        // in a single floating point value which is 2 times the area of the triangle.
        // */
        // float crossProduct(const Vector2& rhs) const { return x * rhs.y - y * rhs.x; }

        // /** Returns true if this vector is zero length. */
        // bool isZeroLength(void) const
        // {
        //     float sqlen = (x * x) + (y * y);
        //     return (sqlen < (Float_EPSILON * Float_EPSILON));
        // }

        // /** As normalise, except that this vector is unaffected and the
        // normalised vector is returned as a copy. */
        // Vector2 normalisedCopy(void) const
        // {
        //     Vector2 ret = *this;
        //     ret.normalise();
        //     return ret;
        // }

        // /** Calculates a reflection vector to the plane with the given normal .
        // @remarks NB assumes 'this' is pointing AWAY FROM the plane, invert if it is not.
        // */
        // Vector2 reflect(const Vector2& normal) const { return Vector2(*this - (2 * this->dotProduct(normal) * normal)); }

        // /// Check whether this vector contains valid values
        // bool isNaN() const { return Math::isNan(x) || Math::isNan(y); }

        // static Vector2 lerp(const Vector2& lhs, const Vector2& rhs, float alpha) { return lhs + alpha * (rhs - lhs); }

        // special points
        static const Vector2 ZERO;
        static const Vector2 UNIT_X;
        static const Vector2 UNIT_Y;
        static const Vector2 NEGATIVE_UNIT_X;
        static const Vector2 NEGATIVE_UNIT_Y;
        static const Vector2 UNIT_SCALE;
    };

} // namespace Piccolo
