
.. _program_listing_file_pymembrane_cppmodule_src_potentials_potentialBendingHelfrich.cpp:

Program Listing for File potentialBendingHelfrich.cpp
=====================================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_potentials_potentialBendingHelfrich.cpp>` (``pymembrane/cppmodule/src/potentials/potentialBendingHelfrich.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "potentialBendingHelfrich.hpp"
   
   // update face global index, normal vector, area and cotangent
   void update_face(const real3 *local_vertices,
                    real *local_cot,
                    real *local_area,
                    real3 *local_sigma,
                    real &At,
                    real3 &nf,
                    const BoxType &_box)
   {
       nf = pymemb::compute_normal_triangle(local_vertices[0], local_vertices[1], local_vertices[2], _box);
       At = 0.5 * sqrt(vdot(nf, nf));
       for (int i = 0; i < 3; ++i)
       {
           int k = (i + 1) % 3, l = (i + 2) % 3;
           real3 ri = local_vertices[i];
           real3 rk = local_vertices[k];
           real3 rl = local_vertices[l];
           auto ril = pymemb::vector_subtract(ri, rl, _box);
           auto rik = pymemb::vector_subtract(ri, rk, _box);
           auto rkl = pymemb::vector_subtract(rk, rl, _box);
           local_cot[k] = -vdot(rik, rkl) / 2. / At;
           local_cot[l] = vdot(ril, rkl) / 2. / At;
           Xvec2(local_sigma[i], 0.5 * local_cot[k], ril, 0.5 * local_cot[l], rik);
           local_area[i] = 0.125 * local_cot[k] * vdot(ril, ril) + 0.125 * local_cot[l] * vdot(rik, rik);
       }
   }
   
   void compute_mean_curvature_operature(int Numvertices,
                                         int Numfaces,
                                         const HE_VertexProp *vertices,
                                         const HE_FaceProp *faces,
                                         real3 *meanH_operator,
                                         real *meanH,
                                         const BoxType &_box)
   {
       std::vector<real> global_area(Numvertices);
       std::vector<real3> global_sigma(Numvertices);
       std::vector<real3> global_normal(Numvertices);
   
       real At;
       real3 nf;
   
       for (int face_index = 0; face_index < Numfaces; face_index++)
       {
           int local_index[3];
           real3 local_vertices[3];
           real local_cot[3];
           real local_area[3];
           real3 local_sigma[3];
   
           local_index[0] = faces[face_index].v1;
           local_index[1] = faces[face_index].v2;
           local_index[2] = faces[face_index].v3;
   
           local_vertices[0] = vertices[local_index[0]].r;
           local_vertices[1] = vertices[local_index[1]].r;
           local_vertices[2] = vertices[local_index[2]].r;
   
           update_face(local_vertices,
                       local_cot,
                       local_area,
                       local_sigma,
                       At,
                       nf,
                       _box);
           for (int i = 0; i < 3; ++i)
           {
               int vi = local_index[i];
   
               global_area[vi] += local_area[i];
               global_sigma[vi] = pymemb::vector_sum(global_sigma[vi], local_sigma[i]);
               global_normal[vi] = pymemb::vector_sum(global_normal[vi], nf);
           }
       }
       for (int vertex_index = 0; vertex_index < Numvertices; vertex_index++)
       {
           real vertex_area = global_area[vertex_index];
           real3 nv = global_normal[vertex_index];
           real3 sigma = global_sigma[vertex_index];
           real sign = (vdot(nv, sigma) > 0.) ? 1. : -1.;
           Xvec1(meanH_operator[vertex_index], 1. / vertex_area, sigma);
           meanH[vertex_index] = sign * sqrt(vdot(sigma, sigma)) / vertex_area;
       }
   }
   
   void ComputeVertexBendingHelfrichEnergyComputeForce_lambda(const HE_VertexProp *vertices,
                                                              const real3 *meanH_operator,
                                                              const real *meanH,
                                                              const int *local_index,
                                                              const real3 *local_vertices,
                                                              const real *kappaH,
                                                              const real *kappaG,
                                                              const real *H0,
                                                              const BoxType &_box,
                                                              real3 *vertex_force)
   {
       vertex_force[0].x = vertex_force[0].y = vertex_force[0].z = 0.0;
       vertex_force[1].x = vertex_force[1].y = vertex_force[1].z = 0.0;
       vertex_force[2].x = vertex_force[2].y = vertex_force[2].z = 0.0;
   
       real local_cot[3];
       real local_area[3];
       real3 local_sigma[3];
       real At;
       real3 nf;
   
       update_face(local_vertices,
                   local_cot,
                   local_area,
                   local_sigma,
                   At,
                   nf,
                   _box);
   
       int vi, vj, vk, vl;
       real a, H, H_0, k_b, k_g, cot_i, cot_j, cot_k, cot_l;
       real3 ri, rj, rk, rl, rij, rjk, ril, rik, rkl;
       real3 Hop, sigmai, sigmaj, dA, force1, force2, forceg;
   
       for (int i = 0; i < 3; ++i)
       {
           // force on i:
           vi = local_index[i];
           ri = vertices[vi].r;
           sigmai = local_sigma[i];
           for (int j = 0; j < 3; ++j)
           {
               a = local_area[j];
               sigmaj = local_sigma[j];
               Hop = meanH_operator[local_index[j]];
               H = meanH[local_index[j]];
               int type = vertices[local_index[j]].type;
               H_0 = H0[type];
               k_b = kappaH[type];
               k_g = kappaG[type];
               vj = local_index[j];
               cot_j = local_cot[j];
               if (i == j)
               {
                   vk = local_index[(i + 1) % 3];
                   vl = local_index[(i + 2) % 3];
                   rk = vertices[vk].r;
                   rl = vertices[vl].r;
                   ril = pymemb::vector_subtract(ri, rl, _box);
                   rik = pymemb::vector_subtract(ri, rk, _box);
                   rkl = pymemb::vector_subtract(rk, rl, _box);
                   cot_k = local_cot[(i + 1) % 3];
                   cot_l = local_cot[(i + 2) % 3];
                   Xvec3(dA,
                         (1. - a / At), sigmai,
                         -0.125 * (cot_k + cot_l), rik,
                         -0.125 * (cot_k + cot_l), ril);
                   Xvec1(force1,
                         0.5 * (H - 2. * H_0) * (H + 2. * H_0), dA);
                   Xvec3(force2,
                         -1.0 * vdot(Hop, sigmai), sigmai,
                         +0.25 * vdot(rkl, rkl), Hop,
                         -0.25 * vdot(rkl, Hop), rkl);
                   if (vdot(force2, force2) > 0.)
                       aXvec(-(H - 2. * H_0) / (H * At), force2);
                   
                   Xvec2(forceg,
                         -1.0 * cot_k / vdot(rik, rik), rik,
                         -1.0 * cot_l / vdot(ril, ril), ril);
               }
               else
               {
                   vj = local_index[j];
                   vk = local_index[3 - i - j];
                   rj = vertices[vj].r;
                   rk = vertices[vk].r;
                   rij = pymemb::vector_subtract(ri, rj, _box);
                   rik = pymemb::vector_subtract(ri, rk, _box);
                   rjk = pymemb::vector_subtract(rj, rk, _box);
                   cot_i = local_cot[i];
                   cot_k = local_cot[3 - i - j];
                   Xvec3(dA,
                         (0.5 - a / At), sigmai,
                         +0.125 * (2 * cot_k), rik,
                         +0.125 * (cot_i - cot_k), rjk);
                   Xvec1(force1,
                         0.5 * (H - 2. * H_0) * (H + 2. * H_0), dA);
                   Xvec4(force2,
                         +1.0 * vdot(Hop, sigmaj), sigmai,
                         -0.5 * vdot(Hop, rjk), rik,
                         +0.25 * vdot(Hop, rik), rjk,
                         +0.25 * vdot(rjk, rik), Hop);
   
                   if (vdot(force2, force2) > 0.)
                       aXvec((H - 2. * H_0) / (H * At), force2);
                   
                   Xvec2(forceg,
                         0.5 / At, rjk,
                         cot_j / vdot(rij, rij), rij);
               }
               vertex_force[i].x += k_b * (force1.x + force2.x) + k_g * forceg.x;
               vertex_force[i].y += k_b * (force1.y + force2.y) + k_g * forceg.y;
               vertex_force[i].z += k_b * (force1.z + force2.z) + k_g * forceg.z;
           }
       }
   }
   
   void ComputeVertexBendingHelfrichEnergy::compute(void)
   {
   
       // std::vector<real3> force(3);
       std::vector<real3> meanH_operator(_system.Numvertices);
       std::vector<real> meanH(_system.Numvertices);
       // std::vector<real3> forces(_system.Numvertices);
   
       compute_mean_curvature_operature(_system.Numvertices,
                                        _system.Numfaces,
                                        &_system.vertices[0],
                                        &_system.faces[0],
                                        &meanH_operator[0],
                                        &meanH[0],
                                        _system.get_box());
   
       for (int face_index = 0; face_index < _system.Numfaces; face_index++)
       {
           int local_index[3];
           local_index[0] = _system.faces[face_index].v1;
           local_index[1] = _system.faces[face_index].v2;
           local_index[2] = _system.faces[face_index].v3;
   
           real3 local_vertices[3];
           local_vertices[0] = _system.vertices[local_index[0]].r;
           local_vertices[1] = _system.vertices[local_index[1]].r;
           local_vertices[2] = _system.vertices[local_index[2]].r;
   
           // Force over the vertices of the face
           real3 vertex_force[3];
           ComputeVertexBendingHelfrichEnergyComputeForce_lambda(&_system.vertices[0],
                                                                 &meanH_operator[0],
                                                                 &meanH[0],
                                                                 &local_index[0],
                                                                 &local_vertices[0],
                                                                 &m_kappaH[0],
                                                                 &m_kappaG[0],
                                                                 &m_H0[0],
                                                                 _system.get_box(),
                                                                 &vertex_force[0]);
   
           _system.vertices[local_index[0]].forceC.x += vertex_force[0].x;
           _system.vertices[local_index[0]].forceC.y += vertex_force[0].y;
           _system.vertices[local_index[0]].forceC.z += vertex_force[0].z;
   
           _system.vertices[local_index[1]].forceC.x += vertex_force[1].x;
           _system.vertices[local_index[1]].forceC.y += vertex_force[1].y;
           _system.vertices[local_index[1]].forceC.z += vertex_force[1].z;
   
           _system.vertices[local_index[2]].forceC.x += vertex_force[2].x;
           _system.vertices[local_index[2]].forceC.y += vertex_force[2].y;
           _system.vertices[local_index[2]].forceC.z += vertex_force[2].z;
       }
   }
   
   real compute_vertex_energy_fn(int query_vertex_index,
                                 const HE_VertexProp *vertices,
                                 const HE_HalfEdgeProp *halfedges,
                                 const real *_kappaH,
                                 const real *_kappaG,
                                 const real *_H0,
                                 const BoxType &_box)
   {
       real3 sigma, nv, nf;
       nv.x = nv.y = nv.z = 0.0;
       sigma.x = sigma.y = sigma.z = 0.0;
       real gaussian_curv = 2.0 * defPI;
       real vertex_area = 0.0;
       int v0 = query_vertex_index, v1, v2;
       int he = vertices[query_vertex_index]._hedge;
       int first = he;
       do
       {
           int edge_index = halfedges[he].edge;
           if (halfedges[he].boundary == false)
           {
               v1 = halfedges[he].vert_to;
               int he_next = halfedges[he].next;
               v2 = halfedges[he_next].vert_to;
               nf = pymemb::compute_normal_triangle(vertices[v0].r, vertices[v1].r, vertices[v2].r, _box);
               nv.x += nf.x;
               nv.y += nf.y;
               nv.z += nf.z;
   
               real3 r0 = vertices[v0].r;
               real3 r1 = vertices[v1].r;
               real3 r2 = vertices[v2].r;
               auto r01 = pymemb::vector_subtract(r0, r1, _box);
               auto r02 = pymemb::vector_subtract(r0, r2, _box);
               auto r10 = pymemb::vector_subtract(r1, r0, _box);
               auto r12 = pymemb::vector_subtract(r1, r2, _box);
               auto r20 = pymemb::vector_subtract(r2, r0, _box);
               auto r21 = pymemb::vector_subtract(r2, r1, _box);
               real r01_dot_r02 = vdot(r01, r02);
               real r10_dot_r12 = vdot(r10, r12);
               real r20_dot_r21 = vdot(r20, r21);
               real3 r01_cross_r02, r10_cross_r12, r20_cross_r21;
               vcross(r01_cross_r02, r01, r02);
               vcross(r10_cross_r12, r10, r12);
               vcross(r20_cross_r21, r21, r20);
               real r01_cross_r02n = sqrt(vdot(r01_cross_r02, r01_cross_r02));
               real r10_cross_r12n = sqrt(vdot(r10_cross_r12, r10_cross_r12));
               real r20_cross_r21n = sqrt(vdot(r20_cross_r21, r20_cross_r21));
               real cot_alpha = r10_dot_r12 / r10_cross_r12n;
               real cot_beta = r20_dot_r21 / r20_cross_r21n;
               real cot_weight = 0.5 * (cot_alpha + cot_beta);
               sigma.x += 0.5 * cot_alpha * r02.x + 0.5 * cot_beta * r01.x;
               sigma.y += 0.5 * cot_alpha * r02.y + 0.5 * cot_beta * r01.y;
               sigma.z += 0.5 * cot_alpha * r02.z + 0.5 * cot_beta * r01.z;
   
               // vertex_area += 0.125 * vdot(r02, r02) * cot_alpha + 0.125 * vdot(r01, r01) * cot_beta;
               if (r01_dot_r02 < 0 || r10_dot_r12 < 0 || r20_dot_r21 < 0)
               {
                   if (r01_dot_r02 < 0)
                       vertex_area += 0.5 * r01_cross_r02n;
                   else
                       vertex_area += 0.25 * r01_cross_r02n;
               }
               else
                   vertex_area += 0.125 * vdot(r02, r02) * cot_alpha + 0.125 * vdot(r01, r01) * cot_beta;
   
               gaussian_curv -= acos(r01_dot_r02 / sqrt(vdot(r01, r01) * vdot(r02, r02)));
           }
           int he_pair = halfedges[he].pair;
           he = halfedges[he_pair].next;
       } while ((he != first));
   
       int type = vertices[query_vertex_index].type;
       real sign = (vdot(nv, sigma) > 0.) ? 1. : -1.;
       real H = sign * sqrt(vdot(sigma, sigma)) / vertex_area;
       real delH = H - 2.0 * _H0[type];
       return (0.5 * _kappaH[type] * delH * delH * vertex_area + _kappaG[type] * gaussian_curv);
   }
   
   void ComputeVertexBendingHelfrichEnergy::compute_energy(void)
   {
       for (int vertex_index = 0; vertex_index < _system.Numvertices; vertex_index++)
       {
           real energy = compute_vertex_energy_fn(vertex_index,
                                                  &_system.vertices[0],
                                                  &_system.halfedges[0],
                                                  &m_kappaH[0],
                                                  &m_kappaG[0],
                                                  &m_H0[0],
                                                  _system.get_box());
           _system.vertices[vertex_index].energy += energy;
       }
   }
   
   real ComputeVertexBendingHelfrichEnergy::compute_edge_energy(int query_edge_index)
   {
       pymemb::vector<int> v_index_vec(4);
       v_index_vec[0] = _system.edges[query_edge_index].v0;
       v_index_vec[1] = _system.edges[query_edge_index].v1;
       v_index_vec[2] = _system.edges[query_edge_index].v2;
       v_index_vec[3] = _system.edges[query_edge_index].v3;
       real edge_energy = 0.0;
       for (auto v_index : v_index_vec)
       {
           edge_energy += compute_vertex_energy_fn(v_index,
                                                   &_system.vertices[0],
                                                   &_system.halfedges[0],
                                                   &m_kappaH[0],
                                                   &m_kappaG[0],
                                                   &m_H0[0],
                                                   _system.get_box());
       }
   
       return (edge_energy);
   }
   
   // energy of query vertex and its neighbor vertices
   real ComputeVertexBendingHelfrichEnergy::compute_vertex_energy(int query_vertex_index)
   {
       real energy = compute_vertex_energy_fn(query_vertex_index,
                                              &_system.vertices[0],
                                              &_system.halfedges[0],
                                              &m_kappaH[0],
                                              &m_kappaG[0],
                                              &m_H0[0],
                                              _system.get_box());
       int he = _system.vertices[query_vertex_index]._hedge;
       int first = he;
       do
       {
           energy += compute_vertex_energy_fn(_system.halfedges[he].vert_to,
                                              &_system.vertices[0],
                                              &_system.halfedges[0],
                                              &m_kappaH[0],
                                              &m_kappaG[0],
                                              &m_H0[0],
                                              _system.get_box());
           int he_pair = _system.halfedges[he].pair;
           he = _system.halfedges[he_pair].next;
       } while ((he != first));
       return energy;
   }
   
   void ComputeVertexBendingHelfrichEnergy::compute_stress(void)
   {
       // std::vector<real3> force(3);
       std::vector<real3> meanH_operator(_system.Numvertices);
       std::vector<real> meanH(_system.Numvertices);
       // std::vector<real3> forces(_system.Numvertices);
   
       compute_mean_curvature_operature(_system.Numvertices,
                                        _system.Numfaces,
                                        &_system.vertices[0],
                                        &_system.faces[0],
                                        &meanH_operator[0],
                                        &meanH[0],
                                        _system.get_box());
   
       for (int face_index = 0; face_index < _system.Numfaces; face_index++)
       {
           int local_index[3];
           local_index[0] = _system.faces[face_index].v1;
           local_index[1] = _system.faces[face_index].v2;
           local_index[2] = _system.faces[face_index].v3;
   
           real3 local_vertices[3];
           local_vertices[0] = _system.vertices[local_index[0]].r;
           local_vertices[1] = _system.vertices[local_index[1]].r;
           local_vertices[2] = _system.vertices[local_index[2]].r;
   
           // Force over the vertices of the face
           real3 vertex_force[3];
           ComputeVertexBendingHelfrichEnergyComputeForce_lambda(&_system.vertices[0],
                                                                 &meanH_operator[0],
                                                                 &meanH[0],
                                                                 &local_index[0],
                                                                 &local_vertices[0],
                                                                 &m_kappaH[0],
                                                                 &m_kappaG[0],
                                                                 &m_H0[0],
                                                                 _system.get_box(),
                                                                 &vertex_force[0]);
   
           // J. Chem. Phys. 131, 154107 (2009) page 4 Eq. 21
           // Assume that v1 is in the local replica then construct the r2, r3 based on it
           real3 r1 = local_vertices[0];
           real3 r2 = local_vertices[1];
           real3 r3 = local_vertices[2];
   
           real3 r12, r13;
           r12 = pymemb::vector_subtract(r2, r1, _system.get_box());
           r13 = pymemb::vector_subtract(r3, r1, _system.get_box());
           real3 uw_r3, uw_r2 /*,uw_r1*/;
           // uw_r1 = r1;
           uw_r2 = pymemb::vector_sum(r1, r12);
           uw_r3 = pymemb::vector_sum(r1, r13);
   
           real3 F3, F2, F1;
   
           F1 = vertex_force[0];
           F2 = vertex_force[1];
           F3 = vertex_force[2];
   
           _system.stress_group_faces[face_index].xx += r1.x * F1.x + uw_r2.x * F2.x + uw_r3.x * F3.x;
           _system.stress_group_faces[face_index].xy += r1.x * F1.y + uw_r2.x * F2.y + uw_r3.x * F3.y;
           _system.stress_group_faces[face_index].xz += r1.x * F1.z + uw_r2.x * F2.z + uw_r3.x * F3.z;
   
           _system.stress_group_faces[face_index].yx += r1.y * F1.x + uw_r2.y * F2.x + uw_r3.y * F3.x;
           _system.stress_group_faces[face_index].yy += r1.y * F1.y + uw_r2.y * F2.y + uw_r3.y * F3.y;
           _system.stress_group_faces[face_index].yz += r1.y * F1.z + uw_r2.y * F2.z + uw_r3.y * F3.z;
   
           _system.stress_group_faces[face_index].zx += r1.z * F1.x + uw_r2.z * F2.x + uw_r3.z * F3.x;
           _system.stress_group_faces[face_index].zy += r1.z * F1.y + uw_r2.z * F2.y + uw_r3.z * F3.y;
           _system.stress_group_faces[face_index].zz += r1.z * F1.z + uw_r2.z * F2.z + uw_r3.z * F3.z;
       }
   }
   void ComputeVertexBendingHelfrichEnergy::compute_atomic_stress(void)
   {
       // std::vector<real3> force(3);
       std::vector<real3> meanH_operator(_system.Numvertices);
       std::vector<real> meanH(_system.Numvertices);
       // std::vector<real3> forces(_system.Numvertices);
   
       compute_mean_curvature_operature(_system.Numvertices,
                                        _system.Numfaces,
                                        &_system.vertices[0],
                                        &_system.faces[0],
                                        &meanH_operator[0],
                                        &meanH[0],
                                        _system.get_box());
   
       for (int face_index = 0; face_index < _system.Numfaces; face_index++)
       {
           int local_index[3];
           local_index[0] = _system.faces[face_index].v1;
           local_index[1] = _system.faces[face_index].v2;
           local_index[2] = _system.faces[face_index].v3;
   
           real3 local_vertices[3];
           local_vertices[0] = _system.vertices[local_index[0]].r;
           local_vertices[1] = _system.vertices[local_index[1]].r;
           local_vertices[2] = _system.vertices[local_index[2]].r;
   
           // Force over the vertices of the face
           real3 vertex_force[3];
           ComputeVertexBendingHelfrichEnergyComputeForce_lambda(&_system.vertices[0],
                                                                 &meanH_operator[0],
                                                                 &meanH[0],
                                                                 &local_index[0],
                                                                 &local_vertices[0],
                                                                 &m_kappaH[0],
                                                                 &m_kappaG[0],
                                                                 &m_H0[0],
                                                                 _system.get_box(),
                                                                 &vertex_force[0]);
   
           // J. Chem. Phys. 131, 154107 (2009) page 4 Eq. 21
           // Assume that v1 is in the local replica then construct the r2, r3 based on it
           real3 r1 = local_vertices[0];
           real3 r2 = local_vertices[1];
           real3 r3 = local_vertices[2];
   
           real3 r12, r13;
           r12 = pymemb::vector_subtract(r2, r1, _system.get_box());
           r13 = pymemb::vector_subtract(r3, r1, _system.get_box());
           real3 uw_r3, uw_r2 /*,uw_r1*/;
           // uw_r1 = r1;
           uw_r2 = pymemb::vector_sum(r1, r12);
           uw_r3 = pymemb::vector_sum(r1, r13);
   
           real3 F3, F2, F1;
   
           F1 = vertex_force[0];
           F2 = vertex_force[1];
           F3 = vertex_force[2];
   
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
   
           for (auto v : local_index)
           {
               _system.stress_virial_atom[v].xx += stress_group_face.xx / 3.0;
               _system.stress_virial_atom[v].xy += stress_group_face.xy / 3.0;
               _system.stress_virial_atom[v].xz += stress_group_face.xz / 3.0;
               _system.stress_virial_atom[v].yx += stress_group_face.yx / 3.0;
               _system.stress_virial_atom[v].yy += stress_group_face.yy / 3.0;
               _system.stress_virial_atom[v].yz += stress_group_face.yz / 3.0;
               _system.stress_virial_atom[v].zx += stress_group_face.zx / 3.0;
               _system.stress_virial_atom[v].zy += stress_group_face.zy / 3.0;
               _system.stress_virial_atom[v].zz += stress_group_face.zz / 3.0;
           }
       }
   }
