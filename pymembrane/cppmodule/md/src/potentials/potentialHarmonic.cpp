#include "potentialHarmonic.hpp"

inline real ComputeEdgeHarmonicEnergy(const real3 &rij,
                                      const real &k,
                                      const real &l0)
{
    auto dr = sqrt(vdot(rij, rij));
    return (0.5 * k * (dr - l0) * (dr - l0));
}

void ComputeVertexHarmonicEnergy_fn(const int Numedges,
                                    HE_EdgeProp *edges,
                                    const HE_VertexProp *vertices,
                                    const real *__restrict__ _k,
                                    const real *__restrict__ _l0,
                                    const BoxType &_box)
{
    for (int edge_index = 0; edge_index < Numedges; edge_index++)
    {
        int type = edges[edge_index].type;
        int v1 = edges[edge_index].i;
        auto r1 = vertices[v1].r;
        int v2 = edges[edge_index].j;
        auto r2 = vertices[v2].r;
        auto rij = pymemb::vector_subtract(r2, r1, _box);

        auto energy = ComputeEdgeHarmonicEnergy(rij, _k[type], _l0[type]);

        /// ADD ENERGY TO THAT EDGE
        edges[edge_index].energy += energy;
    }
}

double ComputeVertexHarmonicEnergy::compute_edge_energy(int query_edge_index)
{
    int type = _system.edges[query_edge_index].type;
    int v1 = _system.edges[query_edge_index].i;
    auto r1 = _system.vertices[v1].r;
    int v2 = _system.edges[query_edge_index].j;
    auto r2 = _system.vertices[v2].r;
    auto rij = pymemb::vector_subtract(r2, r1, _system.get_box());
    double edge_energy = ComputeEdgeHarmonicEnergy(rij, m_k[type], m_l0[type]);
    return edge_energy;
}

double ComputeVertexHarmonicEnergy::compute_vertex_energy(int query_vertex_index)
{

    double energy = 0.0;
    ///< get the triangle that this vertex is part of
    int he = _system.vertices[query_vertex_index]._hedge;
    int first = he;
    do
    {
        int edge_index = _system.halfedges[he].edge;
        int type = _system.edges[edge_index].type;
        int v1 = _system.edges[edge_index].i;
        auto r1 = _system.vertices[v1].r;
        int v2 = _system.edges[edge_index].j;
        auto r2 = _system.vertices[v2].r;
        auto rij = pymemb::vector_subtract(r2, r1, _system.get_box());
        energy += 0.5 * ComputeEdgeHarmonicEnergy(rij, m_k[type], m_l0[type]);
        
        int he_pair = _system.halfedges[he].pair;
        he = _system.halfedges[he_pair].next;
    } while ((he != first));
    return energy;
}

void ComputeVertexHarmonicEnergy::compute_energy(void)
{

    ComputeVertexHarmonicEnergy_fn(_system.Numedges,
                                   &_system.edges[0],
                                   &_system.vertices[0],
                                   &m_k[0],
                                   &m_l0[0],
                                   _system.get_box());
}

inline real ComputeVertexHarmonicForce(const real3& rij,
                                  const real& k,
                                  const real& l0)
{
    auto dr = sqrt(vdot(rij, rij));
    auto fval = k * (dr - l0) / dr;
    return fval;
}

void ComputeVertexHarmonicForce_fn(const int Numedges,
                                   HE_VertexProp *vertices,
                                   const HE_EdgeProp *__restrict__ edges,
                                   const real *__restrict__ _k,
                                   const real *__restrict__ _l0,
                                   const BoxType &_box)
{
    for (int edge_index = 0; edge_index < Numedges; edge_index++)
    {
        int type = edges[edge_index].type;
        int v1 = edges[edge_index].i;
        auto r1 = vertices[v1].r;
        int v2 = edges[edge_index].j;
        auto r2 = vertices[v2].r;
        auto rij = pymemb::vector_subtract(r2, r1, _box);

        double fval = ComputeVertexHarmonicForce(rij, _k[type], _l0[type]);

        vertices[v1].forceC.x += fval * rij.x;
        vertices[v1].forceC.y += fval * rij.y;
        vertices[v1].forceC.z += fval * rij.z;

        vertices[v2].forceC.x += -1.0 * fval * rij.x;
        vertices[v2].forceC.y += -1.0 * fval * rij.y;
        vertices[v2].forceC.z += -1.0 * fval * rij.z;
    }
}

void ComputeVertexHarmonicEnergy::compute(void)
{

    ComputeVertexHarmonicForce_fn(_system.Numedges,
                                  &_system.vertices[0],
                                  &_system.edges[0],
                                  &m_k[0],
                                  &m_l0[0],
                                  _system.get_box());
}

void ComputeVertexHarmonicStress_fn(const int Numedges,
                                    HE_VertexProp *vertices,
                                    const HE_EdgeProp *__restrict__ edges,
                                    const real *__restrict__ _k,
                                    const real *__restrict__ _l0,
                                    realTensor *stress_group_edges,
                                    const BoxType &_box)
{
    for (int edge_index = 0; edge_index < Numedges; edge_index++)
    {

        int type = edges[edge_index].type;
        int v1 = edges[edge_index].i;
        auto r1 = vertices[v1].r;
        int v2 = edges[edge_index].j;
        auto r2 = vertices[v2].r;
        auto r12 = pymemb::vector_subtract(r2, r1, _box);

        double fval = ComputeVertexHarmonicForce(r12, _k[type], _l0[type]);

        
        // J. Chem. Phys. 131, 154107 (2009) page 4 Eq. 21
        // Asume that v1 is in the local replica then construct the r1, r2 based on it
        real3 uw_r2;
        uw_r2 = pymemb::vector_sum(r1, r12);

        real3 F2, F1;
        F1.x = fval * r12.x;
        F1.y = fval * r12.y;
        F1.z = fval * r12.z;

        F2.x = -fval * r12.x;
        F2.y = -fval * r12.y;
        F2.z = -fval * r12.z;

        stress_group_edges[edge_index].xx += r1.x * F1.x + uw_r2.x * F2.x;
        stress_group_edges[edge_index].xy += r1.x * F1.y + uw_r2.x * F2.y;
        stress_group_edges[edge_index].xz += r1.x * F1.z + uw_r2.x * F2.z;

        stress_group_edges[edge_index].yx += r1.y * F1.x + uw_r2.y * F2.x;
        stress_group_edges[edge_index].yy += r1.y * F1.y + uw_r2.y * F2.y;
        stress_group_edges[edge_index].yz += r1.y * F1.z + uw_r2.y * F2.z;

        stress_group_edges[edge_index].zx += r1.z * F1.x + uw_r2.z * F2.x;
        stress_group_edges[edge_index].zy += r1.z * F1.y + uw_r2.z * F2.y;
        stress_group_edges[edge_index].zz += r1.z * F1.z + uw_r2.z * F2.z;
    }
}

void ComputeVertexHarmonicEnergy::compute_stress(void)
{
    ComputeVertexHarmonicStress_fn(_system.Numedges,
                                   &_system.vertices[0],
                                   &_system.edges[0],
                                   &m_k[0],
                                   &m_l0[0],
                                   &_system.stress_group_edges[0],
                                   _system.get_box());
}

void ComputeVertexHarmonicStressAtom_fn(const int Numedges,
                                        HE_VertexProp *vertices,
                                        const HE_EdgeProp *__restrict__ edges,
                                        const real *__restrict__ _k,
                                        const real *__restrict__ _l0,
                                        realTensor *stress_virial_atom,
                                        const BoxType &_box)
{
    for (int edge_index = 0; edge_index < Numedges; edge_index++)
    {
        int type = edges[edge_index].type;
        int v1 = edges[edge_index].i;
        auto r1 = vertices[v1].r;
        int v2 = edges[edge_index].j;
        auto r2 = vertices[v2].r;
        auto r12 = pymemb::vector_subtract(r2, r1, _box);

        double fval = ComputeVertexHarmonicForce(r12, _k[type], _l0[type]);

        // J. Chem. Phys. 131, 154107 (2009) page 4 Eq. 21
        // Asume that v1 is in the local replica then contruct the r1, r2 based on it
        real3 uw_r2;
        uw_r2 = pymemb::vector_sum(r1, r12);

        real3 F2, F1;
        F1.x = fval * r12.x;
        F1.y = fval * r12.y;
        F1.z = fval * r12.z;

        F2.x = -fval * r12.x;
        F2.y = -fval * r12.y;
        F2.z = -fval * r12.z;

        // virial as we know it with PBC
        stress_virial_atom[v1].xx += 0.5 * r12.x * F1.x;
        stress_virial_atom[v1].xy += 0.5 * r12.x * F1.y;
        stress_virial_atom[v1].xz += 0.5 * r12.x * F1.z;
        stress_virial_atom[v1].yx += 0.5 * r12.y * F1.x;
        stress_virial_atom[v1].yy += 0.5 * r12.y * F1.y;
        stress_virial_atom[v1].yz += 0.5 * r12.y * F1.z;
        stress_virial_atom[v1].zx += 0.5 * r12.z * F1.x;
        stress_virial_atom[v1].zy += 0.5 * r12.z * F1.y;
        stress_virial_atom[v1].zz += 0.5 * r12.z * F1.z;

        stress_virial_atom[v2].xx += -0.5 * r12.x * F2.x;
        stress_virial_atom[v2].xy += -0.5 * r12.x * F2.y;
        stress_virial_atom[v2].xz += -0.5 * r12.x * F2.z;
        stress_virial_atom[v2].yx += -0.5 * r12.y * F2.x;
        stress_virial_atom[v2].yy += -0.5 * r12.y * F2.y;
        stress_virial_atom[v2].yz += -0.5 * r12.y * F2.z;
        stress_virial_atom[v2].zx += -0.5 * r12.z * F2.x;
        stress_virial_atom[v2].zy += -0.5 * r12.z * F2.y;
        stress_virial_atom[v2].zz += -0.5 * r12.z * F2.z;
    }
}

void ComputeVertexHarmonicEnergy::compute_atomic_stress(void)
{
    ComputeVertexHarmonicStressAtom_fn(_system.Numedges,
                                       &_system.vertices[0],
                                       &_system.edges[0],
                                       &m_k[0],
                                       &m_l0[0],
                                       &_system.stress_virial_atom[0],
                                       _system.get_box());
}
