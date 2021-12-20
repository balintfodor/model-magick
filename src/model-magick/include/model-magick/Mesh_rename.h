#pragma once

#include <Eigen/Core>

namespace ModelMagick {

struct Mesh {
    Eigen::MatrixX3f vertices;
    Eigen::MatrixX3i indices;

    int numVertices() const { return vertices.rows(); }
    int numFaces() const { return indices.rows(); }
};

}  // namespace ModelMagick
