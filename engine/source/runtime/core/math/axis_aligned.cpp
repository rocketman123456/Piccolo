#include "runtime/core/math/axis_aligned.h"
#include "runtime/core/math/vector_util.h"

namespace Piccolo
{
    AxisAlignedBox::AxisAlignedBox(const FVector3& center, const FVector3& half_extent) { update(center, half_extent); }

    void AxisAlignedBox::merge(const FVector3& new_point)
    {
        makeFloor(m_min_corner_, new_point);
        makeCeil(m_max_corner_, new_point);

        m_center_      = 0.5f * (m_min_corner_ + m_max_corner_);
        m_half_extent_ = m_center_ - m_min_corner_;

        m_min_corner.fromFVector3(m_min_corner_);
        m_max_corner.fromFVector3(m_max_corner_);
        m_center.fromFVector3(m_center_);
        m_half_extent.fromFVector3(m_half_extent_);
    }

    void AxisAlignedBox::update(const FVector3& center, const FVector3& half_extent)
    {
        m_center_      = center;
        m_half_extent_ = half_extent;
        m_min_corner_  = center - half_extent;
        m_max_corner_  = center + half_extent;

        m_min_corner.fromFVector3(m_min_corner_);
        m_max_corner.fromFVector3(m_max_corner_);
        m_center.fromFVector3(m_center_);
        m_half_extent.fromFVector3(m_half_extent_);
    }

} // namespace Piccolo