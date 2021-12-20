#include <catch2/catch.hpp>

#include <tuple>
#include <vector>
#include <string>

#include <model-magick/ConstProvider.h>

using namespace std;
using namespace oneapi::tbb::flow;
using namespace ModelMagick;

TEST_CASE("ConstProvider can provide a fixed value", "[provider]")
{
    graph g;
    auto provider = createConstProvider<std::string>(g, "hello");
    auto consumer = function_node<std::string, bool>(g, 1, [](const std::string& value) {
        CHECK(value == "hello");
        return true;
    });

    make_edge(provider, consumer);
    provider.activate();
    g.wait_for_all();
}
