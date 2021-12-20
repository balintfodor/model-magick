#pragma once

#include <oneapi/tbb.h>

namespace ModelMagick
{

template <typename T>
oneapi::tbb::flow::input_node<T> createConstProvider(
    oneapi::tbb::flow::graph& graph,
    T valueToProvide)
{
    struct Provider {
        Provider(T value) : value(value) {}
        T operator()(tbb::flow_control& fc)
        {
            if (called) {
                fc.stop();
                return T{};
            } else {
                called = true;
                return value;
            }
        };
        bool called = false;
        const T value;
    };

    return oneapi::tbb::flow::input_node<T>(graph, Provider(valueToProvide));
}

}  // namespace ModelMagick
