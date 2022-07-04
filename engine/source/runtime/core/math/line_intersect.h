#pragma once

#include <utility>

#include "runtime/core/math/vector3.h"

namespace Piccolo
{
    // From Game Programming Gems 2-2.3
    // Will return if two line really intersect

    /**************************************************************************
    |
    |     Method: IntersectLineSegments
    |
    |    Purpose: Find the nearest point between two finite length line segments
    |             or two infinite lines in 3-dimensional space. The function calculates
    |             the point on each line/line segment that is closest to the other
    |             line/line segment, the midpoint between the nearest points, and
    |             the vector between these two points. If the two nearest points
    |             are close within a tolerance, a flag is set indicating the lines
    |             have a "true" intersection.
    |
    | Parameters: Input:
    |             ------
    |             A1   - Coordinates of first defining point of line/segment A
    |             A2   - Coordinates of second defining point of line/segment A
    |             B1   - Coordinates of first defining point of line/segment B
    |             B2   - Coordinates of second defining point of line/segment B
    |             infinite_lines  - set to true if lines are to be treated as infinite
    |             epsilon         - tolerance value to be used to check for degenerate
    |                               and parallel lines, and to check for true intersection.
    |
    |             Output:
    |             -------
    |             An,   - Coordinates of the point on segment A that are nearest
    |             An,     to segment B. This corresponds to point C in the text.
    |             An
    |             Bn,   - Coordinates of the point on segment B that are nearest
    |             Bn,     to segment A. This corresponds to point D in the text.
    |             Bn
    |             Z,  - Midpoint between the two nearest points. This can be
    |             Z,    treated as *the* intersection point if nearest points
    |             Z     are sufficiently close. This corresponds to point P
    |                               in the text.
    |             V, - Vector between the nearest point on A to the nearest
    |                               point on segment B. This vector is normal to both
    |                               lines if the lines are infinite, but is not guaranteed
    |                               to be normal to both lines if both lines are finite
    |                               length.
    |           true_intersection - true if the nearest points are close within a small
    |                               tolerance.
    **************************************************************************/
    bool intersect_line_segments(const Vector3& A1,
                                 const Vector3& A2,
                                 const Vector3& B1,
                                 const Vector3& B2,
                                 bool           infinite_lines,
                                 float          epsilon,
                                 Vector3&       An,
                                 Vector3&       Bn,
                                 Vector3&       Z,
                                 Vector3&       V);

    /**************************************************************************
    |
    |     Method: FindNearestPointOnLineSegment
    |
    |    Purpose: Given a line (segment) and a point in 3-dimensional space,
    |             find the point on the line (segment) that is closest to the
    |             point.
    |
    | Parameters: Input:
    |             ------
    |             A1   - Coordinates of first defining point of the line/segment
    |             L    - Vector from (A1x, A1y, A1z) to the second defining point
    |                               of the line/segment.
    |             B    - Coordinates of the point
    |             infinite_lines  - set to true if lines are to be treated as infinite
    |             epsilon_squared - tolerance value to be used to check for degenerate
    |                               and parallel lines, and to check for true intersection.
    |
    |             Output:
    |             -------
    |             nearest,  - Point on line/segment that is closest to (Bx, By, Bz)
    |             nearest,
    |             nearest
    |             parameter       - Parametric coordinate of the nearest point along the
    |                               line/segment. parameter = 0 at (A1x, A1y, A1z) and
    |                               parameter = 1 at the second defining point of the line/
    |                               segmetn
    **************************************************************************/
    void find_nearest_point_on_line_segment(const Vector3& A1,
                                            const Vector3& L,
                                            const Vector3& B,
                                            bool           infinite_line,
                                            float          epsilon_squared,
                                            Vector3&       nearest,
                                            float&         parameter);

    /**************************************************************************
    |
    |     Method: FindNearestPointOfParallelLineSegments
    |
    |    Purpose: Given two lines (segments) that are known to be parallel, find
    |             a representative point on each that is nearest to the other. If
    |             the lines are considered to be finite then it is possible that there
    |             is one true point on each line that is nearest to the other. This
    |             code properly handles this case.
    |
    |             This is the most difficult line intersection case to handle, since
    |             there is potentially a family, or locus of points on each line/segment
    |             that are nearest to the other.
    | Parameters: Input:
    |             ------
    |             A1   - Coordinates of first defining point of line/segment A
    |             A2   - Coordinates of second defining point of line/segment A
    |             La   - Vector from (A1x, A1y, A1z) to the (A2x, A2y, A2z).
    |             B1   - Coordinates of first defining point of line/segment B
    |             B2   - Coordinates of second defining point of line/segment B
    |             Lb   - Vector from (B1x, B1y, B1z) to the (B2x, B2y, B2z).
    |             infinite_lines  - set to true if lines are to be treated as infinite
    |             epsilon_squared - tolerance value to be used to check for degenerate
    |                               and parallel lines, and to check for true intersection.
    |
    |             Output:
    |             -------
    |             An,   - Coordinates of the point on segment A that are nearest
    |             An,     to segment B. This corresponds to point C in the text.
    |             An
    |             Bn,   - Coordinates of the point on segment B that are nearest
    |             Bn,     to segment A. This corresponds to point D in the text.
    |             Bn
    **************************************************************************/
    void find_nearest_point_of_parallel_line_segments(const Vector3& A1,
                                                      const Vector3& A2,
                                                      const Vector3& La,
                                                      const Vector3& B1,
                                                      const Vector3& B2,
                                                      const Vector3& Lb,
                                                      bool           infinite_lines,
                                                      float          epsilon_squared,
                                                      Vector3&       An,
                                                      Vector3&       Bn);

    /**************************************************************************
    |
    |     Method: AdjustNearestPoints
    |
    |    Purpose: Given nearest point information for two infinite lines, adjust
    |             to model finite line segments.
    |
    | Parameters: Input:
    |             ------
    |             A1   - Coordinates of first defining point of line/segment A
    |             La   - Vector from (A1x, A1y, A1z) to the (A2x, A2y, A2z).
    |             B1   - Coordinates of first defining point of line/segment B
    |             Lb   - Vector from (B1x, B1y, B1z) to the (B2x, B2y, B2z).
    |             epsilon_squared - tolerance value to be used to check for degenerate
    |                               and parallel lines, and to check for true intersection.
    |             s               - parameter representing nearest point on infinite line A
    |             t               - parameter representing nearest point on infinite line B
    |
    |             Output:
    |             -------
    |             An,   - Coordinates of the point on segment A that are nearest
    |             An,     to segment B. This corresponds to point C in the text.
    |             An
    |             Bn,   - Coordinates of the point on segment B that are nearest
    |             Bn,     to segment A. This corresponds to point D in the text.
    |             Bn
    **************************************************************************/
    void adjust_nearest_points(const Vector3& A1,
                               const Vector3& La,
                               const Vector3& B1,
                               const Vector3& Lb,
                               float          epsilon_squared,
                               float          s,
                               float          t,
                               Vector3&       An,
                               Vector3&       Bn);
} // namespace Pilot
