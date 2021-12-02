#pragma once

#include <filesystem>
#include <memory>

#include <oneapi/tbb.h>

#include "mesh.h"

namespace ModelMagick
{

Mesh importModel(const std::filesystem::path& filePath);

oneapi::tbb::flow::function_node<std::filesystem::path, Mesh> createModelImporter(
    oneapi::tbb::flow::graph& graph,
    std::size_t concurrency = oneapi::tbb::flow::unlimited);

oneapi::tbb::flow::input_node<Mesh> createModelImporter(
    const std::filesystem::path& filePath,
    oneapi::tbb::flow::graph& graph);

}  // namespace ModelMagick
