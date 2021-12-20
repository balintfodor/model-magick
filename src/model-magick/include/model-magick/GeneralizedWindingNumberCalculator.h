#pragma once

#include <filesystem>
#include <memory>

#include <oneapi/tbb.h>

#include "Mesh.h"

namespace ModelMagick
{

Eigen::VectorXf calculateGeneralizedWindingNumber(const Eigen::MatrixX3f& samplers, const Mesh& mesh);

Eigen::MatrixX3f calculateTriangleCenters(const Mesh& mesh);

oneapi::tbb::flow::function_node<Mesh, Eigen::MatrixX3f, Eigen::VectorXf> createGeneralizedWindingNumberCalculator(
    oneapi::tbb::flow::graph& graph,
    std::size_t concurrency = oneapi::tbb::flow::unlimited);

}  // namespace ModelMagick