#include <catch2/catch.hpp>

#include <string>
#include <tuple>
#include <vector>

#include <model-magick/ConstProvider.h>
#include <model-magick/Consumer.h>
#include <model-magick/GeneralizedWindingNumberCalculator.h>

using namespace std;
using namespace oneapi::tbb::flow;
using namespace Eigen;
using namespace ModelMagick;

TEST_CASE("Triangle centers can be calculated", "[generalized-winding-number]")
{
    const vector<tuple<Mesh, MatrixX3f>> testSamples = []() {
        vector<tuple<Mesh, MatrixX3f>> result;

        {
            Mesh emptyMesh;
            result.push_back({emptyMesh, MatrixX3f()});
        }
        {
            Mesh mesh1;
            mesh1.vertices.resize(3, 3);
            mesh1.vertices << 0, 0, 0, 1, 0.1, 0.01, 5, 0.5, 0.05;
            mesh1.indices.resize(1, 3);
            mesh1.indices << 0, 1, 2;

            MatrixX3f expected1(1, 3);
            expected1 << 2, 0.2, 0.02;
            result.push_back({mesh1, expected1});
        }
        {
            Mesh mesh2;
            mesh2.vertices.resize(4, 3);
            mesh2.vertices << 0, 0, 0, 1, 0.1, 0.01, 5, 0.5, 0.05, 8, 0.8, 0.08;
            mesh2.indices.resize(2, 3);
            mesh2.indices << 0, 1, 2, 0, 1, 3;

            MatrixX3f expected2(2, 3);
            expected2 << 2, 0.2, 0.02, 3, 0.3, 0.03;
            result.push_back({mesh2, expected2});
        }

        return result;
    }();

    const int numTests = testSamples.size();
    auto i = GENERATE_COPY(range(0, numTests));
    const auto [mesh, expectedResult] = testSamples[i];

    const auto result = calculateTriangleCenters(mesh);
    CAPTURE(result, expectedResult);
    CHECK(result.isApprox(expectedResult));
}
