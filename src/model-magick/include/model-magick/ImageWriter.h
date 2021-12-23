#pragma once

#include <filesystem>

#include <oneapi/tbb.h>

#include "Image.h"

namespace ModelMagick
{

class ImageWriteError : public std::runtime_error
{
public:
    ImageWriteError() : std::runtime_error("") {}
};

bool writeImage(const std::filesystem::path& filename, const Image& image);

oneapi::tbb::flow::function_node<Image, bool> createImageWriter(
    const std::filesystem::path& filename,
    oneapi::tbb::flow::graph& graph,
    std::size_t concurrency = oneapi::tbb::flow::unlimited);

}  // namespace ModelMagick
