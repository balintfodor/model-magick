#include <catch2/catch.hpp>

#include <tuple>
#include <vector>
#include <string>

#include <model-magick/MeshStatCalculator.h>

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

    struct MeshProvider {
        MeshProvider(Mesh mesh) : mesh(mesh) {}
        Mesh operator()(tbb::flow_control& fc)
        {
            if (called) {
                fc.stop();
                return Mesh();
            } else {
                called = true;
                return mesh;
            }
        };
        bool called = false;
        const Mesh mesh;
    };

    SECTION("stats node") {

        graph g;
        auto provider = input_node<Mesh>(g, MeshProvider(mesh));
        auto calculator = createMeshStatCalculator(g);
        auto consumer = function_node<MeshStats, int>(g, 1, [nVertices=nVertices, nFaces=nFaces](const MeshStats& meshStats) {
            CHECK(meshStats.numVertices == nVertices);
            CHECK(meshStats.numFaces == nFaces);
            return 0;
        });

        make_edge(provider, calculator);
        make_edge(calculator, consumer);

        provider.activate();
        REQUIRE_NOTHROW(g.wait_for_all());
    }
}
