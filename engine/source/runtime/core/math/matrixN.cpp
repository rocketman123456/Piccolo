#include "runtime/core/math"

namespace Piccolo
{
    Matrix::Matrix(int n, int m) : m_n(n), m_m(m) { m_mat = new float(n * m); }

    ~Matrix::Matrix()
    {
        if (m_mat != nullptr)
            delete[] m_mat;
    }
} // namespace Piccolo