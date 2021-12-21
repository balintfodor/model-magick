#include "model-magick/MeshStatsCalculator.h"

#include <cmath>

#include <Eigen/Geometry>

namespace ModelMagick
{

using namespace std;
using namespace oneapi::tbb;
using namespace oneapi::tbb::flow;

Eigen::VectorXf calculateSolidAnglesForPlanarTriangles(
    const Mesh& mesh,
    const Eigen::RowVector3f& queryPoint)
{
    // https://igl.ethz.ch/projects/winding-number/robust-inside-outside-segmentation-using-generalized-winding-numbers-siggraph-2013-jacobson-et-al.pdf
    // Eq. 6.

    Eigen::VectorXf solidAngles;
    const int m = mesh.numFaces();
    solidAngles.resize(m);
    parallel_for(
        blocked_range<size_t>(0, m),
        [&solidAngles, &mesh, &queryPoint](const blocked_range<size_t>& r) {
            for (size_t i = r.begin(); i != r.end(); ++i) {
                // triangle points in 3x3 matrix
                //       [a1, a2, a3]
                // abc = [b1, b2, b3]
                //       [c1, c2, c3]
                const Eigen::Matrix3f abc
                    = mesh.vertices(mesh.indices(i, Eigen::all), Eigen::all).rowwise() - queryPoint;
                // row norms in column vector
                const Eigen::Vector3f normAbc = abc.rowwise().norm();
                const float d0 = normAbc.prod();
                const float d1 = abc.row(0).dot(abc.row(1)) * normAbc(2);
                const float d2 = abc.row(1).dot(abc.row(2)) * normAbc(0);
                const float d3 = abc.row(0).dot(abc.row(2)) * normAbc(1);
                solidAngles(i) = 2 * std::atan2(abc.determinant(), d0 + d1 + d2 + d3);
            }
        });
    return solidAngles;
}

Eigen::VectorXf calculateGeneralizedWindingNumber(
    const Mesh& mesh,
    const Eigen::MatrixX3f& queryPoints)
{
    Eigen::VectorXf windingNumbers;
    const int numQuery = queryPoints.rows();
    windingNumbers.resize(numQuery, 1);
    parallel_for(
        blocked_range<size_t>(0, numQuery),
        [&windingNumbers, &mesh, &queryPoints](const blocked_range<size_t>& r) {
            for (size_t i = r.begin(); i != r.end(); ++i) {
                windingNumbers(i)
                    = 0.25 * M_1_PI
                      * calculateSolidAnglesForPlanarTriangles(mesh, queryPoints.row(i)).sum();
            }
        });
    return windingNumbers;
}

// oneapi::tbb::flow::function_node<Mesh, Eigen::MatrixX3f, Eigen::VectorXf>
// createGeneralizedWindingNumberCalculator(
//     oneapi::tbb::flow::graph& graph,
//     std::size_t concurrency = oneapi::tbb::flow::unlimited);

}  // namespace ModelMagick
