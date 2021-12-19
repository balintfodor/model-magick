#pragma once

#include <filesystem>
#include <memory>

#include <oneapi/tbb.h>

#include "mesh.h"

namespace ModelMagick
{

class ImporterError : public std::runtime_error {
public:
    ImporterError() : std::runtime_error("") {}
};

class OpenError : public ImporterError {};

Mesh importModel(const std::filesystem::path& filePath);

oneapi::tbb::flow::input_node<Mesh> createModelImporter(
    const std::filesystem::path& filePath,
    oneapi::tbb::flow::graph& graph);

}  // namespace ModelMagick
