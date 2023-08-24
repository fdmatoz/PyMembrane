
.. _program_listing_file_pymembrane_cppmodule_src_potentials_potentialBending.cpp:

Program Listing for File potentialBending.cpp
=============================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_potentials_potentialBending.cpp>` (``pymembrane/cppmodule/src/potentials/potentialBending.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "potentialBending.hpp"
   
   real ComputeVertexBendingEnergy_lambda(const real3 &nk,
                                          const real3 &nl,
                                          const real3 &nk_ref,
                                          const real3 &nl_ref,
                                          const real &kappa_tilde)
   {
       real norm_nk = sqrt(vdot(nk, nk));
       real norm_nl = sqrt(vdot(nl, nl));
   
       real nknl = vdot(nk, nl) / (norm_nk * norm_nl);
   
       real norm_nk_ref = sqrt(vdot(nk_ref, nk_ref));
       real norm_nl_ref = sqrt(vdot(nl_ref, nl_ref));
   
       real nknl_ref = vdot(nk_ref, nl_ref) / (norm_nk_ref * norm_nl_ref);
   
       real energy = kappa_tilde * (1.0 - nknl * nknl_ref - sqrt(fabs(1.0 - nknl * nknl)) * sqrt((1.0 - nknl_ref * nknl_ref)));
       // real energy = kappa_tilde*(1.0 - nknl);
   
       return energy;
   }
   
   void ComputeVertexBendingEnergy_fn(int Numedges,
                                      HE_EdgeProp *edges,
                                      const HE_FaceProp *faces,
                                      const HE_VertexProp *vertices,
                                      const real *_kappa,
                                      const BoxType _box)
   {
       for (int edge_index = 0; edge_index < Numedges; edge_index++)
       {
   
           if (edges[edge_index].boundary == false)
           {
               int type = edges[edge_index].type;
   
               real kappa_tilde = (2.0 / sqrt(3.0)) * (_kappa[type]); // kappa
   
               int v0 = edges[edge_index].v0;
               int v1 = edges[edge_index].v1;
               int v2 = edges[edge_index].v2;
               int v3 = edges[edge_index].v3;
               real3 r0 = vertices[v0].r;
               real3 r1 = vertices[v1].r;
               real3 r2 = vertices[v2].r;
               real3 r3 = vertices[v3].r;
   
               real3 nk, nl;
               nk = pymemb::compute_normal_triangle(r0, r1, r2, _box);
               nl = pymemb::compute_normal_triangle(r0, r2, r3, _box);
   
               real3 nk_ref, nl_ref;
               nk_ref = faces[edges[edge_index].face_k].normal_reference;
               nl_ref = faces[edges[edge_index].face_l].normal_reference;
   
               real energy = ComputeVertexBendingEnergy_lambda(nk, nl, nk_ref, nl_ref, kappa_tilde);
   
               edges[edge_index].energy += energy;
           }
       }
   }
   
   void ComputeVertexBendingEnergy::compute_energy(void)
   {
   
       ComputeVertexBendingEnergy_fn(_system.Numedges,
                                     &_system.edges[0],
                                     &_system.faces[0],
                                     &_system.vertices[0],
                                     &m_kappa[0],
                                     _system.get_box());
   }
   
   real ComputeVertexBendingEnergy::compute_edge_energy(int query_edge_index)
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
   
               real kappa_tilde = (2.0 / sqrt(3.0)) * (m_kappa[type]); // kappa
   
               int v0 = _system.edges[edge_index].v0;
               int v1 = _system.edges[edge_index].v1;
               int v2 = _system.edges[edge_index].v2;
               int v3 = _system.edges[edge_index].v3;
               real3 r0 = _system.vertices[v0].r;
               real3 r1 = _system.vertices[v1].r;
               real3 r2 = _system.vertices[v2].r;
               real3 r3 = _system.vertices[v3].r;
   
               real3 nk, nl;
               nk = pymemb::compute_normal_triangle(r0, r1, r2, _system.get_box());
               nl = pymemb::compute_normal_triangle(r0, r2, r3, _system.get_box());
   
               real3 nk_ref, nl_ref;
               nk_ref = _system.faces[_system.edges[edge_index].face_k].normal_reference;
               nl_ref = _system.faces[_system.edges[edge_index].face_l].normal_reference;
               edge_energy += ComputeVertexBendingEnergy_lambda(nk, nl, nk_ref, nl_ref, kappa_tilde);
           }
       }
       return edge_energy;
   }
   
   real ComputeVertexBendingEnergy::compute_vertex_energy(int query_vertex_index)
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
   
                   real kappa_tilde = (2.0 / sqrt(3.0)) * (m_kappa[type]); // kappa
   
                   int v0 = _system.edges[edge_index].v0;
                   int v1 = _system.edges[edge_index].v1;
                   int v2 = _system.edges[edge_index].v2;
                   int v3 = _system.edges[edge_index].v3;
                   real3 r0 = _system.vertices[v0].r;
                   real3 r1 = _system.vertices[v1].r;
                   real3 r2 = _system.vertices[v2].r;
                   real3 r3 = _system.vertices[v3].r;
   
                   real3 nk, nl;
                   nk = pymemb::compute_normal_triangle(r0, r1, r2, _system.get_box());
                   nl = pymemb::compute_normal_triangle(r0, r2, r3, _system.get_box());
   
                   real3 nk_ref, nl_ref;
                   nk_ref = _system.faces[_system.edges[edge_index].face_k].normal_reference;
                   nl_ref = _system.faces[_system.edges[edge_index].face_l].normal_reference;
                   energy += ComputeVertexBendingEnergy_lambda(nk, nl, nk_ref, nl_ref, kappa_tilde);
               }
           }
           int he_prev = _system.halfedges[he].prev;
           he = _system.halfedges[he_prev].pair;
       } while (he != first);
       return energy;
   }
   
   forceMatrix ComputeVertexBendingForce_lambda(const real3 &r0,
                                                const real3 &r1,
                                                const real3 &r2,
                                                const real3 &r3,
                                                const real &kappa_tilde,
                                                const BoxType &_box)
   {
   
       real3 r01, r02, r03;
       real3 nk, nl;
   
       real3 forceM11, forceM12, forceM13;
   
       nk = pymemb::compute_normal_triangle(r0, r1, r2, _box);
       real Ak = 0.5 * sqrt(vdot(nk, nk));
       nl = pymemb::compute_normal_triangle(r0, r2, r3, _box);
       real Al = 0.5 * sqrt(vdot(nl, nl));
       real s = vdot(nk, nl);
   
       // printf("Ak,Al (%f,%f)\n", Ak, Al);
   
       r01 = pymemb::vector_subtract(r1, r0, _box);
       r02 = pymemb::vector_subtract(r2, r0, _box);
       r03 = pymemb::vector_subtract(r3, r0, _box);
   
       real r01_dot_r02 = vdot(r01, r02);
       real r01_dot_r03 = vdot(r01, r03);
       real r02_dot_r03 = vdot(r02, r03);
       real r01_dot_r01 = vdot(r01, r01);
       real r02_dot_r02 = vdot(r02, r02);
       real r03_dot_r03 = vdot(r03, r03);
   
       forceM11.x = forceM11.y = forceM11.z = 0.0;
       forceM12.x = forceM12.y = forceM12.z = 0.0;
       forceM13.x = forceM13.y = forceM13.z = 0.0;
   
       //(r02 · r03) r02 − (r02 · r02) r03
       //(r02 · r03) r01 + (r01 · r02) r03 − 2 (r01 · r03) r02
       //(r01 · r02) r02 − (r02 · r02) r01
       forceM11.x += (r02_dot_r03)*r02.x - (r02_dot_r02)*r03.x;
       forceM11.y += (r02_dot_r03)*r02.y - (r02_dot_r02)*r03.y;
       forceM11.z += (r02_dot_r03)*r02.z - (r02_dot_r02)*r03.z;
   
       forceM12.x += (r02_dot_r03)*r01.x + (r01_dot_r02)*r03.x - 2.0 * (r01_dot_r03)*r02.x;
       forceM12.y += (r02_dot_r03)*r01.y + (r01_dot_r02)*r03.y - 2.0 * (r01_dot_r03)*r02.y;
       forceM12.z += (r02_dot_r03)*r01.z + (r01_dot_r02)*r03.z - 2.0 * (r01_dot_r03)*r02.z;
   
       forceM13.x += (r01_dot_r02)*r02.x - (r02_dot_r02)*r01.x;
       forceM13.y += (r01_dot_r02)*r02.y - (r02_dot_r02)*r01.y;
       forceM13.z += (r01_dot_r02)*r02.z - (r02_dot_r02)*r01.z;
   
       //(-s/(4*Ak*Ak))((r02 · r02) r01 − (r01 · r02) r02)
       //(-s/(4*Ak*Ak))(r01 · r01) r02 − (r01 · r02) r01
       // 0
       forceM11.x += (-s / (4.0 * Ak * Ak)) * ((r02_dot_r02)*r01.x - (r01_dot_r02)*r02.x);
       forceM11.y += (-s / (4.0 * Ak * Ak)) * ((r02_dot_r02)*r01.y - (r01_dot_r02)*r02.y);
       forceM11.z += (-s / (4 * Ak * Ak)) * ((r02_dot_r02)*r01.z - (r01_dot_r02)*r02.z);
   
       forceM12.x += (-s / (4.0 * Ak * Ak)) * ((r01_dot_r01)*r02.x - (r01_dot_r02)*r01.x);
       forceM12.y += (-s / (4.0 * Ak * Ak)) * ((r01_dot_r01)*r02.y - (r01_dot_r02)*r01.y);
       forceM12.z += (-s / (4.0 * Ak * Ak)) * ((r01_dot_r01)*r02.z - (r01_dot_r02)*r01.z);
   
       // 0
       //(-s/(4*Al*Al))((r03 · r03) r02 − (r02 · r03) r03)
       //(-s/(4*Al*Al))((r02 · r02) r03 − (r02 · r03) r02)
   
       forceM12.x += (-s / (4.0 * Al * Al)) * ((r03_dot_r03)*r02.x - (r02_dot_r03)*r03.x);
       forceM12.y += (-s / (4.0 * Al * Al)) * ((r03_dot_r03)*r02.y - (r02_dot_r03)*r03.y);
       forceM12.z += (-s / (4.0 * Al * Al)) * ((r03_dot_r03)*r02.z - (r02_dot_r03)*r03.z);
   
       forceM13.x += (-s / (4.0 * Al * Al)) * ((r02_dot_r02)*r03.x - (r02_dot_r03)*r02.x);
       forceM13.y += (-s / (4.0 * Al * Al)) * ((r02_dot_r02)*r03.y - (r02_dot_r03)*r02.y);
       forceM13.z += (-s / (4.0 * Al * Al)) * ((r02_dot_r02)*r03.z - (r02_dot_r03)*r02.z);
   
       real factor = kappa_tilde / (4.0 * Ak * Al);
       forceM11.x *= factor;
       forceM12.x *= factor;
       forceM13.x *= factor;
       forceM11.y *= factor;
       forceM12.y *= factor;
       forceM13.y *= factor;
       forceM11.z *= factor;
       forceM12.z *= factor;
       forceM13.z *= factor;
   
       forceMatrix result;
   
       result.forceM11 = forceM11;
       result.forceM12 = forceM12;
       result.forceM13 = forceM13;
   
       return result;
   }
   
   forceMatrix scale_BendingForceMatrix_lambda(const real3 nk,
                                               const real3 nl,
                                               const real3 nk_ref,
                                               const real3 nl_ref,
                                               forceMatrix fval)
   {
       real norm_nk = sqrt(vdot(nk, nk));
       real norm_nl = sqrt(vdot(nl, nl));
   
       real nknl = vdot(nk, nl) / (norm_nk * norm_nl);
       real fac_nknl = 1.0 - nknl * nknl;
   
       real norm_nk_ref = sqrt(vdot(nk_ref, nk_ref));
       real norm_nl_ref = sqrt(vdot(nl_ref, nl_ref));
   
       real nknl_ref = vdot(nk_ref, nl_ref) / (norm_nk_ref * norm_nl_ref);
       real fac_nknl_ref = 1.0 - nknl_ref * nknl_ref;
   
       real factor = 0.0;
       if (fac_nknl > 0.0 && fac_nknl_ref >= 0.0) // save guard for ridges and weird cases
       {
           factor = nknl_ref * (1.0 - sqrt(fac_nknl_ref / fac_nknl));
       }
       /*else
       {
           printf("err fac_nknl = %f  fac_nknl_ref = %f \n",fac_nknl, fac_nknl_ref);
       }*/
   
       fval.forceM11.x *= factor;
       fval.forceM12.x *= factor;
       fval.forceM13.x *= factor;
       fval.forceM11.y *= factor;
       fval.forceM12.y *= factor;
       fval.forceM13.y *= factor;
       fval.forceM11.z *= factor;
       fval.forceM12.z *= factor;
       fval.forceM13.z *= factor;
   
       return fval;
   }
   
   void ComputeVertexBendingForce_fn(int Numedges,
                                     const HE_EdgeProp *edges,
                                     const HE_FaceProp *faces,
                                     HE_VertexProp *vertices,
                                     const real *_kappa,
                                     const BoxType _box)
   {
       for (int edge_index = 0; edge_index < Numedges; edge_index++)
       {
   
           if (edges[edge_index].boundary == false)
           {
               int type = edges[edge_index].type;
   
               real kappa_tilde = (2.0 / sqrt(3.0)) * (_kappa[type]); // kappa
   
               int v0 = edges[edge_index].v0;
               int v1 = edges[edge_index].v1;
               int v2 = edges[edge_index].v2;
               int v3 = edges[edge_index].v3;
               real3 r0 = vertices[v0].r;
               real3 r1 = vertices[v1].r;
               real3 r2 = vertices[v2].r;
               real3 r3 = vertices[v3].r;
   
               forceMatrix gradEe = ComputeVertexBendingForce_lambda(r0, r1, r2, r3, kappa_tilde, _box);
   
               /* Scale matrix of force by reference configuration*/
               real3 nk, nl;
               nk = pymemb::compute_normal_triangle(r0, r1, r2);
               nl = pymemb::compute_normal_triangle(r0, r2, r3);
   
               real3 nk_ref, nl_ref;
               nk_ref = faces[edges[edge_index].face_k].normal_reference;
               nl_ref = faces[edges[edge_index].face_l].normal_reference;
               // std::cout<<"nk_ref:"<<nk_ref.x<<","<<nk_ref.y<<","<<nk_ref.z<<"\n"<<" nl_ref:"<<nl_ref.x<<","<<nl_ref.y<<","<<nl_ref.z<<std::endl;
               // forceMatrix fval = gradEe;//scale_BendingForceMatrix_lambda(nk ,nl, nk_ref, nl_ref, gradEe);
               forceMatrix fval = scale_BendingForceMatrix_lambda(nk, nl, nk_ref, nl_ref, gradEe);
   
               // v0
               vertices[v0].forceC.x += -fval.forceM11.x - fval.forceM12.x - fval.forceM13.x;
               vertices[v0].forceC.y += -fval.forceM11.y - fval.forceM12.y - fval.forceM13.y;
               vertices[v0].forceC.z += -fval.forceM11.z - fval.forceM12.z - fval.forceM13.z;
   
               // v1
               vertices[v1].forceC.x += fval.forceM11.x;
               vertices[v1].forceC.y += fval.forceM11.y;
               vertices[v1].forceC.z += fval.forceM11.z;
   
               // v2
               vertices[v2].forceC.x += fval.forceM12.x;
               vertices[v2].forceC.y += fval.forceM12.y;
               vertices[v2].forceC.z += fval.forceM12.z;
   
               // v3
               vertices[v3].forceC.x += fval.forceM13.x;
               vertices[v3].forceC.y += fval.forceM13.y;
               vertices[v3].forceC.z += fval.forceM13.z;
           }
       }
   }
   
   void ComputeVertexBendingEnergy::compute(void)
   {
   
       ComputeVertexBendingForce_fn(_system.Numedges,
                                    &_system.edges[0],
                                    &_system.faces[0],
                                    &_system.vertices[0],
                                    &m_kappa[0],
                                    _system.get_box());
   }
   
   
   void ComputeVertexBendingStress_fn(const int Numedges,
                                      HE_EdgeProp *edges,
                                      const HE_FaceProp *__restrict__ faces,
                                      const HE_VertexProp *__restrict__ vertices,
                                      const real *__restrict__ _kappa,
                                      realTensor *stress_group_edges,
                                      const BoxType _box)
   {
       for (int edge_index = 0; edge_index < Numedges; edge_index++)
       {
   
           if (edges[edge_index].boundary == false)
           {
               int type = edges[edge_index].type;
   
               // real kappa_tilde = (2.0 / sqrt(3.0)) * (_kappa[type]); //kappa
               real kappa_tilde = _kappa[type];
   
               int v0 = edges[edge_index].v0;
               int v1 = edges[edge_index].v1;
               int v2 = edges[edge_index].v2;
               int v3 = edges[edge_index].v3;
               real3 r0 = vertices[v0].r;
               real3 r1 = vertices[v1].r;
               real3 r2 = vertices[v2].r;
               real3 r3 = vertices[v3].r;
   
               forceMatrix gradEe = ComputeVertexBendingForce_lambda(r0, r1, r2, r3, kappa_tilde, _box);
   
               /* Scale matrix of force by reference configuration*/
               real3 nk, nl;
               nk = pymemb::compute_normal_triangle(r0, r1, r2, _box);
               nl = pymemb::compute_normal_triangle(r0, r2, r3, _box);
   
               real3 nk_ref, nl_ref;
               nk_ref = faces[edges[edge_index].face_k].normal_reference;
               nl_ref = faces[edges[edge_index].face_l].normal_reference;
   
               // forceMatrix fval = gradEe;//scale_BendingForceMatrix_lambda(nk ,nl, nk_ref, nl_ref, gradEe);
               forceMatrix fval = scale_BendingForceMatrix_lambda(nk, nl, nk_ref, nl_ref, gradEe);
               // if (edge_index < 10)
               //     printf("type = %i kappa_tilde = %f fval = (%f,%f,%f,%f,%f,%f,%f,%f,%f)\n", type, kappa_tilde, fval.forceM11.x, fval.forceM12.x, fval.forceM13.x, fval.forceM11.y, fval.forceM12.y, fval.forceM13.y, fval.forceM11.z, fval.forceM12.z, fval.forceM13.z);
   
               // This might be wrong so have to be checked
               // double check J. Chem. Phys. 131, 154107 (2009) page 4 Eq. 21
               // Assume that v0 is in the local replica then construct the r1, r2, r3 based on it
               real3 r01, r02, r03;
               r01 = pymemb::vector_subtract(r1, r0, _box);
               r02 = pymemb::vector_subtract(r2, r0, _box);
               r03 = pymemb::vector_subtract(r3, r0, _box);
               real3 uw_r3, uw_r2, uw_r1 /*,uw_r0,*/;
               // uw_r0 = r0;
               uw_r1 = pymemb::vector_sum(r0, r01);
               uw_r2 = pymemb::vector_sum(r0, r02);
               uw_r3 = pymemb::vector_sum(r0, r03);
   
               real3 F3, F2, F1, F0;
               F0.x = -fval.forceM11.x - fval.forceM12.x - fval.forceM13.x;
               F0.y = -fval.forceM11.y - fval.forceM12.y - fval.forceM13.y;
               F0.z = -fval.forceM11.z - fval.forceM12.z - fval.forceM13.z;
   
               F1.x = fval.forceM11.x;
               F1.y = fval.forceM11.y;
               F1.z = fval.forceM11.z;
   
               F2.x = fval.forceM12.x;
               F2.y = fval.forceM12.y;
               F2.z = fval.forceM12.z;
   
               F3.x = fval.forceM13.x;
               F3.y = fval.forceM13.y;
               F3.z = fval.forceM13.z;
   
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
   
   void ComputeVertexBendingEnergy::compute_stress(void)
   {
       ComputeVertexBendingStress_fn(_system.Numedges,
                                     &_system.edges[0],
                                     &_system.faces[0],
                                     &_system.vertices[0],
                                     &m_kappa[0],
                                     &_system.stress_group_edges[0],
                                     _system.get_box());
   }
   
   
   void ComputeVertexBendingStressAtom_fn(const int Numedges,
                                          HE_EdgeProp *edges,
                                          const HE_FaceProp *__restrict__ faces,
                                          const HE_VertexProp *__restrict__ vertices,
                                          const real *__restrict__ _kappa,
                                          realTensor *stress_virial_atom,
                                          const BoxType _box)
   {
       for (int edge_index = 0; edge_index < Numedges; edge_index++)
       {
   
           if (edges[edge_index].boundary == false)
           {
               int type = edges[edge_index].type;
   
               real kappa_tilde = (2.0 / sqrt(3.0)) * (_kappa[type]); // kappa
               // real kappa_tilde = _kappa[type];
   
               int v0 = edges[edge_index].v0;
               int v1 = edges[edge_index].v1;
               int v2 = edges[edge_index].v2;
               int v3 = edges[edge_index].v3;
               real3 r0 = vertices[v0].r;
               real3 r1 = vertices[v1].r;
               real3 r2 = vertices[v2].r;
               real3 r3 = vertices[v3].r;
   
               forceMatrix gradEe = ComputeVertexBendingForce_lambda(r0, r1, r2, r3, kappa_tilde, _box);
   
               /* Scale matrix of force by reference configuration*/
               real3 nk, nl;
               nk = pymemb::compute_normal_triangle(r0, r1, r2, _box);
               nl = pymemb::compute_normal_triangle(r0, r2, r3, _box);
   
               real3 nk_ref, nl_ref;
               nk_ref = faces[edges[edge_index].face_k].normal_reference;
               nl_ref = faces[edges[edge_index].face_l].normal_reference;
   
               // forceMatrix fval = gradEe;//scale_BendingForceMatrix_lambda(nk ,nl, nk_ref, nl_ref, gradEe);
               forceMatrix fval = scale_BendingForceMatrix_lambda(nk, nl, nk_ref, nl_ref, gradEe);
               // if (edge_index < 10)
               //     printf("type = %i kappa_tilde = %f fval = (%f,%f,%f,%f,%f,%f,%f,%f,%f)\n", type, kappa_tilde, fval.forceM11.x, fval.forceM12.x, fval.forceM13.x, fval.forceM11.y, fval.forceM12.y, fval.forceM13.y, fval.forceM11.z, fval.forceM12.z, fval.forceM13.z);
   
               // This might be wrong so have to be checked
               // double check J. Chem. Phys. 131, 154107 (2009) page 4 Eq. 21
               // Assume that v0 is in the local replica then construct the r1, r2, r3 based on it
               real3 r01, r02, r03;
               r01 = pymemb::vector_subtract(r1, r0, _box);
               r02 = pymemb::vector_subtract(r2, r0, _box);
               r03 = pymemb::vector_subtract(r3, r0, _box);
               real3 uw_r3, uw_r2, uw_r1 /*,uw_r0,*/;
               // uw_r0 = r0;
               uw_r1 = pymemb::vector_sum(r0, r01);
               uw_r2 = pymemb::vector_sum(r0, r02);
               uw_r3 = pymemb::vector_sum(r0, r03);
   
               real3 F3, F2, F1, F0;
               F0.x = -fval.forceM11.x - fval.forceM12.x - fval.forceM13.x;
               F0.y = -fval.forceM11.y - fval.forceM12.y - fval.forceM13.y;
               F0.z = -fval.forceM11.z - fval.forceM12.z - fval.forceM13.z;
   
               F1.x = fval.forceM11.x;
               F1.y = fval.forceM11.y;
               F1.z = fval.forceM11.z;
   
               F2.x = fval.forceM12.x;
               F2.y = fval.forceM12.y;
               F2.z = fval.forceM12.z;
   
               F3.x = fval.forceM13.x;
               F3.y = fval.forceM13.y;
               F3.z = fval.forceM13.z;
   
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
   #pragma unroll
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
   
   void ComputeVertexBendingEnergy::compute_atomic_stress(void)
   {
       ComputeVertexBendingStressAtom_fn(_system.Numedges,
                                         &_system.edges[0],
                                         &_system.faces[0],
                                         &_system.vertices[0],
                                         &m_kappa[0],
                                         &_system.stress_virial_atom[0],
                                         _system.get_box());
   }
