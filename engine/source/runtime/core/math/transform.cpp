#include "runtime/core/math/transform.h"

namespace Piccolo
{
    FTransform3 Transform::getMatrix() const
    {
        FVector3 position = m_position.toFVector3();
        FVector3 scale = m_scale.toFVector3();
        FQuaternion rotation = m_rotation.toFQuaternion();

        FAffine affine;
        affine.scale(scale).rotate(rotation).translate(position);
        return affine.matrix();
    }
}
