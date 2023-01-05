#pragma once

namespace Piccolo
{
    class Matrix
    {
        float* m_mat {nullptr};
        int m_n {0};
        int m_m {0};

    public:
        Matrix(int n, int m);
        ~Matrix();
    };
}
