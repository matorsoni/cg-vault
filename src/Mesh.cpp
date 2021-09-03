#include "Mesh.hpp"

using namespace std;

Mesh::Mesh(const vector<Vertex>& p_vertices,
           const vector<unsigned int>& p_indices)
{
    assert(p_vertices.size() != 0);

    // Copy vertices to the Mesh object.
    vertices.reserve(p_vertices.size());
    for (const auto& v : p_vertices)
        vertices.emplace_back(v);

    // Copy indices to the Mesh object.
    if (p_indices.size() != 0) {
        indices.reserve(p_indices.size());
        for (const auto& ind : p_indices)
            indices.emplace_back(ind);
    }
}
