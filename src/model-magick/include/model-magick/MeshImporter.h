#pragma once

#include <filesystem>
#include <memory>

#include <oneapi/tbb.h>

#include "Mesh.h"

namespace ModelMagick
{

class MeshImporterError : public std::runtime_error {
public:
    MeshImporterError() : std::runtime_error("") {}
};

class MeshOpenError : public MeshImporterError {};

Mesh importMesh(const std::filesystem::path& filePath);

oneapi::tbb::flow::input_node<Mesh> createMeshImporter(
    const std::filesystem::path& filePath,
    oneapi::tbb::flow::graph& graph);

}  // namespace ModelMagick
