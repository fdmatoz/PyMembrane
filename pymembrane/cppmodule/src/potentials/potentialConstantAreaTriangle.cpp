#include "potentialConstantAreaTriangle.hpp"

real ComputeVertexConstantAreaTriangleEnergy_lambda(const real *__restrict__ g_now,
                                                    const real sigma,
                                                    const real target_area)
{
    real triangle_area = pymemb::compute_area_triangle_from_metric(g_now);
    real energy = 0.5 * sigma * (triangle_area - target_area) * (triangle_area - target_area);
    return energy;
}

void ComputeVertexConstantAreaTriangleEnergy_fn(const int Numfaces,
                                                HE_FaceProp *faces,
                                                const HE_VertexProp *vertices,
                                                const real *__restrict__ _sigma,
                                                const real *__restrict__ _target_area,
                                                const BoxType _box)
{
    for (int face_index = 0; face_index < Numfaces; face_index++)
    {
        int type = faces[face_index].type;
        int v1 = faces[face_index].v1;
        int v2 = faces[face_index].v2;
        int v3 = faces[face_index].v3;

        real g_now[3];
        pymemb::compute_form_factor_triangle(g_now, vertices[v1].r, vertices[v2].r, vertices[v3].r, _box);
        real energy = ComputeVertexConstantAreaTriangleEnergy_lambda(g_now, _sigma[type], _target_area[type]);

        /// Add energy to that face
        faces[face_index].energy += energy;
    }
}

void ComputeVertexConstantAreaTriangleEnergy::compute_energy(void)
{

    ComputeVertexConstantAreaTriangleEnergy_fn(_system.Numfaces,
                                               &_system.faces[0],
                                               &_system.vertices[0],
                                               &sigma[0],
                                               &target_area[0],
                                               _system.get_box());
}

real ComputeVertexConstantAreaTriangleEnergy::compute_edge_energy(int query_edge_index)
{
    // we need to loop the 2 faces that are connected to the edge_index
    pymemb::vector<int> face_vec{_system.edges[query_edge_index].face_k, _system.edges[query_edge_index].face_l};
    // reset energy
    real edge_energy = 0.0;
    for (auto face_index : face_vec)
    {
        int type = _system.faces[face_index].type;
        int v1 = _system.faces[face_index].v1;
        int v2 = _system.faces[face_index].v2;
        int v3 = _system.faces[face_index].v3;

        real g_now[3];
        pymemb::compute_form_factor_triangle(g_now, _system.vertices[v1].r, _system.vertices[v2].r, _system.vertices[v3].r, _system.get_box());
        edge_energy += ComputeVertexConstantAreaTriangleEnergy_lambda(g_now, sigma[type], target_area[type]);
    }
    return edge_energy;
}

real ComputeVertexConstantAreaTriangleEnergy::compute_vertex_energy(int query_vertex_index)
{

    real energy = 0.0;

    ///< get the triangle that this vertex is part of
    int he = _system.vertices[query_vertex_index]._hedge;
    int first = he;
    // std::cout<< "first " << first << "\n";
    int face_index, he_pair, he_pair_next;
    do
    {

        face_index = _system.halfedges[he].face;
        if (_system.faces[face_index].boundary == false) // Remember -1 is the virtual face outside of the mesh
        {
            int type = _system.faces[face_index].type;
            int v1 = _system.faces[face_index].v1;
            int v2 = _system.faces[face_index].v2;
            int v3 = _system.faces[face_index].v3;

            real g_now[3];
            pymemb::compute_form_factor_triangle(g_now, _system.vertices[v1].r, _system.vertices[v2].r, _system.vertices[v3].r, _system.get_box());
            energy += ComputeVertexConstantAreaTriangleEnergy_lambda(g_now, sigma[type], target_area[type]);
        }

        int he_prev = _system.halfedges[he].prev;
        he = _system.halfedges[he_prev].pair;
    } while ((he != first));
    return energy;
}

forceMatrix ComputeVertexConstantAreaTriangleForce_lambda(const real3 r1,
                                                          const real3 r2,
                                                          const real3 r3,
                                                          const real *__restrict__ g_now,
                                                          const real sigma,
                                                          const real target_area,
                                                          const BoxType _box)
{

    real triangle_area = pymemb::compute_area_triangle_from_metric(g_now);

    /*----------------------------------------------------------------------------------------------------------------*/
    /*-----------------------------------           FORCE MATRIX        ----------------------------------------------*/
    /*----------------------------------------------------------------------------------------------------------------*/
    /// capture the face type
    real force_factor = sigma * (triangle_area - target_area);

    auto r12 = pymemb::vector_subtract(r2, r1, _box);
    auto r13 = pymemb::vector_subtract(r3, r1, _box);

    real3 forceM11, forceM12;
    forceM11.x = forceM11.y = forceM11.z = 0.0;
    forceM12.x = forceM12.y = forceM12.z = 0.0;

    // T5
    forceM11.x += (0.25 * force_factor / triangle_area) * (g_now[2] * r12.x - g_now[1] * r13.x);
    forceM12.x += (0.25 * force_factor / triangle_area) * (g_now[0] * r13.x - g_now[1] * r12.x);
    forceM11.y += (0.25 * force_factor / triangle_area) * (g_now[2] * r12.y - g_now[1] * r13.y);
    forceM12.y += (0.25 * force_factor / triangle_area) * (g_now[0] * r13.y - g_now[1] * r12.y);
    forceM11.z += (0.25 * force_factor / triangle_area) * (g_now[2] * r12.z - g_now[1] * r13.z);
    forceM12.z += (0.25 * force_factor / triangle_area) * (g_now[0] * r13.z - g_now[1] * r12.z);

    forceM11.x *= -1.0;
    forceM12.x *= -1.0;
    forceM11.y *= -1.0;
    forceM12.y *= -1.0;
    forceM11.z *= -1.0;
    forceM12.z *= -1.0;

    forceMatrix result;

    result.forceM11 = forceM11;
    result.forceM12 = forceM12;

    return result;
}

void ComputeVertexConstantAreaTriangleForce_fn(const int Numfaces,
                                               HE_VertexProp *vertices,
                                               const HE_FaceProp *faces,
                                               const real *__restrict__ _sigma,
                                               const real *__restrict__ _target_area,
                                               const BoxType _box)
{
    for (int face_index = 0; face_index < Numfaces; face_index++)
    {
        int type = faces[face_index].type;
        int v1 = faces[face_index].v1;
        int v2 = faces[face_index].v2;
        int v3 = faces[face_index].v3;

        real g_now[3];
        pymemb::compute_form_factor_triangle(g_now, vertices[v1].r, vertices[v2].r, vertices[v3].r, _box);

        forceMatrix fval = ComputeVertexConstantAreaTriangleForce_lambda(vertices[v1].r,
                                                                         vertices[v2].r,
                                                                         vertices[v3].r,
                                                                         g_now,
                                                                         _sigma[type],
                                                                         _target_area[type],
                                                                         _box);

        /*----------------------------------------------------------------------------------------------------------------*/
        /*-----------------------------------           ACTUAL CALCULATION        ----------------------------------------*/
        /*----------------------------------------------------------------------------------------------------------------*/
        // v1
        vertices[v1].forceC.x += -1.0 * (fval.forceM11.x + fval.forceM12.x);
        vertices[v1].forceC.y += -1.0 * (fval.forceM11.y + fval.forceM12.y);
        vertices[v1].forceC.z += -1.0 * (fval.forceM11.z + fval.forceM12.z);

        // v2
        vertices[v2].forceC.x += fval.forceM11.x;
        vertices[v2].forceC.y += fval.forceM11.y;
        vertices[v2].forceC.z += fval.forceM11.z;

        // v3
        vertices[v3].forceC.x += fval.forceM12.x;
        vertices[v3].forceC.y += fval.forceM12.y;
        vertices[v3].forceC.z += fval.forceM12.z;
    }
}

void ComputeVertexConstantAreaTriangleEnergy::compute(void)
{

    ComputeVertexConstantAreaTriangleForce_fn(_system.Numfaces,
                                              &_system.vertices[0],
                                              &_system.faces[0],
                                              &sigma[0],
                                              &target_area[0],
                                              _system.get_box());
}

void ComputeVertexConstantAreaTriangleEnergyStress_fn(const int Numfaces,
                                                      HE_VertexProp *vertices,
                                                      const HE_FaceProp *faces,
                                                      const real *__restrict__ _sigma,
                                                      const real *__restrict__ _target_area,
                                                      realTensor *stress_group_faces,
                                                      const BoxType _box)
{

    for (int face_index = 0; face_index < Numfaces; face_index++)
    {
        int type = faces[face_index].type;
        int v1 = faces[face_index].v1;
        int v2 = faces[face_index].v2;
        int v3 = faces[face_index].v3;

        real g_now[3];
        pymemb::compute_form_factor_triangle(g_now, vertices[v1].r, vertices[v2].r, vertices[v3].r, _box);

        forceMatrix fval = ComputeVertexConstantAreaTriangleForce_lambda(vertices[v1].r,
                                                                         vertices[v2].r,
                                                                         vertices[v3].r,
                                                                         g_now,
                                                                         _sigma[type],
                                                                         _target_area[type],
                                                                         _box);
        // J. Chem. Phys. 131, 154107 (2009) page 4 Eq. 21
        // Assume that v1 is in the local replica then construct the r2, r3 based on it

        real3 r1 = vertices[v1].r;
        real3 r2 = vertices[v2].r;
        real3 r3 = vertices[v3].r;

        real3 r12, r13;
        r12 = pymemb::vector_subtract(r2, r1, _box);
        r13 = pymemb::vector_subtract(r3, r1, _box);
        real3 uw_r3, uw_r2 /*,uw_r1*/;
        // uw_r1 = r1;
        uw_r2 = pymemb::vector_sum(r1, r12);
        uw_r3 = pymemb::vector_sum(r1, r13);

        real3 F3, F2, F1;

        F1.x = -1.0 * (fval.forceM11.x + fval.forceM12.x);
        F1.y = -1.0 * (fval.forceM11.y + fval.forceM12.y);
        F1.z = -1.0 * (fval.forceM11.z + fval.forceM12.z);

        F2.x = fval.forceM11.x;
        F2.y = fval.forceM11.y;
        F2.z = fval.forceM11.z;

        F3.x = fval.forceM12.x;
        F3.y = fval.forceM12.y;
        F3.z = fval.forceM12.z;

        stress_group_faces[face_index].xx += r1.x * F1.x + uw_r2.x * F2.x + uw_r3.x * F3.x;
        stress_group_faces[face_index].xy += r1.x * F1.y + uw_r2.x * F2.y + uw_r3.x * F3.y;
        stress_group_faces[face_index].xz += r1.x * F1.z + uw_r2.x * F2.z + uw_r3.x * F3.z;

        stress_group_faces[face_index].yx += r1.y * F1.x + uw_r2.y * F2.x + uw_r3.y * F3.x;
        stress_group_faces[face_index].yy += r1.y * F1.y + uw_r2.y * F2.y + uw_r3.y * F3.y;
        stress_group_faces[face_index].yz += r1.y * F1.z + uw_r2.y * F2.z + uw_r3.y * F3.z;

        stress_group_faces[face_index].zx += r1.z * F1.x + uw_r2.z * F2.x + uw_r3.z * F3.x;
        stress_group_faces[face_index].zy += r1.z * F1.y + uw_r2.z * F2.y + uw_r3.z * F3.y;
        stress_group_faces[face_index].zz += r1.z * F1.z + uw_r2.z * F2.z + uw_r3.z * F3.z;
    }
}

void ComputeVertexConstantAreaTriangleEnergy::compute_stress(void)
{

    ComputeVertexConstantAreaTriangleEnergyStress_fn(_system.Numfaces,
                                                     &_system.vertices[0],
                                                     &_system.faces[0],
                                                     &sigma[0],
                                                     &target_area[0],
                                                     &_system.stress_group_faces[0],
                                                     _system.get_box());
}

void ComputeVertexConstantAreaTriangleEnergyStressAtom_fn(const int Numfaces,
                                                          HE_VertexProp *vertices,
                                                          const HE_FaceProp *faces,
                                                          const real *__restrict__ _sigma,
                                                          const real *__restrict__ _target_area,
                                                          realTensor *stress_virial_atom,
                                                          const BoxType _box)
{

    for (int face_index = 0; face_index < Numfaces; face_index++)
    {
        int type = faces[face_index].type;
        int v1 = faces[face_index].v1;
        int v2 = faces[face_index].v2;
        int v3 = faces[face_index].v3;

        real g_now[3];
        pymemb::compute_form_factor_triangle(g_now, vertices[v1].r, vertices[v2].r, vertices[v3].r, _box);

        forceMatrix fval = ComputeVertexConstantAreaTriangleForce_lambda(vertices[v1].r,
                                                                         vertices[v2].r,
                                                                         vertices[v3].r,
                                                                         g_now,
                                                                         _sigma[type],
                                                                         _target_area[type],
                                                                         _box);
        // J. Chem. Phys. 131, 154107 (2009) page 4 Eq. 21
        // Assume that v1 is in the local replica then construct the r2, r3 based on it

        real3 r1 = vertices[v1].r;
        real3 r2 = vertices[v2].r;
        real3 r3 = vertices[v3].r;

        real3 r12, r13;
        r12 = pymemb::vector_subtract(r2, r1, _box);
        r13 = pymemb::vector_subtract(r3, r1, _box);
        real3 uw_r3, uw_r2 /*,uw_r1*/;
        // uw_r1 = r1;
        uw_r2 = pymemb::vector_sum(r1, r12);
        uw_r3 = pymemb::vector_sum(r1, r13);

        real3 F3, F2, F1;

        F1.x = -1.0 * (fval.forceM11.x + fval.forceM12.x);
        F1.y = -1.0 * (fval.forceM11.y + fval.forceM12.y);
        F1.z = -1.0 * (fval.forceM11.z + fval.forceM12.z);

        F2.x = fval.forceM11.x;
        F2.y = fval.forceM11.y;
        F2.z = fval.forceM11.z;

        F3.x = fval.forceM12.x;
        F3.y = fval.forceM12.y;
        F3.z = fval.forceM12.z;

        realTensor stress_group_face;
        stress_group_face.xx = r1.x * F1.x + uw_r2.x * F2.x + uw_r3.x * F3.x;
        stress_group_face.xy = r1.x * F1.y + uw_r2.x * F2.y + uw_r3.x * F3.y;
        stress_group_face.xz = r1.x * F1.z + uw_r2.x * F2.z + uw_r3.x * F3.z;

        stress_group_face.yx = r1.y * F1.x + uw_r2.y * F2.x + uw_r3.y * F3.x;
        stress_group_face.yy = r1.y * F1.y + uw_r2.y * F2.y + uw_r3.y * F3.y;
        stress_group_face.yz = r1.y * F1.z + uw_r2.y * F2.z + uw_r3.y * F3.z;

        stress_group_face.zx = r1.z * F1.x + uw_r2.z * F2.x + uw_r3.z * F3.x;
        stress_group_face.zy = r1.z * F1.y + uw_r2.z * F2.y + uw_r3.z * F3.y;
        stress_group_face.zz = r1.z * F1.z + uw_r2.z * F2.z + uw_r3.z * F3.z;

        int vvec[3] = {v1, v2, v3};

        for (auto v : vvec)
        {
            stress_virial_atom[v].xx += stress_group_face.xx / 3.0;
            stress_virial_atom[v].xy += stress_group_face.xy / 3.0;
            stress_virial_atom[v].xz += stress_group_face.xz / 3.0;
            stress_virial_atom[v].yx += stress_group_face.yx / 3.0;
            stress_virial_atom[v].yy += stress_group_face.yy / 3.0;
            stress_virial_atom[v].yz += stress_group_face.yz / 3.0;
            stress_virial_atom[v].zx += stress_group_face.zx / 3.0;
            stress_virial_atom[v].zy += stress_group_face.zy / 3.0;
            stress_virial_atom[v].zz += stress_group_face.zz / 3.0;
        }
    }
}

void ComputeVertexConstantAreaTriangleEnergy::compute_atomic_stress(void)
{

    ComputeVertexConstantAreaTriangleEnergyStressAtom_fn(_system.Numfaces,
                                                         &_system.vertices[0],
                                                         &_system.faces[0],
                                                         &sigma[0],
                                                         &target_area[0],
                                                         &_system.stress_virial_atom[0],
                                                         _system.get_box());
}