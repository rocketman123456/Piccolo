#pragma once

#include "runtime/core/math/math.h"
#include "runtime/core/math/math_defines.h"
#include "runtime/core/meta/reflection/reflection.h"

#include <cassert>

namespace Piccolo
{
    REFLECTION_TYPE(Matrix2x2)
    CLASS(Matrix2x2, Fields)
    {
        REFLECTION_BODY(Matrix2x2);

    public:
        float v0 {1.f};
        float v1 {0};
        float v2 {0};
        float v3 {1.f};

        META(Disable)
        FMatrix2 data;
    };
}