#include "runtime/core/math/matrix2.h"

namespace Piccolo
{
    const Matrix2x2 Matrix2x2::ZERO({0, 0, 0, 0});
    const Matrix2x2 Matrix2x2::IDENTITY({1, 0, 0, 1});

    Matrix2x2::Matrix2x2(const float (&float_array)[4])
    {
        this->v0 = float_array[0];
        this->v1 = float_array[1];
        this->v2 = float_array[2];
        this->v3 = float_array[3];
    }

    FMatrix2 Matrix2x2::toFMatrix2() const
    {
        FMatrix2 data;
        data << v0, v1, v2, v3;
        return data;
    }

    void Matrix2x2::fromFMatrix2(const FMatrix2& v)
    {
        v0 = v(0, 0);
        v1 = v(0, 1);
        v2 = v(1, 0);
        v3 = v(1, 1);
    }
} // namespace Piccolo
