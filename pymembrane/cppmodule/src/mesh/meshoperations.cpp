#include "meshoperations.hpp"
#include "edge_flip.hpp"
#include "computegeometry.hpp"
#include "../system/systemclass.hpp"
#include "../box/pbc.hpp"
#include <set>

real MeshOperations::compute_average_edge_length()
{
    real avg_edge_length = 0.0;
    for (int edge_id=0; edge_id<_system.Numedges; edge_id++)
    {
        int v0 = _system.edges[edge_id].i;
        int v1 = _system.edges[edge_id].j;
        avg_edge_length += pymemb::vector_length(pymemb::vector_subtract(_system.vertices[v0].r, _system.vertices[v1].r));
    }
    return avg_edge_length/_system.Numedges;
}

bool MeshOperations::check_if_edge_can_flip(const int& flip_edge_index)
{
    auto can_flip = pymemb::CheckEdgeFlip_lambda(flip_edge_index,
                                               (&_system.faces[0]),
                                               (&_system.vertices[0]),
                                               (&_system.edges[0]),
                                               (&_system.halfedges[0]),
                                               _system.get_box());
    return can_flip;
}

bool MeshOperations::edge_need_flip(int flip_edge_index)
{
    int v0 = _system.edges[flip_edge_index].v0;
    int v1 = _system.edges[flip_edge_index].v1;
    int v2 = _system.edges[flip_edge_index].v2;
    int v3 = _system.edges[flip_edge_index].v3;

    real3 r0 = _system.vertices[v0].r;
    real3 r1 = _system.vertices[v1].r;
    real3 r2 = _system.vertices[v2].r;
    real3 r3 = _system.vertices[v3].r;

    // first check if edge can flip
    real angle_0102 = pymemb::compute_angle_vertex(r0, r1, r2);
    real angle_0203 = pymemb::compute_angle_vertex(r0, r2, r3);
    if (angle_0102 + angle_0203 >= M_PI)
        return false;

    real angle_2120 = pymemb::compute_angle_vertex(r2, r1, r0);
    real angle_2023 = pymemb::compute_angle_vertex(r2, r0, r3);
    if (angle_2120 + angle_2023 >= M_PI)
        return false;

    int f0 = _system.edges[flip_edge_index].face_k;
    int f1 = _system.edges[flip_edge_index].face_l;

    real3 ra = _system.vertices[_system.faces[f0].v1].r;
    real3 rb = _system.vertices[_system.faces[f0].v2].r;
    real3 rc = _system.vertices[_system.faces[f0].v3].r;
    real3 n_f0 = pymemb::compute_normal_triangle_unit(ra, rb, rc);

    ra = _system.vertices[_system.faces[f1].v1].r;
    rb = _system.vertices[_system.faces[f1].v2].r;
    rc = _system.vertices[_system.faces[f1].v3].r;
    real3 n_f1 = pymemb::compute_normal_triangle_unit(ra, rb, rc);

    if (pymemb::vector_dot(n_f0, n_f1) < 0)
        return false;
    
    // this condition is relevant when using surface tension and substrate
    // to avoid flip on triple line
    // comment or delete for other use
    int t0 = _system.vertices[v0].type;
    int t1 = _system.vertices[v1].type;
    int t2 = _system.vertices[v2].type;
    int t3 = _system.vertices[v3].type;
    if (t0 == t2 && (t0 > t1 || t0 > t3))
        return false;
    

    // then check if angles can be improved by flip
    real angle012 = pymemb::compute_angle_vertex(r1, r0, r2);
    real angle210 =  pymemb::compute_angle_vertex(r3, r0, r2);

    return angle012 + angle210 > M_PI;
}

/*
                 v0                          v0         
               / | \                       /   \
              /  |  \                     /     \
             v3  |   v1      -->         v3------v1
              \  |  /                     \     /
               \ | /                       \   /
                 v2                          v2
*/


void MeshOperations::edge_flip(int edge_index)
{
    int he02 = _system.edges[edge_index]._hedge;
    int he20 = _system.halfedges[he02].pair;
    int he21 = _system.halfedges[he02].next;
    int he10 = _system.halfedges[he21].next;
    int he03 = _system.halfedges[he20].next;
    int he32 = _system.halfedges[he03].next;

    int v0 = _system.halfedges[he02].vert_from;
    int v1 = _system.halfedges[he21].vert_to;
    int v2 = _system.halfedges[he02].vert_to;
    int v3 = _system.halfedges[he03].vert_to;

    int f021 = _system.halfedges[he02].face;
    int f203 = _system.halfedges[he20].face;

    //repurposed halfedges
    int he13 = he02;
    _system.halfedges[he13].vert_from = v1;
    _system.halfedges[he13].vert_to = v3;
    int he31 = he20;
    _system.halfedges[he31].vert_from = v3;
    _system.halfedges[he31].vert_to = v1;

    //repurposed faces
    int f132 = f021;
    _system.faces[f132]._hedge = he13;
    _system.faces[f132].v1 = v1;
    _system.faces[f132].v2 = v3;
    _system.faces[f132].v3 = v2;
    int f310 = f203;
    _system.faces[f310]._hedge = he31;
    _system.faces[f310].v1 = v3;
    _system.faces[f310].v2 = v1;
    _system.faces[f310].v3 = v0;

    //repurposed edge
    _system.edges[edge_index].i = v1;
    _system.edges[edge_index].j = v3;
    _system.edges[edge_index].v0 = v1;
    _system.edges[edge_index].v1 = v2;
    _system.edges[edge_index].v2 = v3;
    _system.edges[edge_index].v3 = v0;

    //updating halfedges
    _system.halfedges[he13].next = he32;
    _system.halfedges[he32].prev = he13;
    _system.halfedges[he32].next = he21;
    _system.halfedges[he21].prev = he32;
    _system.halfedges[he21].next = he13;
    _system.halfedges[he13].prev = he21;
    _system.halfedges[he31].next = he10;
    _system.halfedges[he10].prev = he31;
    _system.halfedges[he10].next = he03;
    _system.halfedges[he03].prev = he10;
    _system.halfedges[he03].next = he31;
    _system.halfedges[he31].prev = he03;

    _system.halfedges[he32].face = f132;
    _system.halfedges[he10].face = f310;

    //updating vertices
    _system.vertices[v0]._hedge = he03;
    _system.vertices[v2]._hedge = he21;

    //updating edge
    int edge = _system.halfedges[he10].edge;
    if (_system.edges[edge].v1 == v2)
        _system.edges[edge].v1 = v3;
    if (_system.edges[edge].v3 == v2)
        _system.edges[edge].v3 = v3;

    edge = _system.halfedges[he03].edge;
    if (_system.edges[edge].v1 == v2)
        _system.edges[edge].v1 = v1;
    if (_system.edges[edge].v3 == v2)
        _system.edges[edge].v3 = v1;

    edge = _system.halfedges[he32].edge;
    if (_system.edges[edge].v1 == v0)
        _system.edges[edge].v1 = v1;
    if (_system.edges[edge].v3 == v0)
        _system.edges[edge].v3 = v1;

    edge = _system.halfedges[he21].edge;
    if (_system.edges[edge].v1 == v0)
        _system.edges[edge].v1 = v3;
    if (_system.edges[edge].v3 == v0)
        _system.edges[edge].v3 = v3;
}

void MeshOperations::refine_mesh_edge_flip()
{
    for(int edge_index=0; edge_index<_system.Numedges; edge_index++)
    {
        if (edge_need_flip(edge_index))
            edge_flip(edge_index);
    }
}

/*
void MeshOperations::equiangulation(void)
{
    Equiangulation_kernel(_system.Numedges,
                          (&_system.faces[0]),
                          (&_system.vertices[0]),
                          (&_system.edges[0]),
                          (&_system.halfedges[0]),
                          true
                          );
    for (int flip_edge_index = 0;
         flip_edge_index < _system.Numedges;
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
}*/

void MeshOperations::compute_vertex_connexity(int vertex_index)
{
    _system.vertices[vertex_index].coordination = 0;
    int he = _system.vertices[vertex_index]._hedge;
    int first = he;
    do
    {
        _system.vertices[vertex_index].coordination++;
        int he_pair = _system.halfedges[he].pair;
        he = _system.halfedges[he_pair].next;
    } while (he != first);
}

void MeshOperations::compute_vertices_connexities()
{
    for (int i=0; i<_system.Numvertices; i++)
    {
        compute_vertex_connexity(i);
    }
}



int MeshOperations::create_vertex(real mass)
{
    HE_Vertex<PropertyVertices> vertex;
    vertex.id = _system.Numvertices;
    vertex.mass = mass;
    vertex.type = 1;
    _system.vertices.push_back(vertex);
    _system.Numvertices++;
    return vertex.id;
}

int MeshOperations::create_edge()
{
    HE_Edge<PropertyEdges> e;
    HE_HalfEdge<PropertyEdges> he0;
    HE_HalfEdge<PropertyEdges> he1;
    he0.index = _system.Numhalfedges;
    he1.index = _system.Numhalfedges+1;
    e.id = _system.Numedges;
    he0.pair = he1.index;
    he0.edge = e.id;
    he1.pair = he0.index;
    he1.edge = e.id;
    e._hedge = he0.index;
    _system.edges.push_back(e);
    _system.halfedges.push_back(he0);
    _system.halfedges.push_back(he1);
    _system.Numedges++;
    _system.Numhalfedges += 2;
    return e.id;
}

int MeshOperations::create_face()
{
    HE_Face<PropertyFaces> f;
    f.id = _system.Numfaces;
    _system.faces.push_back(f);
    _system.Numfaces++;
    return f.id;
}

void MeshOperations::delete_vertex(int vertex_index)
{
    // shift all vertices in the array > vertex_index
    for (int vid=vertex_index; vid<_system.Numvertices-1; vid++)
    {
        _system.vertices[vid] = _system.vertices[vid+1];
        _system.vertices[vid].id--;
    }

    // udpating vertices id in edge structure
    for (int edge_id=0; edge_id<_system.Numedges; edge_id++)
    {
        if (_system.edges[edge_id].i > vertex_index)
            _system.edges[edge_id].i--;
        if (_system.edges[edge_id].j > vertex_index)
            _system.edges[edge_id].j--;   
        if (_system.edges[edge_id].v0 > vertex_index)
            _system.edges[edge_id].v0--;
        if (_system.edges[edge_id].v1 > vertex_index)
            _system.edges[edge_id].v1--;
        if (_system.edges[edge_id].v2 > vertex_index)
            _system.edges[edge_id].v2--;
        if (_system.edges[edge_id].v3 > vertex_index)
            _system.edges[edge_id].v3--;
    }

    // udpating vertices id in halfedge structure
    for (int hedge_id=0; hedge_id<_system.Numhalfedges; hedge_id++)
    {
        if (_system.halfedges[hedge_id].vert_from > vertex_index)
            _system.halfedges[hedge_id].vert_from--;
        if (_system.halfedges[hedge_id].vert_to > vertex_index)
            _system.halfedges[hedge_id].vert_to--;
    }

    // updating vertices id in face structure
    for (int face_id=0; face_id<_system.Numfaces; face_id++)
    {
        if (_system.faces[face_id].v1 > vertex_index)
            _system.faces[face_id].v1--;
        if (_system.faces[face_id].v2 > vertex_index)
            _system.faces[face_id].v2--;
        if (_system.faces[face_id].v3 > vertex_index)
            _system.faces[face_id].v3--;
    }

    _system.Numvertices--;
}

void MeshOperations::delete_edge(int edge_index)
{
    int he = _system.edges[edge_index]._hedge;
    int he_pair = _system.halfedges[he].pair;
    if (he < he_pair)
        he_pair--;
    delete_hedge(he);
    delete_hedge(he_pair);

    // shift all edges in the array > edge_index
    for (int edge_id=edge_index; edge_id<_system.Numedges-1; edge_id++)
    {
        _system.edges[edge_id] = _system.edges[edge_id+1];
        _system.edges[edge_id].id--;
    }

    // udpating edges id in halfedge structure
    for (int hedge_id=0; hedge_id<_system.Numhalfedges; hedge_id++)
    {
        if(_system.halfedges[hedge_id].edge > edge_index)
            _system.halfedges[hedge_id].edge--;
    }

    _system.Numedges--;
}

void MeshOperations::delete_face(int face_index)
{
    // shift all faces in the array > face_index
    for (int face_id=face_index; face_id<_system.Numfaces-1; face_id++)
    {
        _system.faces[face_id] = _system.faces[face_id+1];
        _system.faces[face_id].id--;
    }

    // udpating faces id in edge structure
    for (int edge_id=0; edge_id<_system.Numedges; edge_id++)
    {
        if (_system.edges[edge_id].face_k > face_index)
            _system.edges[edge_id].face_k--;
        if (_system.edges[edge_id].face_l > face_index)
            _system.edges[edge_id].face_l--;
    }

    // udpating faces id in halfedge structure
    for (int hedge_id=0; hedge_id<_system.Numhalfedges; hedge_id++)
    {
        if (_system.halfedges[hedge_id].face > face_index)
            _system.halfedges[hedge_id].face--;
    }

    _system.Numfaces--;
}

void MeshOperations::delete_hedge(int hedge_index)
{
    // shift all halfedges in the array > hedge_index
    for (int hedge_id=hedge_index; hedge_id<_system.Numhalfedges-1; hedge_id++)
    {
        _system.halfedges[hedge_id] = _system.halfedges[hedge_id+1];
        _system.halfedges[hedge_id].index--;
    }

    // udpating halfedges id in vertex structure
    for (int vertex_id=0; vertex_id<_system.Numvertices; vertex_id++)
    {
        if (_system.vertices[vertex_id]._hedge > hedge_index)
            _system.vertices[vertex_id]._hedge--;
    }

    // updating halfedges id in edge structure
    for (int edge_id=0; edge_id<_system.Numedges; edge_id++)
    {
        if (_system.edges[edge_id]._hedge > hedge_index)
            _system.edges[edge_id]._hedge--;
    }

    // updating halfedges id in face structure
    for (int face_id=0; face_id<_system.Numfaces; face_id++)
    {
        if (_system.faces[face_id]._hedge > hedge_index)
            _system.faces[face_id]._hedge--;
    }

    _system.Numhalfedges--;

    for (int hedge_id=0; hedge_id<_system.Numhalfedges; hedge_id++)
    {
        if (_system.halfedges[hedge_id].next > hedge_index)
            _system.halfedges[hedge_id].next--;
        if (_system.halfedges[hedge_id].pair > hedge_index)
            _system.halfedges[hedge_id].pair--;
        if (_system.halfedges[hedge_id].prev > hedge_index)
            _system.halfedges[hedge_id].prev--;
    }
}



bool MeshOperations::edge_can_collapse(int edge_index)
{
    int he01 = _system.edges[edge_index]._hedge;
    int v0 = _system.halfedges[he01].vert_from;
    int v1 = _system.halfedges[he01].vert_to;
    // First we check which of the two points will be kept
    // if one of them is higher type, we keep this one
    if (_system.vertices[v0].type < _system.vertices[v1].type)
    {
        he01 = _system.halfedges[he01].pair;
        v0 = _system.halfedges[he01].vert_from;
        v1 = _system.halfedges[he01].vert_to;
    }
    real3 r0 = _system.vertices[v0].r;
    real3 r1 = _system.vertices[v1].r;


    int he = _system.vertices[v0]._hedge;
    int first = he;
    std::set<int> v0_neighbours;
    do
    {
        v0_neighbours.insert(_system.halfedges[he].vert_to);
        he = _system.halfedges[he].pair;
        he = _system.halfedges[he].next;
    } while (he != first);

    int count = 0;
    he = _system.vertices[v1]._hedge;
    first = he;
    do
    {
        if( v0_neighbours.find(_system.halfedges[he].vert_to) != v0_neighbours.end())
            count++;
        he = _system.halfedges[he].pair;
        he = _system.halfedges[he].next;
    } while (he != first);

    if (count != 2)
        return false;

    int stop = _system.halfedges[he01].pair;

    he = _system.halfedges[he01].next;
    he = _system.halfedges[he].pair;
    he = _system.halfedges[he].next;
    do
    {
        int v2 = _system.halfedges[he].vert_to;
        int he_next = _system.halfedges[he].next;
        int v3 = _system.halfedges[he_next].vert_to;
        real3 r2 = _system.vertices[v2].r;
        real3 r3 = _system.vertices[v3].r;

        real3 n123 = pymemb::compute_normal_triangle_unit(r1, r2, r3);
        real3 n023 = pymemb::compute_normal_triangle_unit(r0, r2, r3);

        // a triangle will flip, can't perform edge collapse
        if (pymemb::vector_dot(n123, n023) <= 0)
            return false;

        he = _system.halfedges[he].pair;
        he = _system.halfedges[he].next;
    } while (he != stop);

    return true;
}


/*      Collapsing edge v0v1

*/          

void MeshOperations::edge_case_collapse(int he01, int &del_e1, int &del_e2, int &del_f0, int &del_f1)
{
    int he12 = _system.halfedges[he01].next;
    int he20 = _system.halfedges[he12].next;
    int he10 = _system.halfedges[he01].pair;
    int he03 = _system.halfedges[he10].next;
    int he31 = _system.halfedges[he03].next;
    int he = _system.halfedges[he31].pair;
    int he32 = _system.halfedges[he].next;
    int v0 = _system.halfedges[he01].vert_from;
    int v1 = _system.halfedges[he01].vert_to;
    int v2 = _system.halfedges[he20].vert_from;
    int v3 = _system.halfedges[he03].vert_to;

    del_e1 = _system.halfedges[he12].edge;
    del_e2 = _system.halfedges[he31].edge;
    del_f0 = _system.halfedges[he01].face;
    del_f1 = _system.halfedges[he10].face;
    int f132 = _system.halfedges[he32].face;

    //updating halfedges
    _system.halfedges[he20].next = he03;
    _system.halfedges[he03].prev = he20;
    _system.halfedges[he03].next = he32;
    _system.halfedges[he32].prev = he03;
    _system.halfedges[he32].next = he20;
    _system.halfedges[he20].prev = he32;
    _system.halfedges[he20].face = f132;
    _system.halfedges[he03].face = f132;

    //updating vertices
    _system.vertices[v0]._hedge = he03;
    _system.vertices[v2]._hedge = he20;
    _system.vertices[v3]._hedge = he32;

    //updating face
    _system.faces[f132].v1 = v0;
    _system.faces[f132].v2 = v3;
    _system.faces[f132].v3 = v2;
    _system.faces[f132]._hedge = he32;

    //updating edges
    int e02 = _system.halfedges[he20].edge;
    if (_system.edges[e02].v1 == v1)
        _system.edges[e02].v1 = v3;
    if (_system.edges[e02].v3 == v1)
        _system.edges[e02].v3 = v3;
    int he_pair = _system.halfedges[he20].pair;
    _system.edges[e02].face_k = f132;
    _system.edges[e02].face_l = _system.halfedges[he_pair].face;

    int e03 = _system.halfedges[he03].edge;
    if (_system.edges[e03].v1 == v1)
        _system.edges[e03].v1 = v2;
    if (_system.edges[e03].v3 == v1)
        _system.edges[e03].v3 = v2;
    he_pair = _system.halfedges[he03].pair;
    _system.edges[e03].face_k = f132;
    _system.edges[e03].face_l = _system.halfedges[he_pair].face;

    int e23 = _system.halfedges[he32].edge;
    if (_system.edges[e23].v1 == v1)
        _system.edges[e23].v1 = v0;
    if (_system.edges[e23].v3 == v1)
        _system.edges[e23].v3 = v0;
}


void MeshOperations::edge_collapse(int edge_index, int &del_vertex, int &del_e1,
                                    int &del_e2, int &del_f0, int &del_f1)
{
    int he01 = _system.edges[edge_index]._hedge;
    int v0 = _system.halfedges[he01].vert_from;
    int v1 = _system.halfedges[he01].vert_to;

    // First we check which of the two points will be kept
    // if one of them is higher type, we keep this one
    if (_system.vertices[v0].type < _system.vertices[v1].type)
    {
        he01 = _system.halfedges[he01].pair;
        v0 = _system.halfedges[he01].vert_from;
        v1 = _system.halfedges[he01].vert_to;
    }
    
    del_vertex = v1;
    
    compute_vertex_connexity(v1);
    // dealing with edge case
    if (_system.vertices[v1].coordination == 3)
    {
        edge_case_collapse(he01, del_e1, del_e2, del_f0, del_f1);
        return;
    }

    // Get all ids needed
    del_f0 = _system.halfedges[he01].face;
    int he10 = _system.halfedges[he01].pair;
    del_f1 = _system.halfedges[he10].face;
    int he03 = _system.halfedges[he10].next;
    int he31 = _system.halfedges[he03].next;
    int he12 = _system.halfedges[he01].next;
    int he20 = _system.halfedges[he12].next;
    int he = _system.halfedges[he31].pair;
    int he35 = _system.halfedges[he].next;
    int he51 = _system.halfedges[he35].next;
    he = _system.halfedges[he12].pair;
    int he14 = _system.halfedges[he].next;
    int he42 = _system.halfedges[he14].next;

    del_e1 = _system.halfedges[he31].edge;
    del_e2 = _system.halfedges[he12].edge;

    int v2 =_system.halfedges[he12].vert_to;
    int v3 = _system.halfedges[he03].vert_to;
    int v4 = _system.halfedges[he14].vert_to;
    int v5 = _system.halfedges[he51].vert_from;

    he = he14;
    do
    {
        _system.halfedges[he].vert_from = v0;
        int edge = _system.halfedges[he].edge;
        _system.edges[edge].i = v0;
        _system.edges[edge].j = _system.halfedges[he].vert_to;
        _system.edges[edge].v0 = v0;
        _system.edges[edge].v2 = _system.halfedges[he].vert_to;

        int he_pair = _system.halfedges[he].pair;
        int he_next = _system.halfedges[he].next;
        _system.halfedges[he_pair].vert_to = v0;

        //Updating face
        int face = _system.halfedges[he].face;
        _system.faces[face]._hedge = he_next;
        _system.faces[face].v1 = v0;
        _system.faces[face].v2 = _system.halfedges[he].vert_to;
        _system.faces[face].v3 = _system.halfedges[he_next].vert_to;

        int outer_edge = _system.halfedges[he_next].edge;
        if (_system.edges[outer_edge].v1 == v1)
            _system.edges[outer_edge].v1 = v0;
        if (_system.edges[outer_edge].v3 == v1)
            _system.edges[outer_edge].v3 = v0;

        he = _system.halfedges[he_pair].next;
    } while(he != he10);

    // Updating vertices
    _system.vertices[v2]._hedge = he20;
    _system.vertices[v3]._hedge = he35;
    _system.vertices[v0]._hedge = he03;


    // Updating halfedges
    _system.halfedges[he03].next = he35;
    _system.halfedges[he35].prev = he03;
    _system.halfedges[he51].next = he03;
    _system.halfedges[he03].prev = he51;
    _system.halfedges[he20].next = he14;
    _system.halfedges[he14].prev = he20;

    _system.halfedges[he03].face = _system.halfedges[he35].face;
    _system.halfedges[he42].next = he20;
    _system.halfedges[he20].prev = he42;
    _system.halfedges[he20].face = _system.halfedges[he42].face;

    // Updating outer edges

    int outer_edge = _system.halfedges[he20].edge;
    if (_system.edges[outer_edge].v1 == v1)
        _system.edges[outer_edge].v1 = v4;
    if (_system.edges[outer_edge].v3 == v1)
        _system.edges[outer_edge].v3 = v4;
    he = _system.halfedges[he20].pair;
    _system.edges[outer_edge].face_k = _system.halfedges[he].face;
    _system.edges[outer_edge].face_l = _system.halfedges[he14].face;

    outer_edge = _system.halfedges[he03].edge;
    if (_system.edges[outer_edge].v1 == v1)
        _system.edges[outer_edge].v1 = v5;
    if (_system.edges[outer_edge].v3 == v1)
        _system.edges[outer_edge].v3 = v5;
    he = _system.halfedges[he03].pair;
    _system.edges[outer_edge].face_k = _system.halfedges[he].face;
    _system.edges[outer_edge].face_l = _system.halfedges[he35].face;
    
}

/*      Splitting edge v0v2
         v0                                 v0
       / | \                              / | \
      /  |  \                            /  |  \
    v3   |   v1          -->            v3--v4--v1
      \  |  /                            \  |  /
       \ | /                              \ | /
         v2                                 v2

    Repurpose face : f034 = f032 // f041 = f021
    Repurpose edge : e04 = e02
    Repurpose halfedge : he04 = he02 // he40 = he20
*/

void MeshOperations::edge_split(int edge_index, int v4, int e34, int e14, int e24,
                                int f243, int f142)
{
    // First get all ids needed
    int he02 = _system.edges[edge_index]._hedge;
    int v0 = _system.halfedges[he02].vert_from;
    int v2 = _system.halfedges[he02].vert_to;
    int he21 = _system.halfedges[he02].next;
    int v1 = _system.halfedges[he21].vert_to;
    int he_pair = _system.halfedges[he02].pair;
    int he03 = _system.halfedges[he_pair].next;
    int v3 = _system.halfedges[he03].vert_to;
    int he23 = _system.halfedges[he03].next;
    he23 = _system.halfedges[he23].pair;
    int he01 = _system.halfedges[he21].next;
    he01 = _system.halfedges[he01].pair;

    int he34 = _system.edges[e34]._hedge;
    int he43 = _system.halfedges[he34].pair;
    int he14 = _system.edges[e14]._hedge;
    int he41 = _system.halfedges[he14].pair;
    int he24 = _system.edges[e24]._hedge;
    int he42 = _system.halfedges[he24].pair;
    
    int f021 = _system.halfedges[he02].face;
    int f032 = _system.halfedges[he03].face;

    _system.vertices[v4].r = pymemb::vector_sum(_system.vertices[v0].r, _system.vertices[v2].r);
    aXvec(0.5, _system.vertices[v4].r);
    _system.vertices[v4]._hedge = he43;
    _system.vertices[v4].type = std::min(_system.vertices[v0].type, _system.vertices[v2].type);

    // Setting up topology on new halfedges
    _system.halfedges[he34].vert_from = v3;
    _system.halfedges[he34].vert_to = v4;
    _system.halfedges[he34].next = _system.halfedges[he02].pair;
    _system.halfedges[he34].prev = he03;
    _system.halfedges[he34].face = f032;

    _system.halfedges[he43].vert_from = v4;
    _system.halfedges[he43].vert_to = v3;
    _system.halfedges[he43].next = _system.halfedges[he23].pair;
    _system.halfedges[he43].prev = he24;
    _system.halfedges[he43].face = f243;

    _system.halfedges[he14].vert_from = v1;
    _system.halfedges[he14].vert_to = v4;
    _system.halfedges[he14].next = he42;
    _system.halfedges[he14].prev = he21;
    _system.halfedges[he14].face = f142;

    _system.halfedges[he41].vert_from = v4;
    _system.halfedges[he41].vert_to = v1;
    _system.halfedges[he41].next = _system.halfedges[he01].pair;
    _system.halfedges[he41].prev = he02;
    _system.halfedges[he41].face = f021;

    _system.halfedges[he24].vert_from = v2;
    _system.halfedges[he24].vert_to = v4;
    _system.halfedges[he24].next = he43;
    _system.halfedges[he24].prev = _system.halfedges[he23].pair;
    _system.halfedges[he24].face = f243;

    _system.halfedges[he42].vert_from = v4;
    _system.halfedges[he42].vert_to = v2;
    _system.halfedges[he42].next = he21;
    _system.halfedges[he42].prev = he14;
    _system.halfedges[he42].face = f142;

    // Setting up new edges
    _system.edges[e34].i = v3;
    _system.edges[e34].j = v4;
    _system.edges[e34]._hedge = he34;
    _system.edges[e34].face_k = _system.halfedges[he34].face;
    _system.edges[e34].face_l = _system.halfedges[he43].face;
    _system.edges[e34].v0 = v3;
    _system.edges[e34].v1 = v0;
    _system.edges[e34].v2 = v4;
    _system.edges[e34].v3 = v2;

    _system.edges[e14].i = v1;
    _system.edges[e14].j = v4;
    _system.edges[e14]._hedge = he14;
    _system.edges[e14].face_k = _system.halfedges[he14].face;
    _system.edges[e14].face_l = _system.halfedges[he41].face;
    _system.edges[e14].v0 = v1;
    _system.edges[e14].v1 = v2;
    _system.edges[e14].v2 = v4;
    _system.edges[e14].v3 = v0;

    _system.edges[e24].i = v2;
    _system.edges[e24].j = v4;
    _system.edges[e24]._hedge = he24;
    _system.edges[e24].face_k = _system.halfedges[he24].face;
    _system.edges[e24].face_l = _system.halfedges[he42].face;
    _system.edges[e24].v0 = v2;
    _system.edges[e24].v1 = v3;
    _system.edges[e24].v2 = v4;
    _system.edges[e24].v3 = v1;

    // Setting up new face
    _system.faces[f243].v1 = v2;
    _system.faces[f243].v2 = v4;
    _system.faces[f243].v3 = v3;
    _system.faces[f243]._hedge = he24;

    _system.faces[f142].v1 = v1;
    _system.faces[f142].v2 = v4;
    _system.faces[f142].v3 = v2;
    _system.faces[f142]._hedge = he14;

    // Updating vertices
    _system.vertices[v2]._hedge = he21;

    // Updating repurposed halfedges
    _system.halfedges[he02].vert_to = v4;
    _system.halfedges[he02].next = he41;

    int he = _system.halfedges[he02].pair;
    _system.halfedges[he].vert_from = v4;
    _system.halfedges[he].prev = he34;


    // Updating repurposed edge
    _system.edges[edge_index].i = v0;
    _system.edges[edge_index].j = v4;
    _system.edges[edge_index].v0 = v0;
    _system.edges[edge_index].v1 = v1;
    _system.edges[edge_index].v2 = v4;
    _system.edges[edge_index].v3 = v3;

    // Updating repurposed faces
    _system.faces[f021].v1 = v0;
    _system.faces[f021].v2 = v4;
    _system.faces[f021].v3 = v1;
    _system.faces[f021]._hedge = he02;

    _system.faces[f032].v1 = v0;
    _system.faces[f032].v2 = v3;
    _system.faces[f032].v3 = v4;
    _system.faces[f032]._hedge = he03;
    
    // Updating exterior halfedges
    he = _system.halfedges[he23].pair;
    _system.halfedges[he].face = f243;
    _system.halfedges[he].next = he24;
    _system.halfedges[he].prev = he43;

    _system.halfedges[he21].face = f142;
    _system.halfedges[he21].next = he14;
    _system.halfedges[he21].prev = he42;

    he = _system.halfedges[he01].pair;
    _system.halfedges[he].prev = he41;

    _system.halfedges[he03].next = he34;

    // Updating exterior edges
    int e = _system.halfedges[he23].edge;
    _system.edges[e].face_k = _system.halfedges[he23].face;
    _system.edges[e].face_l = _system.halfedges[he24].face;
    _system.edges[e].v0 = v2;
    he = _system.halfedges[he23].next;
    _system.edges[e].v1 = _system.halfedges[he].vert_to;
    _system.edges[e].v2 = v3;
    _system.edges[e].v3 = v4;

    e = _system.halfedges[he21].edge;
    _system.edges[e].face_k = _system.halfedges[he21].face;
    he = _system.halfedges[he21].pair;
    _system.edges[e].face_l = _system.halfedges[he].face;
    _system.edges[e].v0 = v2;
    he = _system.halfedges[he].next;
    _system.edges[e].v1 = _system.halfedges[he].vert_to;
    _system.edges[e].v2 = v1;
    _system.edges[e].v3 = v4;

    e = _system.halfedges[he01].edge;
    _system.edges[e].face_k = _system.halfedges[he01].face;
    _system.edges[e].face_l = _system.halfedges[he02].face;
    _system.edges[e].v0 = v0;
    _system.edges[e].v1 = v4;
    _system.edges[e].v2 = v1;
    he = _system.halfedges[he01].next;
    _system.edges[e].v3 = _system.halfedges[he].vert_to;

    e = _system.halfedges[he03].edge;
    _system.edges[e].face_k = _system.halfedges[he03].face;
    he = _system.halfedges[he03].pair;
    _system.edges[e].face_l = _system.halfedges[he].face;
    _system.edges[e].v0 = v0;
    _system.edges[e].v1 = v4;
    _system.edges[e].v2 = v3;
    he = _system.halfedges[he].next;
    _system.edges[e].v3 = _system.halfedges[he].vert_to;
}


void MeshOperations::refine_mesh_edge_split(real threshold)
{
    real avg_edge_length = compute_average_edge_length();
    for (int edge_id=0; edge_id<_system.Numedges; edge_id++)
    {
        int v0 = _system.edges[edge_id].i;
        int v1 = _system.edges[edge_id].j;
        real edge_length = pymemb::vector_length(pymemb::vector_subtract(_system.vertices[v0].r, _system.vertices[v1].r));
        if (edge_length/avg_edge_length > threshold)
        {
            int v = create_vertex();
            int e0 = create_edge();
            int e1 = create_edge();
            int e2 = create_edge();
            int f0 = create_face();
            int f1 = create_face();
            edge_split(edge_id, v, e0, e1, e2, f0, f1);
        }
    }    
}

void MeshOperations::simplify_mesh(real threshold)
{
    real avg_edge_length = compute_average_edge_length();
    for (int edge_id=0; edge_id<_system.Numedges; edge_id++)
    {
        int v0 = _system.edges[edge_id].i;
        int v1 = _system.edges[edge_id].j;

        real edge_length = pymemb::vector_length(pymemb::vector_subtract(_system.vertices[v0].r, _system.vertices[v1].r));
        if (edge_length/avg_edge_length < threshold)
        {
            int del_v, del_e1, del_e2, del_f0, del_f1;
            edge_collapse(edge_id, del_v, del_e1, del_e2, del_f0, del_f1);
            delete_vertex(del_v);
            _system.vertices.resize(_system.Numvertices);
            delete_edge(del_e1);
            if (del_e1 < del_e2)
                del_e2--;
            if (del_e1 < edge_id)
                edge_id--;
            delete_edge(del_e2);
            if (del_e2 < edge_id)
                edge_id--;
            delete_edge(edge_id);
            _system.halfedges.resize(_system.Numhalfedges);
            _system.edges.resize(_system.Numedges);
            delete_face(del_f0);
            if (del_f0 < del_f1)
                del_f1--;
            delete_face(del_f1);
            _system.faces.resize(_system.Numfaces);
        }
    }
}

void MeshOperations::adapt_mesh(real threshold)
{
    real avg_edge_length = compute_average_edge_length();

    std::vector<std::pair<real, int>> sorted_edges;
    for (int edge_id=0; edge_id<_system.Numedges; edge_id++)
    {
        real3 a = _system.vertices[_system.edges[edge_id].i].r;
        real3 b = _system.vertices[_system.edges[edge_id].j].r;

        std::pair<real, int> len_edge(pymemb::vector_length(pymemb::vector_subtract(a, b)), edge_id);
        sorted_edges.push_back(len_edge);
    }
    std::sort(sorted_edges.begin(), sorted_edges.end());

    
    int i=0;
    int j=sorted_edges.size()-1;
    while (i < j)
    {
        real min_ratio = sorted_edges[i].first/avg_edge_length;
        real max_ratio = sorted_edges[j].first/avg_edge_length;

        if (min_ratio < threshold || max_ratio > (1/threshold))
        {
            if (edge_can_collapse(sorted_edges[i].second))
            {
                int del_e1, del_e2, del_v, del_f0, del_f1;
                edge_collapse(sorted_edges[i].second, del_v, del_e1, del_e2, del_f0, del_f1);
                
                while (del_e1 == sorted_edges[j].second || del_e2 == sorted_edges[j].second)
                    j--;

                edge_split(sorted_edges[j].second, del_v, sorted_edges[i].second, del_e1, del_e2, del_f0, del_f1);
            }
            else
                j++;       
        }

        else
            break;
        i++;
        j--;
    }

}
