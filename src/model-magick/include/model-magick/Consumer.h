#pragma once

#include <oneapi/tbb.h>

namespace ModelMagick
{

template <typename T>
oneapi::tbb::flow::buffer_node<T> createConsumer(oneapi::tbb::flow::graph& graph)
{
    return oneapi::tbb::flow::buffer_node<T>(graph);
}

}  // namespace ModelMagick
