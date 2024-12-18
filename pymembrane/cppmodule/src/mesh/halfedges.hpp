#ifndef __HALFEDGES_HPP__
#define __HALFEDGES_HPP__

#include "../types/globaltypes.hpp"
#include "meshproperties.hpp"

/*! \addtogroup mesh
 *
 *  @{
 */

/**
 * @struct HE_Vertex vertex class
 * @brief Structure that contains all the vertex in the mesh, as well basic information for each one.
 * @tparam Property
 */
template <typename PropertyVertices>
struct HE_Vertex
{
    // properties
    real3 r;          //!< position of the vertex in a 3D space
    int id;           //!< unique id
    bool boundary;    //!< if true, vertex is on boundary
    int coordination; //!< number of neighbours this vertex has
    inth3 cellId;     //!< cellId that belongs for neighbours list
    real3 normal;     //!< vertex normal is the weighted sum of the face normals that belongs
    // handle
    int _hedge; //!< HANDLE INDEX OF: one of the half-edges emanating from the vertex
    // md properties
    int type;    //!< Vertex/Edge/Face Type
    real3 v;     //!< Velocity
    real3 accel; //!< Vertex acceleration

    real3 forceC; //!< Vertex conservative force
    real3 forceD; //!< Vertex dissipative force
    real3 forceE; //!< Vertex External force
    real energy;  //!< Vertex energy
    real mass;    //!< Vertex mass
    inth3 ip;     //!< Periodic box image flags (to enable unwrapping of particle coordinates)
    real spin; //!< ferromagnetic spin +1.0, -1.0
    PropertyVertices _property;
};
using HE_VertexProp = HE_Vertex<PropertyVertices>;

/**
 * @struct HE_Edge edge class
 * @brief Structure that contains all the edges in the mesh, as well basic information for each one.
 *
 * @tparam Property
 */
template <typename PropertyEdges>
struct HE_Edge
{
    int i, j;           //!< indices of two vertices that connect the edge
    int id;             //!< unique id
    bool boundary;      //!< if true, edge is a boundary edge
    int face_k;         //!< index to one of the faces shared by this edge -1 if is outer face
    int face_l;         //!< index to one of the faces shared by this edge -1 if is outer face
    int v0, v1, v2, v3; //!< indices to the vertices around that edge
    real l0;            //!< equilibrium rest length
    // handle
    int _hedge; //!< HANDLE INDEX OF: one of the two half edges
    // md
    int type;    //!< Vertex/Edge/Face Type
    real energy; //!< Vertex/Edge/Face energy
    // properties
    PropertyEdges _property;
};
using HE_EdgeProp = HE_Edge<PropertyEdges>;

/**
 * @struct HE_Face face class
 * @brief Structure that contains all the faces in the mesh, as well basic information for each one.
 * @tparam Property
 */
template <typename PropertyFaces>
struct HE_Face
{
    // properties
    int id;       //!< face id
    bool outer;   //!< if true, face is a ghost outer face
    int nsides;   //!< number of sides face has
    real3 normal; //!< normal to that face
    real area;    //!< area of the triangle
    ///
    real g_reference[3];     //!< reference metric tensor [0] |(v2-v1)|^2 [1] (v2-v1)*(v3-v1) [2]|(v3-v1)|^2
    real g_reference_inv[3]; //!< reference inverse metric tensor
    real3 normal_reference;  //!< reference normal
    real3 r12_reference;     //!< r12 reference
    real3 r13_reference;     //!< r13 reference
    ///
    int v1, v2, v3; //!< the 3 vertices that belong to that face
    bool boundary;  //!< if true, the face is a boundary
    // handle
    int _hedge; //!< HANDLE INDEX OF: one of the half-edges bordering the face
    // md
    int type;    //!< Vertex/Edge/Face Type
    real energy; //!< Vertex/Edge/Face energy
    // properties
    PropertyFaces _property;
};
using HE_FaceProp = HE_Face<PropertyFaces>;
/**
 * @struct HE_HalfEdge handle class
 * @brief Structure that contains all the handles for the double linked-list
 * @tparam Property
 */
template <typename PropertyEdges>
struct HE_HalfEdge
{
    // handle
    int index;     //!< Index, used for half-edge sorting
    int vert_from; //!< HANDLE INDEX OF: vertex at the beginning of the half-edge
    int vert_to;   //!< HANDLE INDEX OF: vertex at the end of the half-edge
    int edge;      //!< HANDLE INDEX OF: edge this he is part of
    int face;      //!< HANDLE INDEX OF: face the half-edge borders
    int pair;      //!< HANDLE INDEX OF: oppositely oriented adjacent half-edge
    int next;      //!< HANDLE INDEX OF: next half-edge around the face
    int prev;      //!< HANDLE INDEX OF: previous half-edge around the face
    bool boundary; //!< if true, the halfedge is a boundary
    // properties
    PropertyEdges _property;
};
using HE_HalfEdgeProp = HE_HalfEdge<PropertyEdges>;

/**
 * @struct triangle
 * @brief triangle typedef
 */
struct triangle_type
{
    int id;
    int v1, v2, v3;
    int orientation;
    real g_reference[3];      //!< reference metric tensor [0] |(v2-v1)|^2 [1] (v2-v1)*(v3-v1) [2]|(v3-v1)|^2
    real3 normal_reference;   //!< reference normal
    bool is_reference_normal; //!< if is  false then use <0,0,1>
    bool is_reference_g;      //!<  if is false then use gnow
    int type;                 //!< Vertex/Edge/Face Type
    real energy;              //!< Vertex energy
    PropertyFaces prop;
    bool operator<(const triangle_type &rhs) const { return (id < rhs.id); }
};
struct vertices_type
{
    int id;
    real3 r;
    int type;     //!< Vertex/Edge/Face Type
    real3 forceC; //!< Vertex conservative force
    real3 forceD; //!< Vertex dissipative force
    real3 forceE; //!< Vertex External force
    real energy;  //!< Vertex energy
    real mass;    //!< Vertex mass
    PropertyVertices prop;
    bool operator<(const vertices_type &rhs) const { return (id < rhs.id); }
};
#endif

/*! @} */
