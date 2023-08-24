#ifndef __edge_flip_hpp__
#define __edge_flip_hpp__

#include "../system/systemclass.hpp"
#include "../box/pbc.hpp"

namespace pymemb
{
    inline bool CheckEdgeFlip_lambda(const int flip_edge_index,
                                     HE_FaceProp *faces,
                                     HE_VertexProp *vertices,
                                     HE_EdgeProp *edges,
                                     HE_HalfEdgeProp *halfedges,
                                     const BoxType _box)
    {
        // printf("trying to flip the edge =%i\n", flip_edge_index);
        // by default the edge can flip
        bool can_flip = true;
        // retrieve the vertices around the edge
        int v0 = edges[flip_edge_index].v0;
        int v1 = edges[flip_edge_index].v1;
        int v2 = edges[flip_edge_index].v2;
        int v3 = edges[flip_edge_index].v3;

        int face_k = edges[flip_edge_index].face_k;
        int face_l = edges[flip_edge_index].face_l;

        //(0) Check that the edge is not a boundary edge
        if (edges[flip_edge_index].boundary == true)
        {
            py::print("the edge id ", flip_edge_index, " is a boundary edge");
            // can_flip = false;
            return false;
        }
        //(1) first we need to check if we don't destroy the triangulation by flipping the edge.
        // This can be done by checking that the connectivity of the edges v0, v2 of the involve edge is
        // strictly larger than 3.
        int he, first, he_pair, he_pair_next;
        int n_neighbors_v0 = 0, n_neighbors_v2 = 0;

        // check v0
        he = vertices[v0]._hedge;
        first = he;

        do
        {
            n_neighbors_v0++;
            // MOVE TO THE NEXT FACE
            he_pair = halfedges[he].pair;
            he_pair_next = halfedges[he_pair].next;
            he = he_pair_next;
        } while ((he != first));

        // check v2
        he = vertices[v2]._hedge;
        first = he;

        do
        {
            n_neighbors_v2++;
            // MOVE TO THE NEXT FACE
            he_pair = halfedges[he].pair;
            he_pair_next = halfedges[he_pair].next;
            he = he_pair_next;
        } while ((he != first));

        // if (n_neighbors_v0 <= 4 || n_neighbors_v2 <= 4)
        if (n_neighbors_v0 < 3 || n_neighbors_v2 < 3)
        {
            py::print("the edge id ", flip_edge_index, "vertex the connectivity v0 or v2 is smaller than 3");
            // can_flip = false;
            return false;
        }

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
        r02 = pymemb::minimum_image(vertices[v0].r, vertices[v2].r, _box);
        r03 = pymemb::minimum_image(vertices[v0].r, vertices[v3].r, _box);
        r01 = pymemb::minimum_image(vertices[v0].r, vertices[v1].r, _box);
        real r02_norm_sq = (vdot(r02, r02));
        real r03_norm_sq = (vdot(r03, r03));
        real r01_norm_sq = (vdot(r01, r01));
        real angle_r01_r02 = acos(vdot(r01, r02) / sqrt(r01_norm_sq * r02_norm_sq));
        real angle_r03_r02 = acos(vdot(r03, r02) / sqrt(r03_norm_sq * r02_norm_sq));
        if ((angle_r01_r02 + angle_r03_r02) >= defPI)
        {
            py::print("the edge id ", flip_edge_index, " vertex v0 ", v0, " take part of an obtuse triangles");
            // can_flip = false;
            return false;
        }
        real3 r20, r21, r23;
        r20 = pymemb::minimum_image(vertices[v2].r, vertices[v0].r, _box);
        r23 = pymemb::minimum_image(vertices[v2].r, vertices[v3].r, _box);
        r21 = pymemb::minimum_image(vertices[v2].r, vertices[v1].r, _box);
        real r23_norm_sq = (vdot(r23, r23));
        real r21_norm_sq = (vdot(r21, r21));
        real angle_r21_r20 = acos(vdot(r21, r20) / sqrt(r21_norm_sq * r02_norm_sq));
        real angle_r23_r20 = acos(vdot(r23, r20) / sqrt(r23_norm_sq * r02_norm_sq));
        if ((angle_r21_r20 + angle_r23_r20) >= defPI)
        {
            py::print("the edge id ", flip_edge_index, " vertex v2 ", v2, " take part of an obtuse triangles");
            // can_flip = false;
            return false;
        }
        return true;
    }

    inline bool EdgeFlip_lambda(const int flip_edge_index,
                                const bool flip_face_up,
                                HE_FaceProp *faces,
                                HE_VertexProp *vertices,
                                HE_EdgeProp *edges,
                                HE_HalfEdgeProp *halfedges,
                                const BoxType _box)
    {
        // printf("trying to flip the edge =%i\n", flip_edge_index);
        // by default the edge can flip
        bool can_flip = true;
        // retrieve the vertices around the edge
        int v0 = edges[flip_edge_index].v0;
        int v1 = edges[flip_edge_index].v1;
        int v2 = edges[flip_edge_index].v2;
        int v3 = edges[flip_edge_index].v3;

        int face_k = edges[flip_edge_index].face_k;
        int face_l = edges[flip_edge_index].face_l;

        //(0) Check that the edge is not a boundary edge
        if (edges[flip_edge_index].boundary == true)
        {
            // py::print("the edge is a boundary edge\n");
            can_flip = false;
            // break;
        }
        //(1) first we need to check if we don't destroy the triangulation by flipping the edge.
        // This can be done by checking that the connectivity of the edges v0, v2 of the involve edge is
        // strictly larger than 3.
        int he, first, he_pair, he_pair_next;
        int n_neighbors_v0 = 0, n_neighbors_v2 = 0;

        // check v0
        he = vertices[v0]._hedge;
        first = he;

        do
        {
            n_neighbors_v0++;
            // MOVE TO THE NEXT FACE
            he_pair = halfedges[he].pair;
            he_pair_next = halfedges[he_pair].next;
            he = he_pair_next;
        } while ((he != first));

        // check v2
        he = vertices[v2]._hedge;
        first = he;

        do
        {
            n_neighbors_v2++;
            // MOVE TO THE NEXT FACE
            he_pair = halfedges[he].pair;
            he_pair_next = halfedges[he_pair].next;
            he = he_pair_next;
        } while ((he != first));

        // if (n_neighbors_v0 <= 4 || n_neighbors_v2 <= 4)
        if (n_neighbors_v0 < 3 || n_neighbors_v2 < 3)
        {
            // py::print("the connectivity of the edges v0, v2 of the involve edge is smaller than 3\n");
            can_flip = false;
            // break;
        }
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
        r02 = pymemb::minimum_image(vertices[v0].r, vertices[v2].r, _box);
        r03 = pymemb::minimum_image(vertices[v0].r, vertices[v3].r, _box);
        r01 = pymemb::minimum_image(vertices[v0].r, vertices[v1].r, _box);
        real r02_norm_sq = (vdot(r02, r02));
        real r03_norm_sq = (vdot(r03, r03));
        real r01_norm_sq = (vdot(r01, r01));
        real angle_r01_r02 = acos(vdot(r01, r02) / sqrt(r01_norm_sq * r02_norm_sq));
        real angle_r03_r02 = acos(vdot(r03, r02) / sqrt(r03_norm_sq * r02_norm_sq));
        if ((angle_r01_r02 + angle_r03_r02) >= defPI)
        {
            // py::print("v0 and v2 take part of an obtuse triangles\n");
            can_flip = false;
            // break;
        }
        real3 r20, r21, r23;
        r20 = pymemb::minimum_image(vertices[v2].r, vertices[v0].r, _box);
        r23 = pymemb::minimum_image(vertices[v2].r, vertices[v3].r, _box);
        r21 = pymemb::minimum_image(vertices[v2].r, vertices[v1].r, _box);
        real r23_norm_sq = (vdot(r23, r23));
        real r21_norm_sq = (vdot(r21, r21));
        real angle_r21_r20 = acos(vdot(r21, r20) / sqrt(r21_norm_sq * r02_norm_sq));
        real angle_r23_r20 = acos(vdot(r23, r20) / sqrt(r23_norm_sq * r02_norm_sq));
        if ((angle_r21_r20 + angle_r23_r20) >= defPI)
        {
            can_flip = false;
            // break;
        }

        if (can_flip == true)
        {
            // update the halfedges
            int he_vec[6]; // he01, he12, he20, he02, he23, he30;
            int he_edge = edges[flip_edge_index]._hedge;
            if (halfedges[he_edge].vert_from == v0)
            {
                he_vec[3] = he_edge;                   // he02
                he_vec[2] = halfedges[he_vec[3]].pair; // he20
            }
            else
            {
                he_vec[2] = he_edge;                   // he20
                he_vec[3] = halfedges[he_vec[2]].pair; // he02
            }
            he_vec[0] = halfedges[he_vec[2]].next; // he01
            he_vec[1] = halfedges[he_vec[2]].prev; // he12
            he_vec[4] = halfedges[he_vec[3]].next; // he23
            he_vec[5] = halfedges[he_vec[3]].prev; // he30

            // deal with the from, to vertices in the "new" flip edge
            halfedges[he_vec[2]].vert_from = v3;
            halfedges[he_vec[2]].vert_to = v1;
            halfedges[he_vec[3]].vert_from = v1;
            halfedges[he_vec[3]].vert_to = v3;

            // flip face: this stage is crucial if we want to make the faces diffuse
            int new_face_k = face_k;
            int new_face_l = face_l;
            if (flip_face_up == false) // counter-clock wise
            {
                new_face_k = face_l;
                new_face_l = face_k;
            }
            // deal with the references inside of the halfedges
            // he12 > h23 > h20
            halfedges[he_vec[1]].next = he_vec[4];
            halfedges[he_vec[1]].prev = he_vec[2];
            halfedges[he_vec[4]].next = he_vec[2];
            halfedges[he_vec[4]].prev = he_vec[1];
            halfedges[he_vec[2]].next = he_vec[1];
            halfedges[he_vec[2]].prev = he_vec[4];
            // faces
            halfedges[he_vec[1]].face = new_face_k;
            halfedges[he_vec[4]].face = new_face_k;
            halfedges[he_vec[2]].face = new_face_k;

            // he01 > h02 > h30
            halfedges[he_vec[0]].next = he_vec[3];
            halfedges[he_vec[0]].prev = he_vec[5];
            halfedges[he_vec[3]].next = he_vec[5];
            halfedges[he_vec[3]].prev = he_vec[0];
            halfedges[he_vec[5]].next = he_vec[0];
            halfedges[he_vec[5]].prev = he_vec[3];
            // faces
            halfedges[he_vec[5]].face = new_face_l;
            halfedges[he_vec[0]].face = new_face_l;
            halfedges[he_vec[3]].face = new_face_l;

            // deal with the faces and references to the halfedges
            // face_k
            int _v1k = v2;
            int _v2k = v3;
            int _v3k = v1;
            pymemb::arrange_vertices_by_smallest(_v1k, _v2k, _v3k);

            faces[new_face_k].v1 = _v1k; // v1;
            faces[new_face_k].v2 = _v2k; // v2;
            faces[new_face_k].v3 = _v3k; // v3;
            faces[new_face_k]._hedge = he_vec[2];
            // face_l
            int _v1l = v0;
            int _v2l = v1;
            int _v3l = v3;
            pymemb::arrange_vertices_by_smallest(_v1l, _v2l, _v3l);
            faces[new_face_l].v1 = _v1l; // v3;
            faces[new_face_l].v2 = _v2l; // v0;
            faces[new_face_l].v3 = _v3l; // v1;
            faces[new_face_l]._hedge = he_vec[3];

            // deal with the halfedge references to the vertices
            if (vertices[v0]._hedge == he_vec[3] /*he02*/)
                vertices[v0]._hedge = he_vec[0]; /*h01*/
            if (vertices[v2]._hedge == he_vec[2] /*he20*/)
                vertices[v2]._hedge = he_vec[4]; /*h23*/

            // deal with the i,j,v0,v1,v2,v3 in the edge list
            for (auto he_index : he_vec)
            {
                HE_HalfEdgeProp he = halfedges[he_index];
                HE_HalfEdgeProp he_pair = halfedges[halfedges[he_index].pair];
                int edge_index = he.edge;
                edges[edge_index].face_k = he.face;      //!< index to one of the faces shared by this edge -1 if is outer face
                edges[edge_index].face_l = he_pair.face; //!< index to one of the faces shared by this edge -1 if is outer face
                edges[edge_index].v0 = he.vert_to;
                edges[edge_index].v2 = he.vert_from;
                edges[edge_index].j = he.vert_to;
                edges[edge_index].i = he.vert_from;
                int he_next = he.next;
                edges[edge_index].v1 = halfedges[he_next].vert_to;
                int he_index_pair_next = he_pair.next;
                edges[edge_index].v3 = halfedges[he_index_pair_next].vert_to;
            }

            // printf("flipped edge =%i\n", flip_edge_index);
        }

        return can_flip;
    }
}
#endif