#include <catch2/catch.hpp>

#include <string>

#include <model-magick/ConstProvider.h>
#include <model-magick/Consumer.h>

using namespace std;
using namespace oneapi::tbb::flow;
using namespace ModelMagick;

TEST_CASE("ConstProvider can provide a fixed value", "[provider]")
{
    graph g;
    auto provider = createConstProvider<string>(g, "hello");
    auto consumer = createConsumer<string>(g);

    make_edge(provider, consumer);
    provider.activate();
    g.wait_for_all();

    string out;
    CHECK(consumer.try_get(out));
    CHECK(out == "hello");
}
