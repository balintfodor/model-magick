#include "model-magick/ImageWriter.h"

#include <stdio.h>

#include <lodepng.h>

namespace ModelMagick
{

using namespace std;
using namespace std::filesystem;
using namespace oneapi::tbb;
using namespace oneapi::tbb::flow;

bool writeImage(const std::filesystem::path& filename, const Image& image)
{
    unsigned error = lodepng::encode(filename.string(), image.pixels, image.width, image.height);

    if (error) {
        // TODO: use lodepng_error_text(error)
        throw ImageWriteError();
        return false;
    }
    return true;
}

function_node<Image, bool> createMeshImporter(
    std::filesystem::path filename,
    oneapi::tbb::flow::graph& graph,
    std::size_t concurrency)
{
    return function_node<Image, bool>(
        graph, concurrency, [filename](const Image& image) { return writeImage(filename, image); });
}

}  // namespace ModelMagick
