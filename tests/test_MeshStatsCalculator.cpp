#include <catch2/catch.hpp>

#include <tuple>
#include <vector>
#include <string>

#include <model-magick/ConstProvider.h>
#include <model-magick/Consumer.h>
#include <model-magick/MeshStatsCalculator.h>

using namespace std;
using namespace oneapi::tbb::flow;
using namespace ModelMagick;

TEST_CASE("Stats function and node can print mesh stats", "[stats]")
{
    const vector<tuple<Mesh, int, int>> meshes = []() {
        vector<tuple<Mesh, int, int>> result;

        Mesh emptyMesh;
        Mesh mesh;

        mesh.vertices.resize(42, 3);
        mesh.indices.resize(10, 3);

        result.push_back({emptyMesh, 0, 0});
        result.push_back({mesh, 42, 10});

        return result;
    }();

    auto i = GENERATE(0, 1);
    const auto [mesh, nVertices, nFaces] = meshes[i];

    SECTION("stats function") {
        MeshStats stats = calculateMeshStats(mesh);
        CHECK(stats.numVertices == nVertices);
        CHECK(stats.numFaces == nFaces);
    }

    SECTION("stats node") {

        graph g;
        auto provider = createConstProvider<Mesh>(g, mesh);
        auto calculator = createMeshStatsCalculator(g);
        auto consumer = createConsumer<MeshStats>(g);

        make_edge(provider, calculator);
        make_edge(calculator, consumer);

        provider.activate();
        g.wait_for_all();

        MeshStats resultStats;
        CHECK(consumer.try_get(resultStats));
        CHECK(resultStats.numVertices == nVertices);
        CHECK(resultStats.numFaces == nFaces);
    }
}
