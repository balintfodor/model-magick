#include "model-magick/MeshStatsCalculator.h"

namespace ModelMagick
{

using namespace std;
using namespace oneapi::tbb;
using namespace oneapi::tbb::flow;

MeshStats calculateMeshStats(const Mesh& mesh)
{
    return MeshStats{mesh.numVertices(), mesh.numFaces()};
}

function_node<Mesh, MeshStats> createMeshStatsCalculator(graph& graph, size_t concurrency)
{
    return function_node<Mesh, MeshStats>(graph, concurrency, calculateMeshStats);
}

}  // namespace ModelMagick
