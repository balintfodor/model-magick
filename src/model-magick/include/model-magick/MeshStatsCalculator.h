#pragma once

#include <filesystem>
#include <memory>

#include <oneapi/tbb.h>

#include "Mesh.h"

namespace ModelMagick
{

struct MeshStats {
    int numVertices = -1;
    int numFaces = -1;
};

MeshStats calculateMeshStats(const Mesh& mesh);

oneapi::tbb::flow::function_node<Mesh, MeshStats> createMeshStatsCalculator(
    oneapi::tbb::flow::graph& graph,
    std::size_t concurrency = oneapi::tbb::flow::unlimited);

}  // namespace ModelMagick
