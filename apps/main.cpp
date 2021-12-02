#include <iostream>

#include <oneapi/tbb.h>
#include <model-magick/importer.h>

using namespace std;
using namespace oneapi::tbb::flow;
using namespace ModelMagick;

int main()
{
    graph taskGraph;

    auto importer = createModelImporter("fotellab.stl", taskGraph);
    auto consumer = function_node<Mesh, int>(taskGraph, 1, [](const Mesh& mesh) {
        cout << mesh.vertices.rows() << " " << mesh.indices.rows() << "\n";
        return 0;
    });

    make_edge(importer, consumer);

    importer.activate();
    taskGraph.wait_for_all();

    return 0;
}
