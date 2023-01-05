#include "runtime/core/math/math.h"
#include "runtime/core/math/matrix4.h"

namespace Piccolo
{
    Math::AngleUnit Math::k_AngleUnit;

    Math::Math() { k_AngleUnit = AngleUnit::AU_DEGREE; }

    bool Math::realEqual(float a, float b, float tolerance /* = std::numeric_limits<float>::epsilon() */) { return std::fabs(b - a) <= tolerance; }

    float Math::degreesToRadians(float degrees) { return degrees * Math_fDeg2Rad; }

    float Math::radiansToDegrees(float radians) { return radians * Math_fRad2Deg; }

    float Math::angleUnitsToRadians(float angleunits)
    {
        if (k_AngleUnit == AngleUnit::AU_DEGREE)
            return angleunits * Math_fDeg2Rad;

        return angleunits;
    }

    float Math::radiansToAngleUnits(float radians)
    {
        if (k_AngleUnit == AngleUnit::AU_DEGREE)
            return radians * Math_fRad2Deg;

        return radians;
    }

    float Math::angleUnitsToDegrees(float angleunits)
    {
        if (k_AngleUnit == AngleUnit::AU_RADIAN)
            return angleunits * Math_fRad2Deg;

        return angleunits;
    }

    float Math::degreesToAngleUnits(float degrees)
    {
        if (k_AngleUnit == AngleUnit::AU_RADIAN)
            return degrees * Math_fDeg2Rad;

        return degrees;
    }

    Radian Math::acos(float value)
    {
        if (-1.0 < value)
        {
            if (value < 1.0)
                return Radian(::acos(value));

            return Radian(0.0);
        }

        return Radian(Math_PI);
    }
    //-----------------------------------------------------------------------
    Radian Math::asin(float value)
    {
        if (-1.0 < value)
        {
            if (value < 1.0)
                return Radian(::asin(value));

            return Radian(Math_HALF_PI);
        }

        return Radian(-Math_HALF_PI);
    }

    FMatrix4 Math::makeViewMatrix(const FVector3& position, const FQuaternion& orientation, const FMatrix4* reflect_matrix)
    {
        FMatrix4 viewMatrix;

        // View matrix is:
        //
        //  [ Lx  Uy  Dz  Tx  ]
        //  [ Lx  Uy  Dz  Ty  ]
        //  [ Lx  Uy  Dz  Tz  ]
        //  [ 0   0   0   1   ]
        //
        // Where T = -(Transposed(Rot) * Pos)

        // This is most efficiently done using 3x3 Matrices
        FMatrix3 rot = orientation.matrix();

        // Make the translation relative to new axes
        FMatrix3 rotT  = rot.transpose();
        FVector3 trans = -rotT * position;

        // Make final matrix
        viewMatrix = FMatrix4::Identity();
        viewMatrix.block<3, 3>(0, 0) = rotT; // fills upper 3x3
        viewMatrix.block<3, 1>(0, 3) = trans;

        // Deal with reflections
        if (reflect_matrix)
        {
            viewMatrix = viewMatrix * (*reflect_matrix);
        }

        return viewMatrix;
    }

    FMatrix4 Math::makeLookAtMatrix(const FVector3& eye_position, const FVector3& target_position, const FVector3& up_dir)
    {
        const FVector3& up = up_dir.normalized();

        FVector3 f = (target_position - eye_position).normalized();
        FVector3 s = f.cross(up).normalized();
        FVector3 u = s.cross(f);

        FMatrix4 view_mat = FMatrix4::Identity();
        view_mat.block<3, 1>(0, 0) = s;
        view_mat(0, 3)    = -s.dot(eye_position);
        view_mat.block<3, 1>(1, 0) = u;
        view_mat(1, 3)    = -u.dot(eye_position);
        view_mat.block<3, 1>(2, 0) = -f;
        view_mat(2, 3)    = f.dot(eye_position);
        return view_mat;
    }

    FMatrix4 Math::makePerspectiveMatrix(Radian fovy, float aspect, float znear, float zfar)
    {
        float tan_half_fovy = Math::tan(fovy / 2.f);

        FMatrix4 ret = FMatrix4::Zero();
        ret(0, 0)    = 1.f / (aspect * tan_half_fovy);
        ret(1, 1)    = 1.f / tan_half_fovy;
        ret(2, 2)    = zfar / (znear - zfar);
        ret(3, 2)    = -1.f;
        ret(2, 3)    = -(zfar * znear) / (zfar - znear);

        return ret;
    }

    FMatrix4 Math::makeOrthographicProjectionMatrix(float left, float right, float bottom, float top, float znear, float zfar)
    {
        float inv_width    = 1.0f / (right - left);
        float inv_height   = 1.0f / (top - bottom);
        float inv_distance = 1.0f / (zfar - znear);

        float A  = 2 * inv_width;
        float B  = 2 * inv_height;
        float C  = -(right + left) * inv_width;
        float D  = -(top + bottom) * inv_height;
        float q  = -2 * inv_distance;
        float qn = -(zfar + znear) * inv_distance;

        // NB: This creates 'uniform' orthographic projection matrix,
        // which depth range [-1,1], right-handed rules
        //
        // [ A   0   0   C  ]
        // [ 0   B   0   D  ]
        // [ 0   0   q   qn ]
        // [ 0   0   0   1  ]
        //
        // A = 2 * / (right - left)
        // B = 2 * / (top - bottom)
        // C = - (right + left) / (right - left)
        // D = - (top + bottom) / (top - bottom)
        // q = - 2 / (far - near)
        // qn = - (far + near) / (far - near)

        FMatrix4 proj_matrix = FMatrix4::Zero();
        proj_matrix(0, 0)    = A;
        proj_matrix(0, 3)    = C;
        proj_matrix(1, 1)    = B;
        proj_matrix(1, 3)    = D;
        proj_matrix(2, 2)    = q;
        proj_matrix(2, 3)    = qn;
        proj_matrix(3, 3)    = 1;

        return proj_matrix;
    }

    FMatrix4 Math::makeOrthographicProjectionMatrix01(float left, float right, float bottom, float top, float znear, float zfar)
    {
        float inv_width    = 1.0f / (right - left);
        float inv_height   = 1.0f / (top - bottom);
        float inv_distance = 1.0f / (zfar - znear);

        float A  = 2 * inv_width;
        float B  = 2 * inv_height;
        float C  = -(right + left) * inv_width;
        float D  = -(top + bottom) * inv_height;
        float q  = -1 * inv_distance;
        float qn = -znear * inv_distance;

        // NB: This creates 'uniform' orthographic projection matrix,
        // which depth range [-1,1], right-handed rules
        //
        // [ A   0   0   C  ]
        // [ 0   B   0   D  ]
        // [ 0   0   q   qn ]
        // [ 0   0   0   1  ]
        //
        // A = 2 * / (right - left)
        // B = 2 * / (top - bottom)
        // C = - (right + left) / (right - left)
        // D = - (top + bottom) / (top - bottom)
        // q = - 1 / (far - near)
        // qn = - near / (far - near)

        FMatrix4 proj_matrix = FMatrix4::Zero();
        proj_matrix(0, 0)    = A;
        proj_matrix(0, 3)    = C;
        proj_matrix(1, 1)    = B;
        proj_matrix(1, 3)    = D;
        proj_matrix(2, 2)    = q;
        proj_matrix(2, 3)    = qn;
        proj_matrix(3, 3)    = 1;

        return proj_matrix;
    }

} // namespace Piccolo
