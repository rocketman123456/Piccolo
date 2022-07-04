#include "runtime/core/math/line_intersect.h"

#include <cassert>
#include <cmath>

// uncomment the following line to have the code check intermediate results
//#define CHECK_ANSWERS

// uncomment the following line to use Cramer's rule instead of Gaussian elimination
//#define USE_CRAMERS_RULE

#define FMAX(a, b) ((a) > (b) ? (a) : (b))
#define FMIN(a, b) ((a) > (b) ? (b) : (a))
#define FABS(a) ((a) < 0.0f ? -(a) : (a))
#define OUT_OF_RANGE(a) ((a) < 0.0f || (a) > 1.f)

namespace Piccolo
{
    bool intersect_line_segments(const Vector3& A1,
                                 const Vector3& A2,
                                 const Vector3& B1,
                                 const Vector3& B2,
                                 bool           infinite_lines,
                                 float          epsilon,
                                 Vector3&       An,
                                 Vector3&       Bn,
                                 Vector3&       Z,
                                 Vector3&       V)
    {
        float temp            = 0.f;
        float epsilon_squared = epsilon * epsilon;

        // Compute parameters from Equations (1) and (2) in the text
        Vector3 La = A2 - A1; // Vector3(A2.x - A1.x, A2.y - A1.y, A2.z - A1.z);
        Vector3 Lb = B2 - B1; // Vector3(B2.x - B1.x, B2.y - B1.y, B2.z - B1.z);.
        // From Equation (15)
        float L11 = (La.x * La.x) + (La.y * La.y) + (La.z * La.z);
        float L22 = (Lb.x * Lb.x) + (Lb.y * Lb.y) + (Lb.z * Lb.z);

        // Line/Segment A is degenerate ---- Special Case #1
        if (L11 < epsilon_squared)
        {
            An = A1;
            find_nearest_point_on_line_segment(B1, Lb, A1, infinite_lines, epsilon, Bn, temp);
        }
        // Line/Segment B is degenerate ---- Special Case #1
        else if (L22 < epsilon_squared)
        {
            Bn = B1;
            find_nearest_point_on_line_segment(A1, La, B1, infinite_lines, epsilon, An, temp);
        }
        // Neither line/segment is degenerate
        else
        {
            // Compute more parameters from Equation (3) in the text.
            Vector3 AB = B1 - A1;

            // and from Equation (15).
            float L12 = -(La.x * Lb.x) - (La.y * Lb.y) - (La.z * Lb.z);

            float DetL = L11 * L22 - L12 * L12;
            // Lines/Segments A and B are parallel ---- special case #2.
            if (FABS(DetL) < epsilon)
            {
                find_nearest_point_of_parallel_line_segments(A1, A2, La, B1, B2, Lb, infinite_lines, epsilon, An, Bn);
            }
            // The general case
            else
            {
                // from Equation (15)
                float ra = La.x * AB.x + La.y * AB.y + La.z * AB.z;
                float rb = -Lb.x * AB.x - Lb.y * AB.y - Lb.z * AB.z;

                float t = (L11 * rb - ra * L12) / DetL; // Equation (12)

#ifdef USE_CRAMERS_RULE
                float s = (L22 * ra - rb * L12) / DetL;
#else
                float s = (ra - L12 * t) / L11; // Equation (13)
#endif // USE_CRAMERS_RULE

#ifdef CHECK_ANSWERS
                float check_ra = s * L11 + t * L12;
                float check_rb = s * L12 + t * L22;
                assert(FABS(check_ra - ra) < epsilon);
                assert(FABS(check_rb - rb) < epsilon);
#endif // CHECK_ANSWERS

                // if we are dealing with infinite lines or if parameters s and t both
                // lie in the range [0,1] then just compute the points using Equations
                // (1) and (2) from the text.
                An = A1 + s * La;
                Bn = B1 + t * Lb;
                // otherwise, at least one of s and t is outside of [0,1] and we have to
                // handle this case.
                if (false == infinite_lines && (OUT_OF_RANGE(s) || OUT_OF_RANGE(t)))
                {
                    adjust_nearest_points(A1, La, B1, Lb, epsilon, s, t, An, Bn);
                }
            }
        }

        Z = 0.5 * (An + Bn);
        V = Bn - An;

        // optional check to indicate if the lines truly intersect
        bool true_intersection = (FABS(V.x) + FABS(V.y) + FABS(V.z)) < epsilon ? true : false;
        return true_intersection;
    }

    void find_nearest_point_on_line_segment(const Vector3& A1,
                                            const Vector3& L,
                                            const Vector3& B,
                                            bool           infinite_line,
                                            float          epsilon_squared,
                                            Vector3&       nearest,
                                            float&         parameter)
    {
        // Line/Segment is degenerate --- special case #1
        float D = L.x * L.x + L.y * L.y + L.z * L.z;
        if (D < epsilon_squared)
        {
            nearest = A1;
            return;
        }

        Vector3 AB = B - A1;

        // parameter is computed from Equation (20).
        parameter = (L.x * AB.x + L.y * AB.y + L.z * AB.z) / D;

        if (false == infinite_line)
            parameter = FMAX(0.0f, FMIN(1.0f, parameter));

        nearest = A1 + parameter * L;
        return;
    }

    void find_nearest_point_of_parallel_line_segments(const Vector3& A1,
                                                      const Vector3& A2,
                                                      const Vector3& La,
                                                      const Vector3& B1,
                                                      const Vector3& B2,
                                                      const Vector3& Lb,
                                                      bool           infinite_lines,
                                                      float          epsilon_squared,
                                                      Vector3&       An,
                                                      Vector3&       Bn)
    {
        float s[2], temp;
        find_nearest_point_on_line_segment(A1, La, B1, true, epsilon_squared, An, s[0]);
        if (true == infinite_lines)
        {
            Bn = B1;
        }
        else
        {
            Vector3 tp;
            find_nearest_point_on_line_segment(A1, La, B2, true, epsilon_squared, tp, s[1]);
            if (s[0] < 0.f && s[1] < 0.f)
            {
                An = A1;
                if (s[0] < s[1])
                {
                    Bn = B2;
                }
                else
                {
                    Bn = B1;
                }
            }
            else if (s[0] > 1.f && s[1] > 1.f)
            {
                An = A2;
                if (s[0] < s[1])
                {
                    Bn = B1;
                }
                else
                {
                    Bn = B2;
                }
            }
            else
            {
                temp = 0.5f * (FMAX(0.0f, FMIN(1.0f, s[0])) + FMAX(0.0f, FMIN(1.0f, s[1])));
                An   = A1 + temp * La;
                find_nearest_point_on_line_segment(B1, Lb, An, true, epsilon_squared, Bn, temp);
            }
        }
    }

    void adjust_nearest_points(const Vector3& A1,
                               const Vector3& La,
                               const Vector3& B1,
                               const Vector3& Lb,
                               float          epsilon_squared,
                               float          s,
                               float          t,
                               Vector3&       An,
                               Vector3&       Bn)
    {
        // handle the case where both parameter s and t are out of range
        if (OUT_OF_RANGE(s) && OUT_OF_RANGE(t))
        {
            s  = FMAX(0.0f, FMIN(1.0f, s));
            An = A1 + s * La;
            find_nearest_point_on_line_segment(B1, Lb, An, true, epsilon_squared, Bn, t);
            if (OUT_OF_RANGE(t))
            {
                t  = FMAX(0.0f, FMIN(1.0f, t));
                Bn = B1 + t * Lb;
                find_nearest_point_on_line_segment(A1, La, Bn, false, epsilon_squared, An, s);
                find_nearest_point_on_line_segment(B1, Lb, An, false, epsilon_squared, Bn, t);
            }
        }
        // otherwise, handle the case where the parameter for only one segment is
        // out of range
        else if (OUT_OF_RANGE(s))
        {
            s  = FMAX(0.0f, FMIN(1.0f, s));
            An = A1 + s * La;
            find_nearest_point_on_line_segment(B1, Lb, An, false, epsilon_squared, Bn, t);
        }
        else if (OUT_OF_RANGE(t))
        {
            t  = FMAX(0.0f, FMIN(1.0f, t));
            Bn = B1 + t * Lb;
            find_nearest_point_on_line_segment(A1, La, Bn, false, epsilon_squared, An, s);
        }
        else
        {
            assert(0);
        }
    }
} // namespace Pilot