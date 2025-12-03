#include "potentialIsing.hpp"

/*! Iterate over all edges that origiante in the given vertex. If the
    edge is shorter than \a m_min or longer than \a m_max infinity (actually
    a very large number, \f$ 10^{15} \f$ is returned. Otherwise, this method
    returns zero.

    \f$ E_{limit} = \infty \f$ if \f$ r < l_{min} \f$ or \f$ r > l_{max} \f$ and
    \f$ E_{limit} = 0 \f$ otherwise
    \param v Pointer to the vertex
*/

real ComputeEdgeFerroEnergy_lambda(const real& s1,
                                const real& s2,
                                const real& J)
{
    auto H = -J * s1 * s2;
    return H;
}

void ComputeVertexFerroEnergy_fn(int Numedges,
                                 HE_EdgeProp *edges,
                                 HE_VertexProp *vertices,
                                 real *_J,
                                 real *_h)
{
    for (int edge_index = 0; edge_index < Numedges; edge_index++)
    {
        int type = edges[edge_index].type;

        int v1 = edges[edge_index].i;
        auto _s1 = vertices[v1].spin;

        int v2 = edges[edge_index].j;
        auto _s2 = vertices[v2].spin;

        edges[edge_index].energy += ComputeEdgeFerroEnergy_lambda(_s1, _s2, _J[type]);
    }
}

void ComputeVertexFerromagneticEnergy::compute_energy(void)
{

    ComputeVertexFerroEnergy_fn(_system.Numedges,
                                &_system.edges[0],
                                &_system.vertices[0],
                                &J[0],
                                &h[0]);
}

real ComputeVertexFerromagneticEnergy::compute_edge_energy(int query_edge_index)
{
    int type = _system.edges[query_edge_index].type;

    int v1 = _system.edges[query_edge_index].i;
    auto _s1 = _system.vertices[v1].spin;

    int v2 = _system.edges[query_edge_index].j;
    auto _s2 = _system.vertices[v2].spin;

    real edge_energy = ComputeEdgeFerroEnergy_lambda(_s1, _s2, J[type]);

    return edge_energy;
}

real ComputeVertexFerromagneticEnergy::compute_vertex_energy(int query_vertex_index)
{

    real energy = 0.0;
    ///< get the triangle that this vertex is part of
    int he = _system.vertices[query_vertex_index]._hedge;
    int first = he;
    do
    {
        int edge_index = _system.halfedges[he].edge;
        int type = _system.edges[edge_index].type;
        int v1 = _system.edges[edge_index].i;
        auto _s1 = _system.vertices[v1].spin;
        int v2 = _system.edges[edge_index].j;
        auto _s2 = _system.vertices[v2].spin;
        energy += 0.5 * ComputeEdgeFerroEnergy_lambda(_s1, _s2, J[type]);
        // MOVE TO THE NEXT edge
        int he_pair = _system.halfedges[he].pair;
        he = _system.halfedges[he_pair].next;
    } while ((he != first));
    return energy;
}

void ComputeVertexFerromagneticEnergy::compute(void)
{
}
