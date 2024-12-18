#include "montercarlo_flip_edge.hpp"

bool MonteCarloIntegratorFlipEdge::edge_flip(int flip_edge_index, bool flip_face_up)
{
    int v0 = _system.edges[flip_edge_index].v0;
    int v1 = _system.edges[flip_edge_index].v1;
    int v2 = _system.edges[flip_edge_index].v2;
    int v3 = _system.edges[flip_edge_index].v3;

    int face_k = _system.edges[flip_edge_index].face_k;
    int face_l = _system.edges[flip_edge_index].face_l;

    //(0) Check that the edge is not a boundary edge
    if (_system.edges[flip_edge_index].boundary == true)
        return false;

    //(1) first we need to check if we don't destroy the triangulation by flipping the edge.
    // This can be done by checking that the connectivity of the edges v0, v2 of the involve edge is
    // strictly larger than 3.
    int he, first, he_pair, he_pair_next;
    int n_neighbors_v0 = 0, n_neighbors_v2 = 0;

    //check v0
    he = _system.vertices[v0]._hedge;
    first = he;

    do
    {
        n_neighbors_v0++;
        // MOVE TO THE NEXT FACE
        he_pair = _system.halfedges[he].pair;
        he_pair_next = _system.halfedges[he_pair].next;
        he = he_pair_next;
    } while ((he != first));

    //check v2
    he = _system.vertices[v2]._hedge;
    first = he;

    do
    {
        n_neighbors_v2++;
        // MOVE TO THE NEXT FACE
        he_pair = _system.halfedges[he].pair;
        he_pair_next = _system.halfedges[he_pair].next;
        he = he_pair_next;
    } while ((he != first));

    if (n_neighbors_v0 <= 4 || n_neighbors_v2 <= 4)
        return false;

    ///(2) Check that the vertices v0 and v2 do not take part of an obtuse triangles.
    // If that happens then an edge flip might destroy the triangulation.
    // If the sum of angles next to each of the two sides of the edge is >= PI
    // the flip would break triangulation. We disallow such a move.
    // For example, in this case edge cannot be flipped
    //          1:X
    //          * *
    //        *  *
    //      *   * 
    //   0:X---X:2 <-- This angle is < PI and the edge cannot be flipped! 
    //      *   *
    //       *  *
    //         * *
    //          3:X
    real3 r02, r01, r03;
    vsub(r02, _system.vertices[v2].r, _system.vertices[v0].r);
    vsub(r03, _system.vertices[v3].r, _system.vertices[v0].r);
    vsub(r01, _system.vertices[v1].r, _system.vertices[v0].r);
    real r02_norm = sqrt(vdot(r02, r02));
    real r03_norm = sqrt(vdot(r03, r03));
    real r01_norm = sqrt(vdot(r01, r01));
    real angle_r01_r02 = acos(vdot(r01, r02) / (r01_norm * r02_norm));
    real angle_r03_r02 = acos(vdot(r03, r02) / (r03_norm * r02_norm));
    if ((angle_r01_r02 + angle_r03_r02) >= defPI)
        return false;

    real3 r20, r21, r23;
    vsub(r20, _system.vertices[v0].r, _system.vertices[v2].r);
    vsub(r23, _system.vertices[v3].r, _system.vertices[v2].r);
    vsub(r21, _system.vertices[v1].r, _system.vertices[v2].r);
    real r23_norm = sqrt(vdot(r23, r23));
    real r21_norm = sqrt(vdot(r21, r21));
    real angle_r21_r20 = acos(vdot(r21, r20) / (r21_norm * r02_norm));
    real angle_r23_r20 = acos(vdot(r23, r20) / (r23_norm * r02_norm));
    if ((angle_r21_r20 + angle_r23_r20) >= defPI)
        return false;

    //update the halfedges
    int he_vec[6]; //he01, he12, he20, he02, he23, he30;
    int he_edge = _system.edges[flip_edge_index]._hedge;
    if (_system.halfedges[he_edge].vert_from == v0)
    {
        he_vec[3] = he_edge;                           //he02
        he_vec[2] = _system.halfedges[he_vec[3]].pair; //he20
    }
    else
    {
        he_vec[2] = he_edge;                           //he20
        he_vec[3] = _system.halfedges[he_vec[2]].pair; //he02
    }
    he_vec[0] = _system.halfedges[he_vec[2]].next; //he01
    he_vec[1] = _system.halfedges[he_vec[2]].prev; //he12
    he_vec[4] = _system.halfedges[he_vec[3]].next; //he23
    he_vec[5] = _system.halfedges[he_vec[3]].prev; //he30

    //deal with the from, to vertices in the "new" flip edge
    _system.halfedges[he_vec[2]].vert_from = v3;
    _system.halfedges[he_vec[2]].vert_to = v1;
    _system.halfedges[he_vec[3]].vert_from = v1;
    _system.halfedges[he_vec[3]].vert_to = v3;

    //flip face: this stage is crucial if we want to make the faces diffuse
    int new_face_k = face_k;
    int new_face_l = face_l;
    if (flip_face_up == false) //counter-clock wise
    {
        new_face_k = face_l;
        new_face_l = face_k;
    }
    //deal with the references inside of the halfedges
    //he12 > h23 > h20
    _system.halfedges[he_vec[1]].next = he_vec[4];
    _system.halfedges[he_vec[1]].prev = he_vec[2];
    _system.halfedges[he_vec[4]].next = he_vec[2];
    _system.halfedges[he_vec[4]].prev = he_vec[1];
    _system.halfedges[he_vec[2]].next = he_vec[1];
    _system.halfedges[he_vec[2]].prev = he_vec[4];
    //faces
    _system.halfedges[he_vec[1]].face = new_face_k;
    _system.halfedges[he_vec[4]].face = new_face_k;
    _system.halfedges[he_vec[2]].face = new_face_k;

    //he01 > h02 > h30
    _system.halfedges[he_vec[0]].next = he_vec[3];
    _system.halfedges[he_vec[0]].prev = he_vec[5];
    _system.halfedges[he_vec[3]].next = he_vec[5];
    _system.halfedges[he_vec[3]].prev = he_vec[0];
    _system.halfedges[he_vec[5]].next = he_vec[0];
    _system.halfedges[he_vec[5]].prev = he_vec[3];
    //faces
    _system.halfedges[he_vec[5]].face = new_face_l;
    _system.halfedges[he_vec[0]].face = new_face_l;
    _system.halfedges[he_vec[3]].face = new_face_l;

    //deal with the faces and references to the halfedges
    //face_k
    _system.faces[new_face_k].v1 = v1;
    _system.faces[new_face_k].v2 = v2;
    _system.faces[new_face_k].v3 = v3;
    _system.faces[new_face_k]._hedge = he_vec[2];
    //face_l
    _system.faces[new_face_l].v1 = v3;
    _system.faces[new_face_l].v2 = v0;
    _system.faces[new_face_l].v3 = v1;
    _system.faces[new_face_l]._hedge = he_vec[3];

    //deal with the halfedge references to the vertices
    if (_system.vertices[v0]._hedge == he_vec[3] /*he02*/)
        _system.vertices[v0]._hedge = he_vec[0]; /*h01*/
    if (_system.vertices[v2]._hedge == he_vec[2] /*he20*/)
        _system.vertices[v2]._hedge = he_vec[4]; /*h23*/

    //deal with the i,j,v0,v1,v2,v3 in the edge list
    for (auto he_index : he_vec)
    {
        HE_HalfEdgeProp he = _system.halfedges[he_index];
        HE_HalfEdgeProp he_pair = _system.halfedges[_system.halfedges[he_index].pair];
        int edge_index = he.edge;
        _system.edges[edge_index].face_k = he.face;      //!< index to one of the faces shared by this edge -1 if is outer face
        _system.edges[edge_index].face_l = he_pair.face; //!< index to one of the faces shared by this edge -1 if is outer face
        _system.edges[edge_index].v0 = he.vert_to;
        _system.edges[edge_index].v2 = he.vert_from;
        _system.edges[edge_index].j = he.vert_to;
        _system.edges[edge_index].i = he.vert_from;
        int he_next = he.next;
        _system.edges[edge_index].v1 = _system.halfedges[he_next].vert_to;
        int he_index_pair_next = he_pair.next;
        _system.edges[edge_index].v3 = _system.halfedges[he_index_pair_next].vert_to;
    }
    return true;
}

int MonteCarloIntegratorFlipEdge::integrate(void)
{
    int accepted_moves = 0;
    // std::copy(freezed_edge.begin(), freezed_edge.end(), std::ostream_iterator<bool>(std::cout, ","));
    for (int edge_index = 0; edge_index < _system.Numedges; edge_index++)
    // for (int ii = 0; ii < 10; ii++)
    {
        // int edge_index = _system.Numedges * m_rng->drnd();
        // std::cout<<"flip "<<edge_index<<std::endl;

		int v0t=_system.vertices[_system.edges[edge_index].v0].type;
		int v1t=_system.vertices[_system.edges[edge_index].v1].type;
		int v2t=_system.vertices[_system.edges[edge_index].v2].type;
		int v3t=_system.vertices[_system.edges[edge_index].v3].type;
		
		//PARTICLE-OCCUPIED VERTICES DO NOT PARTICIPATE IN FLIPPING
        if ((freezed_edge[_system.edges[edge_index].type] == false) && (v0t==-1) && (v1t==-1) && (v2t==-1) && (v3t==-1))
        {
            double delE = -this->ComputeEnergyFromEdge(edge_index);
            if (this->edge_flip(edge_index, true) == true)
            {
                delE+=this->ComputeEnergyFromEdge(edge_index);
                accepted_moves++;
                if (!(delE < 0.0))
                {
                    if (!(m_rng->drnd() < exp(-delE / get_temperature())))
                    {
                        this->edge_flip(edge_index, true);
                        accepted_moves--;
                    } 
                }
            }
         }

    }
    return (accepted_moves);
}
