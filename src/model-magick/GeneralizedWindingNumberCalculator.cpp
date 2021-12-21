#include "model-magick/MeshStatsCalculator.h"

#include <Eigen/Geometry>

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

Eigen::MatrixX3f calculateTriangleNormals(const Mesh& mesh)
{
    // TODO: make sure to remove all 0 area triangles before this

    Eigen::MatrixX3f triangleNormals;
    const int m = mesh.numFaces();
    triangleNormals.resize(m, 3);
    parallel_for(
        blocked_range<size_t>(0, m), [&triangleNormals, &mesh](const blocked_range<size_t>& r) {
            for (size_t i = r.begin(); i != r.end(); ++i) {
                const Eigen::RowVector3f a = mesh.vertices(mesh.indices(i, 0), Eigen::all);
                const Eigen::RowVector3f b = mesh.vertices(mesh.indices(i, 1), Eigen::all);
                const Eigen::RowVector3f c = mesh.vertices(mesh.indices(i, 2), Eigen::all);
                triangleNormals.row(i) = (b - a).cross(c - a);
            }
        });
    return triangleNormals;
}

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
    windingNumbers.resize(numQuery);
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
