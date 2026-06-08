#include "potentialDihedralHarmonics.hpp"

/*! @fn real ComputeVertexDihedralEnergy::compute_energy(int vertex)
    @brief
    calculate bengind energy use dihedral harmonics as
    \f$ E_{bend} = \frac 1 2 k_b (1 - \cos(\theta - \theta_0)) \f$, where
    \f$ k_b \f$ is the bending rigidity, \f$ \theta \f$ and \f$ \theta_0 \f$ are the angle and preferred dihedral angle of the triangles meeting at the edge.
    @param vertex id
    @return the bending energy at vertex
*/
real ComputeVertexDihedralEnergy_lambda(const real3 &r0,
                                        const real3 &r1,
                                        const real3 &r2,
                                        const real3 &r3,
                                        const real &kappa,
                                        const real &theta0,
                                        const BoxType &box)
{
    // std::cout<<"test seg"<<std::endl;
    real3 r01, r20, r02, r32;
    r01 = pymemb::vector_subtract(r0, r1, box);
    r02 = pymemb::vector_subtract(r0, r2, box);
    r20 = pymemb::vector_subtract(r2, r0, box);
    r32 = pymemb::vector_subtract(r3, r2, box);

    real3 nk, nl;
    vcross(nk, r01, r02);
    vcross(nl, r32, r02);

    real nksq, nlsq, r02norm;
    nksq = vdot(nk, nk);
    nlsq = vdot(nl, nl);
    r02norm = sqrt(vdot(r02, r02));

    real r02inv, nk2inv, nl2inv, nknlinv;
    r02inv = nk2inv = nl2inv = 0.0;
    if (r02norm > 0.0)
        r02inv = 1.0 / r02norm;
    if (nksq > 0.0)
        nk2inv = 1.0 / nksq;
    if (nlsq > 0.0)
        nl2inv = 1.0 / nlsq;
    nknlinv = sqrt(nk2inv * nl2inv);

    real cos_theta, sin_theta, cos_theta_0, sin_theta_0;
    cos_theta = -vdot(nk, nl) * nknlinv;
    sin_theta = -r02norm * nknlinv * vdot(nk, r32);
    sin_theta_0 = sin(theta0);
    cos_theta_0 = cos(theta0);

    if (cos_theta > 1.0)
        cos_theta = 1.0;
    if (cos_theta < -1.0)
        cos_theta = -1.0;

    // real energy = 0.5 * kappa * (1.0 - cos_theta * cos_theta_0 - sin_theta * sin_theta_0);
    return (kappa * (1.0 - cos_theta * cos_theta_0 - sin_theta * sin_theta_0));
}

void ComputeVertexDihedralEnergy_fn(const int Numedges,
                                    HE_EdgeProp *edges,
                                    const HE_FaceProp *faces,
                                    const HE_VertexProp *vertices,
                                    const real *_kappa,
                                    const real *_theta0,
                                    const BoxType _box)
{
    for (int edge_index = 0; edge_index < Numedges; edge_index++)
    {

        if (edges[edge_index].boundary == false)
        {
            int type = edges[edge_index].type;

            int v0 = edges[edge_index].v0;
            int v1 = edges[edge_index].v1;
            int v2 = edges[edge_index].v2;
            int v3 = edges[edge_index].v3;
            /// vertices
            real3 r0 = vertices[v0].r;
            real3 r1 = vertices[v1].r;
            real3 r2 = vertices[v2].r;
            real3 r3 = vertices[v3].r;

            real energy = ComputeVertexDihedralEnergy_lambda(r0, r1, r2, r3, _kappa[type], _theta0[type], _box);

            /// Add energy to that edge
            edges[edge_index].energy += energy;
        }
    }
}

void ComputeVertexDihedralEnergy::compute_energy(void)
{

    ComputeVertexDihedralEnergy_fn(_system.Numedges,
                                   &_system.edges[0],
                                   &_system.faces[0],
                                   &_system.vertices[0],
                                   &m_kappa[0],
                                   &m_theta0[0],
                                   _system.get_box());
}

real ComputeVertexDihedralEnergy::compute_edge_energy(int query_edge_index)
{
    // we need to loop the 4 edges that are connected to the edge_index
    auto edge_index_vec = _system.get_edge_neighbours_host(query_edge_index);
    // reset energy
    real edge_energy = 0.0;
    for (auto edge_index : edge_index_vec)
    {
        if (_system.edges[edge_index].boundary == false)
        {
            int type = _system.edges[edge_index].type;

            int v0 = _system.edges[edge_index].v0;
            int v1 = _system.edges[edge_index].v1;
            int v2 = _system.edges[edge_index].v2;
            int v3 = _system.edges[edge_index].v3;
            /// vertices
            real3 r0 = _system.vertices[v0].r;
            real3 r1 = _system.vertices[v1].r;
            real3 r2 = _system.vertices[v2].r;
            real3 r3 = _system.vertices[v3].r;

            edge_energy += ComputeVertexDihedralEnergy_lambda(r0, r1, r2, r3, m_kappa[type], m_theta0[type], _system.get_box());
        }
    }
    return edge_energy;
}

real ComputeVertexDihedralEnergy::compute_vertex_energy(int query_vertex_index)
{
    real energy = 0.0;
    int he = _system.vertices[query_vertex_index]._hedge;
    int first = he;
    int he_vec[2];
    do
    {
        he_vec[0] = he;
        he_vec[1] = _system.halfedges[he].next;
        for (auto he_index : he_vec)
        {
            int edge_index = _system.halfedges[he_index].edge;
            if (_system.edges[edge_index].boundary == false)
            {
                int type = _system.edges[edge_index].type;

                int v0 = _system.edges[edge_index].v0;
                int v1 = _system.edges[edge_index].v1;
                int v2 = _system.edges[edge_index].v2;
                int v3 = _system.edges[edge_index].v3;
                /// vertices
                real3 r0 = _system.vertices[v0].r;
                real3 r1 = _system.vertices[v1].r;
                real3 r2 = _system.vertices[v2].r;
                real3 r3 = _system.vertices[v3].r;

                energy += ComputeVertexDihedralEnergy_lambda(r0, r1, r2, r3, m_kappa[type], m_theta0[type], _system.get_box());
            }
        }
        int he_prev = _system.halfedges[he].prev;
        he = _system.halfedges[he_prev].pair;
    } while (he != first);
    return energy;
}

void ComputeVertexBendingForce_fn(int Numedges,
                                  const HE_EdgeProp *edges,
                                  const HE_FaceProp *faces,
                                  HE_VertexProp *vertices,
                                  const real *_kappa,
                                  const real *_theta0,
                                  const BoxType _box)
{
    for (int edge_index = 0; edge_index < Numedges; edge_index++)
    {

        if (edges[edge_index].boundary == false)
        {
            int type = edges[edge_index].type;

            int v0 = edges[edge_index].v0;
            int v1 = edges[edge_index].v1;
            int v2 = edges[edge_index].v2;
            int v3 = edges[edge_index].v3;
            /// vertices
            real3 r0 = vertices[v0].r;
            real3 r1 = vertices[v1].r;
            real3 r2 = vertices[v2].r;
            real3 r3 = vertices[v3].r;

            real3 r01 = pymemb::vector_subtract(r0, r1, _box);
            real3 r02 = pymemb::vector_subtract(r0, r2, _box);
            // real3 r20 = pymemb::vector_subtract(r2, r0, _box);
            real3 r32 = pymemb::vector_subtract(r3, r2, _box);

            real3 nk, nl;
            vcross(nk, r01, r02);
            vcross(nl, r32, r02);

            real nksq, nlsq, r02norm;
            nksq = vdot(nk, nk);
            nlsq = vdot(nl, nl);
            r02norm = sqrt(vdot(r02, r02));

            real r02inv = 0.0, nk2inv = 0.0, nl2inv = 0.0, nknlinv;
            if (r02norm > 0.0)
                r02inv = 1.0 / r02norm;
            if (nksq > 0.0)
                nk2inv = 1.0 / nksq;
            if (nlsq > 0.0)
                nl2inv = 1.0 / nlsq;
            nknlinv = sqrt(nk2inv * nl2inv);

            real cos_theta, sin_theta, cos_theta_0, sin_theta_0;
            cos_theta = -vdot(nk, nl) * nknlinv;
            sin_theta = -r02norm * nknlinv * vdot(nk, r32);
            sin_theta_0 = sin(_theta0[type]);
            cos_theta_0 = cos(_theta0[type]);

            if (cos_theta > 1.0)
                cos_theta = 1.0;
            if (cos_theta < -1.0)
                cos_theta = -1.0;

            real df = -_kappa[type] * (sin_theta * cos_theta_0 - cos_theta * sin_theta_0);

            real dE0k = vdot(r01, r02) * nk2inv * r02inv - r02norm * nk2inv;
            real dE0l = -vdot(r32, r02) * nl2inv * r02inv;
            real dE1k = nk2inv * r02norm;
            real dE2k = -vdot(r01, r02) * nk2inv * r02inv;
            real dE2l = vdot(r32, r02) * nl2inv * r02inv - r02norm * nl2inv;
            real dE3l = nl2inv * r02norm;

            real3 F0, F1, F2, F3;
            Xvec2(F0, df * dE0k, nk, df * dE0l, nl);
            Xvec1(F1, df * dE1k, nk);
            Xvec2(F2, df * dE2k, nk, df * dE2l, nl);
            Xvec1(F3, df * dE3l, nl);

            vertices[v0].forceC.x += F0.x;
            vertices[v0].forceC.y += F0.y;
            vertices[v0].forceC.z += F0.z;

            vertices[v1].forceC.x += F1.x;
            vertices[v1].forceC.y += F1.y;
            vertices[v1].forceC.z += F1.z;

            vertices[v2].forceC.x += F2.x;
            vertices[v2].forceC.y += F2.y;
            vertices[v2].forceC.z += F2.z;

            vertices[v3].forceC.x += F3.x;
            vertices[v3].forceC.y += F3.y;
            vertices[v3].forceC.z += F3.z;
        }
    }
}

void ComputeVertexDihedralEnergy::compute(void)
{

    ComputeVertexBendingForce_fn(_system.Numedges,
                                 &_system.edges[0],
                                 &_system.faces[0],
                                 &_system.vertices[0],
                                 &m_kappa[0],
                                 &m_theta0[0],
                                 _system.get_box());
}

void ComputeVertexBendingStress_fn(int Numedges,
                                   const HE_EdgeProp *edges,
                                   const HE_FaceProp *faces,
                                   HE_VertexProp *vertices,
                                   const real *_kappa,
                                   const real *_theta0,
                                   realTensor *stress_group_edges,
                                   const BoxType _box)
{
    for (int edge_index = 0; edge_index < Numedges; edge_index++)
    {

        if (edges[edge_index].boundary == false)
        {
            int type = edges[edge_index].type;

            int v0 = edges[edge_index].v0;
            int v1 = edges[edge_index].v1;
            int v2 = edges[edge_index].v2;
            int v3 = edges[edge_index].v3;
            /// vertices
            real3 r0 = vertices[v0].r;
            real3 r1 = vertices[v1].r;
            real3 r2 = vertices[v2].r;
            real3 r3 = vertices[v3].r;

            real3 r01, r20, r02, r32;
            r01 = pymemb::vector_subtract(r0, r1, _box);
            r02 = pymemb::vector_subtract(r0, r2, _box);
            r20 = pymemb::vector_subtract(r2, r0, _box);
            r32 = pymemb::vector_subtract(r3, r2, _box);

            real3 nk, nl;
            vcross(nk, r01, r02);
            vcross(nl, r32, r02);

            real nksq, nlsq, r02norm;
            nksq = vdot(nk, nk);
            nlsq = vdot(nl, nl);
            r02norm = sqrt(vdot(r02, r02));

            real r02inv, nk2inv, nl2inv, nknlinv;
            r02inv = nk2inv = nl2inv = 0.0;
            if (r02norm > 0.0)
                r02inv = 1.0 / r02norm;
            if (nksq > 0.0)
                nk2inv = 1.0 / nksq;
            if (nlsq > 0.0)
                nl2inv = 1.0 / nlsq;
            nknlinv = sqrt(nk2inv * nl2inv);

            real cos_theta, sin_theta, cos_theta_0, sin_theta_0;
            cos_theta = -vdot(nk, nl) * nknlinv;
            sin_theta = -r02norm * nknlinv * vdot(nk, r32);
            sin_theta_0 = sin(_theta0[type]);
            cos_theta_0 = cos(_theta0[type]);

            if (cos_theta > 1.0)
                cos_theta = 1.0;
            if (cos_theta < -1.0)
                cos_theta = -1.0;

            real df = -_kappa[type] * (sin_theta * cos_theta_0 - cos_theta * sin_theta_0);

            real dE0k = vdot(r01, r02) * nk2inv * r02inv - r02norm * nk2inv;
            real dE0l = -vdot(r32, r02) * nl2inv * r02inv;
            real dE1k = nk2inv * r02norm;
            real dE2k = -vdot(r01, r02) * nk2inv * r02inv;
            real dE2l = vdot(r32, r02) * nl2inv * r02inv - r02norm * nl2inv;
            real dE3l = nl2inv * r02norm;

            real3 F0, F1, F2, F3;
            Xvec2(F0, df * dE0k, nk, df * dE0l, nl);
            Xvec1(F1, df * dE1k, nk);
            Xvec2(F2, df * dE2k, nk, df * dE2l, nl);
            Xvec1(F3, df * dE3l, nl);

            // This might be wrong so have to be checked
            // real check J. Chem. Phys. 131, 154107 (2009) page 4 Eq. 21
            // Asume that v0 is in the local replica then contruct the r1, r2, r3 based on it
            // real3 r01, r02,
            real3 r03;
            r01.x *= -1.0;
            r01.y *= -1.0;
            r01.z *= -1.0; // r01 = pymemb::vector_subtract(r1, r0, _box);
            r02.x *= -1.0;
            r02.y *= -1.0;
            r02.z *= -1.0; // r02 = pymemb::vector_subtract(r2, r0, _box);
            r03 = pymemb::vector_subtract(r3, r0, _box);
            real3 uw_r3, uw_r2, uw_r1 /*,uw_r0,*/;
            // uw_r0 = r0;
            uw_r1 = pymemb::vector_sum(r0, r01);
            uw_r2 = pymemb::vector_sum(r0, r02);
            uw_r3 = pymemb::vector_sum(r0, r03);

            stress_group_edges[edge_index].xx += r0.x * F0.x + uw_r1.x * F1.x + uw_r2.x * F2.x + uw_r3.x * F3.x;
            stress_group_edges[edge_index].xy += r0.x * F0.y + uw_r1.x * F1.y + uw_r2.x * F2.y + uw_r3.x * F3.y;
            stress_group_edges[edge_index].xz += r0.x * F0.z + uw_r1.x * F1.z + uw_r2.x * F2.z + uw_r3.x * F3.z;

            stress_group_edges[edge_index].yx += r0.y * F0.x + uw_r1.y * F1.x + uw_r2.y * F2.x + uw_r3.y * F3.x;
            stress_group_edges[edge_index].yy += r0.y * F0.y + uw_r1.y * F1.y + uw_r2.y * F2.y + uw_r3.y * F3.y;
            stress_group_edges[edge_index].yz += r0.y * F0.z + uw_r1.y * F1.z + uw_r2.y * F2.z + uw_r3.y * F3.z;

            stress_group_edges[edge_index].zx += r0.z * F0.x + uw_r1.z * F1.x + uw_r2.z * F2.x + uw_r3.z * F3.x;
            stress_group_edges[edge_index].zy += r0.z * F0.y + uw_r1.z * F1.y + uw_r2.z * F2.y + uw_r3.z * F3.y;
            stress_group_edges[edge_index].zz += r0.z * F0.z + uw_r1.z * F1.z + uw_r2.z * F2.z + uw_r3.z * F3.z;
        }
    }
}

void ComputeVertexDihedralEnergy::compute_stress(void)
{
    ComputeVertexBendingStress_fn(_system.Numedges,
                                  &_system.edges[0],
                                  &_system.faces[0],
                                  &_system.vertices[0],
                                  &m_kappa[0],
                                  &m_theta0[0],
                                  &_system.stress_group_edges[0],
                                  _system.get_box());
}

void ComputeVertexBendingStressAtom_fn(int Numedges,
                                       const HE_EdgeProp *edges,
                                       const HE_FaceProp *faces,
                                       HE_VertexProp *vertices,
                                       const real *_kappa,
                                       const real *_theta0,
                                       realTensor *stress_virial_atom,
                                       const BoxType _box)
{
    for (int edge_index = 0; edge_index < Numedges; edge_index++)
    {

        if (edges[edge_index].boundary == false)
        {
            int type = edges[edge_index].type;

            int v0 = edges[edge_index].v0;
            int v1 = edges[edge_index].v1;
            int v2 = edges[edge_index].v2;
            int v3 = edges[edge_index].v3;
            /// vertices
            real3 r0 = vertices[v0].r;
            real3 r1 = vertices[v1].r;
            real3 r2 = vertices[v2].r;
            real3 r3 = vertices[v3].r;

            real3 r01, r20, r02, r32;
            r01 = pymemb::vector_subtract(r0, r1, _box);
            r02 = pymemb::vector_subtract(r0, r2, _box);
            r20 = pymemb::vector_subtract(r2, r0, _box);
            r32 = pymemb::vector_subtract(r3, r2, _box);

            real3 nk, nl;
            vcross(nk, r01, r02);
            vcross(nl, r32, r02);

            real nksq, nlsq, r02norm;
            nksq = vdot(nk, nk);
            nlsq = vdot(nl, nl);
            r02norm = sqrt(vdot(r02, r02));

            real r02inv, nk2inv, nl2inv, nknlinv;
            r02inv = nk2inv = nl2inv = 0.0;
            if (r02norm > 0.0)
                r02inv = 1.0 / r02norm;
            if (nksq > 0.0)
                nk2inv = 1.0 / nksq;
            if (nlsq > 0.0)
                nl2inv = 1.0 / nlsq;
            nknlinv = sqrt(nk2inv * nl2inv);

            real cos_theta, sin_theta, cos_theta_0, sin_theta_0;
            cos_theta = -vdot(nk, nl) * nknlinv;
            sin_theta = -r02norm * nknlinv * vdot(nk, r32);
            sin_theta_0 = sin(_theta0[type]);
            cos_theta_0 = cos(_theta0[type]);

            if (cos_theta > 1.0)
                cos_theta = 1.0;
            if (cos_theta < -1.0)
                cos_theta = -1.0;

            real df = -_kappa[type] * (sin_theta * cos_theta_0 - cos_theta * sin_theta_0);

            real dE0k = vdot(r01, r02) * nk2inv * r02inv - r02norm * nk2inv;
            real dE0l = -vdot(r32, r02) * nl2inv * r02inv;
            real dE1k = nk2inv * r02norm;
            real dE2k = -vdot(r01, r02) * nk2inv * r02inv;
            real dE2l = vdot(r32, r02) * nl2inv * r02inv - r02norm * nl2inv;
            real dE3l = nl2inv * r02norm;

            real3 F0, F1, F2, F3;
            Xvec2(F0, df * dE0k, nk, df * dE0l, nl);
            Xvec1(F1, df * dE1k, nk);
            Xvec2(F2, df * dE2k, nk, df * dE2l, nl);
            Xvec1(F3, df * dE3l, nl);

            /*
            FOR NOW USE THE GROUP METHOD FOR AN EDGE AND DEVIDE BY 4 IN EACH ATOM
            */
            // This might be wrong so have to be checked
            // real check J. Chem. Phys. 131, 154107 (2009) page 4 Eq. 21
            // Asume that v0 is in the local replica then contruct the r1, r2, r3 based on it
            // real3 r01, r02,
            real3 r03;
            r01.x *= -1.0;
            r01.y *= -1.0;
            r01.z *= -1.0; // r01 = pymemb::vector_subtract(r1, r0, _box);
            r02.x *= -1.0;
            r02.y *= -1.0;
            r02.z *= -1.0; // r02 = pymemb::vector_subtract(r2, r0, _box);
            r03 = pymemb::vector_subtract(r3, r0, _box);
            real3 uw_r3, uw_r2, uw_r1 /*,uw_r0,*/;
            // uw_r0 = r0;
            uw_r1 = pymemb::vector_sum(r0, r01);
            uw_r2 = pymemb::vector_sum(r0, r02);
            uw_r3 = pymemb::vector_sum(r0, r03);

            realTensor stress_group_edge;
            stress_group_edge.xx = r0.x * F0.x + uw_r1.x * F1.x + uw_r2.x * F2.x + uw_r3.x * F3.x;
            stress_group_edge.xy = r0.x * F0.y + uw_r1.x * F1.y + uw_r2.x * F2.y + uw_r3.x * F3.y;
            stress_group_edge.xz = r0.x * F0.z + uw_r1.x * F1.z + uw_r2.x * F2.z + uw_r3.x * F3.z;

            stress_group_edge.yx = r0.y * F0.x + uw_r1.y * F1.x + uw_r2.y * F2.x + uw_r3.y * F3.x;
            stress_group_edge.yy = r0.y * F0.y + uw_r1.y * F1.y + uw_r2.y * F2.y + uw_r3.y * F3.y;
            stress_group_edge.yz = r0.y * F0.z + uw_r1.y * F1.z + uw_r2.y * F2.z + uw_r3.y * F3.z;

            stress_group_edge.zx = r0.z * F0.x + uw_r1.z * F1.x + uw_r2.z * F2.x + uw_r3.z * F3.x;
            stress_group_edge.zy = r0.z * F0.y + uw_r1.z * F1.y + uw_r2.z * F2.y + uw_r3.z * F3.y;
            stress_group_edge.zz = r0.z * F0.z + uw_r1.z * F1.z + uw_r2.z * F2.z + uw_r3.z * F3.z;

            int vvec[4] = {v0, v1, v2, v3};
            for (auto v : vvec)
            {
                stress_virial_atom[v].xx += 0.25 * stress_group_edge.xx;
                stress_virial_atom[v].xy += 0.25 * stress_group_edge.xy;
                stress_virial_atom[v].xz += 0.25 * stress_group_edge.xz;

                stress_virial_atom[v].yx += 0.25 * stress_group_edge.yx;
                stress_virial_atom[v].yy += 0.25 * stress_group_edge.yy;
                stress_virial_atom[v].yz += 0.25 * stress_group_edge.yz;

                stress_virial_atom[v].zx += 0.25 * stress_group_edge.zx;
                stress_virial_atom[v].zy += 0.25 * stress_group_edge.zy;
                stress_virial_atom[v].zz += 0.25 * stress_group_edge.zz;
            }
        }
    }
}

void ComputeVertexDihedralEnergy::compute_atomic_stress(void)
{
    ComputeVertexBendingStressAtom_fn(_system.Numedges,
                                      &_system.edges[0],
                                      &_system.faces[0],
                                      &_system.vertices[0],
                                      &m_kappa[0],
                                      &m_theta0[0],
                                      &_system.stress_virial_atom[0],
                                      _system.get_box());
}