#include "meshoperations.hpp"
#include "edge_flip.hpp"
#include "computegeometry.hpp"
#include "../system/systemclass.hpp"
#include "../box/pbc.hpp"

bool MeshOperations::check_if_edge_can_flip(int flip_edge_index)
{
    auto can_flip = pymemb::CheckEdgeFlip_lambda(flip_edge_index,
                                               (&_system.faces[0]),
                                               (&_system.vertices[0]),
                                               (&_system.edges[0]),
                                               (&_system.halfedges[0]),
                                               _system.get_box());
    return can_flip;
}

void MeshOperations::edge_flip(const int& flip_edge_index, const bool& flip_face_up)
{

    pymemb::EdgeFlip_lambda(flip_edge_index,
                               flip_face_up,
                               &_system.faces[0],
                              &_system.vertices[0],
                               &_system.edges[0],
                               &_system.halfedges[0],
                               _system.get_box());

}

void MeshOperations::equiangulation(void)
{
    Equiangulation_kernel(_system.Numedges,
                          (&_system.faces[0]),
                          (&_system.vertices[0]),
                          (&_system.edges[0]),
                          (&_system.halfedges[0]),
                          true,
                          );
    for (int flip_edge_index = 0;
         flip_edge_index < Numedges;
         flip_edge_index += 1)
    {
        // retrieve the vertices around the edge
        int v0 = _system.edges[flip_edge_index].v0;
        int v1 = _system.edges[flip_edge_index].v1;
        int v2 = _system.edges[flip_edge_index].v2;
        int v3 = _system.edges[flip_edge_index].v3;

        auto need_flip = pymemb::is_equiangular(_system.vertices[v0].r, 
                                                _system.vertices[v1].r, 
                                                _system.vertices[v2].r, 
                                                _system.vertices[v3].r, 
                                                _system.get_box());

        if (!need_flip)
        {
            pymemb::EdgeFlip_lambda(flip_edge_index,
                                    true,
                                    &_system.faces[0],
                                    &_system.vertices[0],
                                    &_system.edges[0],
                                    &_system.halfedges[0],
                                    _system.get_box());
            py::print("flipped edge =%i\n", flip_edge_index);
        }
    }
}
