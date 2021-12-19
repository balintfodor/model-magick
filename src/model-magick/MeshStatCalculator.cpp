#include "model-magick/MeshStatCalculator.h"

namespace ModelMagick
{

using namespace std;
using namespace oneapi::tbb;
using namespace oneapi::tbb::flow;

MeshStat calculateMeshStats(const Mesh& mesh)
{
    return MeshStat{mesh.numVertices(), mesh.numFaces()};
}

function_node<Mesh, MeshStats>
createMeshStatCalculator(const Mesh& mesh, graph& graph, size_t concurrency)
{
    return function_node<Mesh, MeshStats>(graph, concurrency, calculateMeshStats);
}

}  // namespace ModelMagick
