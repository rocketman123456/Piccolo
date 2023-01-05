#pragma once

#include "runtime/core/math/vector3.h"
#include "runtime/core/meta/reflection/reflection.h"
#include <limits>

namespace Piccolo
{
    REFLECTION_TYPE(AxisAlignedBox)
    CLASS(AxisAlignedBox, Fields)
    {
        REFLECTION_BODY(AxisAlignedBox)
    public:
        AxisAlignedBox() {}
        AxisAlignedBox(const FVector3& center, const FVector3& half_extent);

        void merge(const FVector3& new_point);
        void update(const FVector3& center, const FVector3& half_extent);

        // only need to convert after initialization and before save
        void copyDataToFVector();
        void copyDataToVector();
        void collectData() { copyDataToVector(); }

        const FVector3& getCenter() const { return m_center_; }
        const FVector3& getHalfExtent() const { return m_half_extent_; }
        const FVector3& getMinCorner() const { return m_min_corner_; }
        const FVector3& getMaxCorner() const { return m_max_corner_; }

    private:
        Vector3 m_center {Vector3::ZERO};
        Vector3 m_half_extent {Vector3::ZERO};

        Vector3 m_min_corner {std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
        Vector3 m_max_corner {-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max()};

        META(Disable)
        FVector3 m_center_;
        META(Disable)
        FVector3 m_half_extent_;
        META(Disable)
        FVector3 m_min_corner_;
        META(Disable)
        FVector3 m_max_corner_;
    };
} // namespace Piccolo