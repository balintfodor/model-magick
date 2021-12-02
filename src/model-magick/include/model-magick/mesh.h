#pragma once

#include <Eigen/Core>

namespace ModelMagick {

struct Mesh {
    Eigen::MatrixX3f vertices;
    Eigen::MatrixX3i indices;
};

}  // namespace ModelMagick
