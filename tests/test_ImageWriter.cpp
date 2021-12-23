#include <catch2/catch.hpp>

#include <cstdio>
#include <filesystem>

#include <model-magick/ImageWriter.h>

using namespace std;
using namespace ModelMagick;

TEST_CASE("Images can be written", "[image-writer]")
{
    Image image;
    image.width = 7;
    image.height = 5;
    image.channels = 4;
    image.pixels.resize(5 * 7 * 4);
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            image.pixels[(j * image.width + i) * image.channels + 0] = i * 30;
            image.pixels[(j * image.width + i) * image.channels + 1] = j * 40;
            image.pixels[(j * image.width + i) * image.channels + 2] = 100;
            image.pixels[(j * image.width + i) * image.channels + 3] = 255;
        }
    }

    filesystem::path filename = "test-out.png";
    if (std::filesystem::exists(filename)) {
        REQUIRE(std::filesystem::remove(filename));
    }
    REQUIRE(writeImage(filename, image));
    REQUIRE(std::filesystem::exists(filename));
}
