#pragma once

#include <Eigen/Eigen>

namespace Piccolo
{
    using FVector2 = Eigen::Vector2f;
    using FVector3 = Eigen::Vector3f;
    using FVector4 = Eigen::Vector4f;

    using DVector2 = Eigen::Vector2d;
    using DVector3 = Eigen::Vector3d;
    using DVector4 = Eigen::Vector4d;

    using FMatrix2 = Eigen::Matrix2f;
    using FMatrix3 = Eigen::Matrix3f;
    using FMatrix4 = Eigen::Matrix4f;

    using DMatrix2 = Eigen::Matrix2d;
    using DMatrix3 = Eigen::Matrix3d;
    using DMatrix4 = Eigen::Matrix4d;

    using FMatrixX = Eigen::MatrixXf;
    using DMatrixX = Eigen::MatrixXd;

    using FQuaternion = Eigen::Quaternionf;
    using DQuaternion = Eigen::Quaterniond;

    using FTranslation2 = Eigen::Translation2f;
    using FTranslation3 = Eigen::Translation3f;

    using DTranslation2 = Eigen::Translation2d;
    using DTranslation3 = Eigen::Translation3d;

    using FAngleAxis = Eigen::AngleAxisf;
    using DAngleAxis = Eigen::AngleAxisd;

    using FAffine = Eigen::Affine3f;
    using DAffine = Eigen::Affine3d;

    using FTransform = Eigen::Transform;
} // namespace Piccolo
