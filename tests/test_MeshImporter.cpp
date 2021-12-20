#include <catch2/catch.hpp>

#include <tuple>
#include <vector>
#include <string>

#include <model-magick/ConstProvider.h>
#include <model-magick/Consumer.h>
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
        auto filenameProvider = createConstProvider<filesystem::path>(g, filename);
        auto importer = createMeshImporter(g);
        auto consumer = createConsumer<Mesh>(g);

        make_edge(filenameProvider, importer);
        make_edge(importer, consumer);

        filenameProvider.activate();
        g.wait_for_all();

        Mesh resultMesh;
        CHECK(consumer.try_get(resultMesh));
        CHECK(resultMesh.numVertices() == nVertices);
        CHECK(resultMesh.numFaces() == nFaces);
    }
}

TEST_CASE("Import can throw if the file does not exist", "[importer]")
{
    SECTION("import function") {
        REQUIRE_THROWS_AS(importMesh("NOT-EXISTING-FILE.stl"), MeshOpenError);
    }

    SECTION("import node with fixed filename") {

        graph g;
        auto filenameProvider = createConstProvider<filesystem::path>(g, "NOT-EXISTING-FILE.stl");
        auto importer = createMeshImporter(g);
        auto consumer = createConsumer<Mesh>(g);

        make_edge(filenameProvider, importer);
        make_edge(importer, consumer);

        filenameProvider.activate();
        REQUIRE_THROWS_AS(g.wait_for_all(), MeshOpenError);
    }
}
