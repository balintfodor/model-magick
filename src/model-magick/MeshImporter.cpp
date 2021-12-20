#include "model-magick/MeshImporter.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

namespace ModelMagick
{

using namespace std;
using namespace std::filesystem;
using namespace oneapi::tbb;
using namespace oneapi::tbb::flow;

Mesh importMesh(const path& filePath)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(
        filePath.string(), aiProcess_Triangulate | aiProcess_RemoveComponent
                               | aiProcess_JoinIdenticalVertices | aiProcess_PreTransformVertices);

    if (!scene) {
        throw MeshOpenError();
    }

    // TODO: handle scene->mNumMeshes != 1

    const aiMesh* importedMesh = scene->mMeshes[0];

    Mesh newMesh;
    const int n = importedMesh->mNumVertices;
    const int m = importedMesh->mNumFaces;
    newMesh.vertices = Eigen::MatrixX3f(n, 3);
    newMesh.indices = Eigen::MatrixX3i(m, 3);

    for (int i = 0; i < n; ++i) {
        aiVector3D& v = importedMesh->mVertices[i];
        newMesh.vertices.row(i) = Eigen::RowVector3f(v.x, v.y, v.z);
    }

    for (int j = 0; j < m; ++j) {
        aiFace& f = importedMesh->mFaces[j];
        // TODO: check f.mNumIndices == 3
        newMesh.indices.row(j) = Eigen::RowVector3i(f.mIndices[0], f.mIndices[1], f.mIndices[2]);
    }

    // importer dtor will cleanup scene

    return newMesh;
}

input_node<Mesh> createMeshImporter(
    const std::filesystem::path& filePath,
    oneapi::tbb::flow::graph& graph)
{
    struct ImporterInput {
        ImporterInput(std::filesystem::path filePath) : filePath(filePath) {}
        Mesh operator()(flow_control& fc)
        {
            if (called) {
                fc.stop();
                return Mesh();
            } else {
                called = true;
                return importMesh(filePath);
            }
        };
        bool called = false;
        const std::filesystem::path filePath;
    };

    return input_node<Mesh>(graph, ImporterInput(filePath));
}

}  // namespace ModelMagick
