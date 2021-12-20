#include <catch2/catch.hpp>

#include <tuple>
#include <vector>
#include <string>

#include <model-magick/MeshImporter.h>

using namespace std;
using namespace oneapi::tbb::flow;
using namespace ModelMagick;

TEST_CASE("Import can load files", "[importer]")
{
    vector<tuple<string, int, int>> samples = {{"assets/box.stl", 24, 12}, {"assets/base.obj", 192, 144}};

    auto i = GENERATE(0, 1);
    auto [filename, nVertices, nFaces] = samples[i];

    SECTION("import function") {
        Mesh mesh = importMesh(filename);
        CHECK(mesh.numVertices() == nVertices);
        CHECK(mesh.numFaces() == nFaces);
    }

    SECTION("import node with fixed filename") {

        graph g;
        auto importer = createMeshImporter(filename, g);
        auto consumer = function_node<Mesh, int>(g, 1, [nVertices=nVertices, nFaces=nFaces](const Mesh& mesh) {
            CHECK(mesh.numVertices() == nVertices);
            CHECK(mesh.numFaces() == nFaces);
            return 0;
        });

        make_edge(importer, consumer);

        importer.activate();
        REQUIRE_NOTHROW(g.wait_for_all());
    }
}

TEST_CASE("Import can throw if the file does not exist", "[importer]")
{
    SECTION("import function") {
        REQUIRE_THROWS_AS(importMesh("NOT-EXISTING-FILE.stl"), MeshOpenError);
    }

    SECTION("import node with fixed filename") {

        graph g;
        auto importer = createMeshImporter("NOT-EXISTING-FILE.stl", g);
        auto consumer = function_node<Mesh, int>(g, 1, [](const Mesh&) {
            return 0;
        });

        make_edge(importer, consumer);

        importer.activate();
        REQUIRE_THROWS_AS(g.wait_for_all(), MeshOpenError);
    }
}
