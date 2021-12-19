#include <catch2/catch.hpp>

#include <tuple>
#include <vector>
#include <string>

#include <model-magick/stats.h>

using namespace std;
using namespace oneapi::tbb::flow;
using namespace ModelMagick;

TEST_CASE("Stats function and node can print mesh stats", "[stats]")
{
    Mesh mesh;

    SECTION("stats function") {
        MeshStats stats = calculateMeshStats(filename);
        CHECK(stats.numVertices() == 0);
        CHECK(stats.numFaces() == 0);
    }

    SECTION("stats node") {

        graph g;
        auto provider = createProvider<Mesh>(mesh);
        auto calculator = createMeshStatCalculator(g);
        auto consumer = function_node<Mesh, int>(g, 1, [](const MeshStats& meshStats) {
            CHECK(meshStats.numVertices == 0);
            CHECK(meshStats.numFaces == 0);
            return 0;
        });

        make_edge(provider, calculator);
        make_edge(calculator, consumer);

        provider.activate();
        REQUIRE_NOTHROW(g.wait_for_all());
    }
}
