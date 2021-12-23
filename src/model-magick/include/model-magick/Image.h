#pragma once

#include <cstdint>
#include <vector>

namespace ModelMagick
{

struct Image {
    std::vector<uint8_t> pixels;

    int width = 0;
    int height = 0;
    int channels = 4;
};

}  // namespace ModelMagick
