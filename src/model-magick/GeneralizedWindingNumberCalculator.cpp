#include "model-magick/MeshStatsCalculator.h"

namespace ModelMagick
{

using namespace std;
using namespace oneapi::tbb;
using namespace oneapi::tbb::flow;

Eigen::MatrixX3f calculateTriangleCenters(const Mesh& mesh)
{
    Eigen::MatrixX3f triangleCenters;
    const int m = mesh.numFaces();
    triangleCenters.resize(m, 3);
    parallel_for(
        blocked_range<size_t>(0, m), [&triangleCenters, &mesh](const blocked_range<size_t>& r) {
            for (size_t i = r.begin(); i != r.end(); ++i) {
                triangleCenters.row(i)
                    = mesh.vertices(mesh.indices.row(i), Eigen::all).colwise().mean();
            }
        });
    return triangleCenters;
}

Eigen::VectorXf calculateGeneralizedWindingNumber(const Eigen::MatrixX3f&, const Mesh& mesh)
{
    Eigen::MatrixX3f triangleCenters = calculateTriangleCenters(mesh);
    // Eigen::MatrixX3f triangleNormals;

    // one point p
    // winding_sum = 0
    // for all triangle with center c and normal n
    //   winding_sum += normalize(c - p).dot(n)
    // winding_sum /= 4*pi
    return Eigen::VectorXf();
}

// oneapi::tbb::flow::function_node<Mesh, Eigen::MatrixX3f, Eigen::VectorXf>
// createGeneralizedWindingNumberCalculator(
//     oneapi::tbb::flow::graph& graph,
//     std::size_t concurrency = oneapi::tbb::flow::unlimited);

}  // namespace ModelMagick
