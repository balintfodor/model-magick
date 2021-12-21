#include <catch2/catch.hpp>

#include <string>
#include <tuple>
#include <vector>

#include <model-magick/ConstProvider.h>
#include <model-magick/Consumer.h>
#include <model-magick/GeneralizedWindingNumberCalculator.h>
#include <model-magick/MeshImporter.h>

using namespace std;
using namespace Eigen;
using namespace oneapi::tbb::flow;
using namespace ModelMagick;

TEST_CASE("Solid angles can be calculated", "[generalized-winding-number]")
{
    const vector<tuple<Mesh, VectorXf>> testSamples = []() {
        vector<tuple<Mesh, VectorXf>> result;

        {
            Mesh emptyMesh;
            result.push_back({emptyMesh, VectorXf()});
        }
        {
            Mesh mesh1;
            mesh1.vertices.resize(5, 3);
            mesh1.vertices << 1, 0, 0,  // x
                0, 1, 0,  // y
                0, 0, 1,  // z
                2, 0, 0,  // 2 * x
                0.5, 0.5, 0;  // 45 deg x-y
            mesh1.indices.resize(5, 3);
            mesh1.indices << 0, 1, 2,  // positive facing triangle
                3, 1, 2,  // bigger positive facing triangle
                1, 0, 2,  // negative facing triangle
                4, 1, 2,  // smaller triangle
                0, 0, 0;  // zero area triangle

            VectorXf expected1(5, 1);
            const float sphereAreaOctant = 4.0 * M_PI / 8.0;
            expected1 << sphereAreaOctant, sphereAreaOctant, -sphereAreaOctant,
                0.5 * sphereAreaOctant, 0;
            result.push_back({mesh1, expected1});
        }

        {
            Mesh mesh2;
            mesh2.vertices.resize(3, 3);
            // triangle cutting the sphere in half
            mesh2.vertices << 5, 0, 0,  //
                0, 5, 0,  //
                -5, -5, 0;  //
            mesh2.indices.resize(1, 3);
            mesh2.indices << 0, 1, 2;

            VectorXf expected2(1, 1);
            expected2 << 2 * M_PI;
            result.push_back({mesh2, expected2});
        }

        return result;
    }();

    const int numTests = testSamples.size();
    auto i = GENERATE_COPY(range(0, numTests));
    const auto [mesh, expectedResult] = testSamples[i];

    // TODO: test with different query point
    const auto result = calculateSolidAnglesForPlanarTriangles(mesh, {0, 0, 0});
    CAPTURE(result, expectedResult);
    CHECK(result.isApprox(expectedResult));
}

TEST_CASE("Winding number works on empty mesh", "[generalized-winding-number]")
{
    const Mesh mesh;
    MatrixX3f queries(1, 3);
    queries << 0, 0, 0;
    const VectorXf result = calculateGeneralizedWindingNumber(mesh, queries);
    REQUIRE(result.size() == 1);
    CHECK(result(0) == Approx(0));
}

TEST_CASE("Winding numbers can be calculated for closed mesh", "[generalized-winding-number]")
{
    const Mesh mesh = importMesh("assets/unit-box.stl");
    MatrixX3f queries(2, 3);
    queries << 0, 0, 0,  // inside
        2, 2, 2;  // outside
    const VectorXf result = calculateGeneralizedWindingNumber(mesh, queries);
    REQUIRE(result.size() == 2);
    CHECK(result(0) == Approx(1));
    CHECK_THAT(result(1), Catch::Matchers::WithinAbs(0, 0.000001));
}

TEST_CASE("Winding numbers can be calculated for non-closed mesh", "[generalized-winding-number]")
{
    const Mesh mesh = importMesh("assets/opened-unit-box.stl");
    MatrixX3f queries(2, 3);
    queries << 0, 0, 0,  // inside
        2, 2, 2;  // outside
    const VectorXf result = calculateGeneralizedWindingNumber(mesh, queries);
    REQUIRE(result.size() == 2);
    CHECK(result(0) > 0.5);
    CHECK(result(0) < 1);
    CHECK(result(1) > 0);
    CHECK(result(1) < 0.5);
}
