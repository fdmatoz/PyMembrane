
.. _program_listing_file_pymembrane_cppmodule_src_potentials_potentialBendingGompperKroll.cpp:

Program Listing for File potentialBendingGompperKroll.cpp
=========================================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_potentials_potentialBendingGompperKroll.cpp>` (``pymembrane/cppmodule/src/potentials/potentialBendingGompperKroll.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "potentialBendingGompperKroll.hpp"
   
   //update face global index, normal vector, area and cotangent
   void update_face(const std::vector<int> local_index,
                   const std::vector<real3> local_vertices,
                   std::vector<double> &local_cot,
                   std::vector<double> &local_area,
                   std::vector<real3> &local_sigma,
                   double &At, real3 &nf)
   {
       nf = pymemb::compute_normal_triangle(local_vertices[0], local_vertices[1], local_vertices[2]);
       At = 0.5*sqrt(vdot(nf,nf));
       for (int i = 0; i < 3; ++i)
       {
           int k=(i+1)%3, l=(i+2)%3;
           int vi = local_index[i];
           int vk = local_index[k];
           int vl = local_index[l];
           real3 ri = local_vertices[i];
           real3 rk = local_vertices[k];
           real3 rl = local_vertices[l];
           real3 ril,rik,rkl;
           vsub(ril, ri, rl);
           vsub(rik, ri, rk);
           vsub(rkl, rk, rl);
           local_cot[k] = -vdot(rik,rkl)/2./At;
           local_cot[l] =  vdot(ril,rkl)/2./At;
           Xvec2(local_sigma[i], 0.5 * local_cot[k], ril, 
                                 0.5 * local_cot[l], rik);
           local_area[i] =  0.125 * local_cot[k] * vdot(ril,ril) 
                          + 0.125 * local_cot[l] * vdot(rik,rik);
       }
   }
   
   void compute_mean_curvature_operature(
                       int Numvertices,
                       int Numfaces,
                       const HE_VertexProp *vertices,
                       const HE_FaceProp *faces,
                       std::vector<real3> &meanH_operator,
                       std::vector<double> &meanH)
   {
       std::vector<int> local_index (3);
       std::vector<real3> local_vertices (3);
       std::vector<double> local_cot (3);
       std::vector<double> local_area (3);
       std::vector<real3> local_sigma (3);
   
       std::vector<double> global_area (Numvertices);
       std::vector<real3> global_sigma (Numvertices);
       std::vector<real3> global_normal (Numvertices);
   
       double At;
       real3 nf;
   
       for (int face_index = 0; 
           face_index < Numfaces; 
           face_index ++)
       {
           local_index[0]=faces[face_index].v1;
           local_index[1]=faces[face_index].v2;
           local_index[2]=faces[face_index].v3;
   
           local_vertices[0]=vertices[local_index[0]].r;
           local_vertices[1]=vertices[local_index[1]].r;
           local_vertices[2]=vertices[local_index[2]].r;
   
           update_face(local_index,
                       local_vertices, 
                       local_cot, 
                       local_area, 
                       local_sigma, 
                       At,nf);
           for (int i = 0; i < 3; ++i)
           {
               int vi = local_index[i];
   
               global_area[vi] += local_area[i];
               vsum(global_sigma[vi],global_sigma[vi],local_sigma[i]);
               vsum(global_normal[vi],global_normal[vi],nf);
           }
       }
       for (int vertex_index = 0; vertex_index < Numvertices; vertex_index++)
       {
           double vertex_area = global_area[vertex_index];
           real3 nv = global_normal[vertex_index];
           real3 sigma = global_sigma[vertex_index];
           double sign = (vdot(nv, sigma) > 0.) ? 1. : -1.;
           Xvec1(meanH_operator[vertex_index], 1./vertex_area, sigma);
           meanH[vertex_index] = sign * sqrt(vdot(sigma, sigma)) / vertex_area;
       }
   }
   
   void ComputeVertexBendingGKEnergy::compute(void)
   {
       std::vector<int> local_index (3);
       std::vector<real3> local_vertices (3);
       std::vector<double> local_cot (3);
       std::vector<double> local_area (3);
       std::vector<real3> local_sigma (3);
       std::vector<real3> force (3);
       std::vector<real3> meanH_operator (_system.Numvertices);
       std::vector<double> meanH (_system.Numvertices);
       std::vector<real3> forces (_system.Numvertices);
   
       int vi,vj,vk,vl;
       double a,At,H,H_0,k_b,k_g,cot_i,cot_j,cot_k,cot_l;
       real3 ri,rj,rk,rl,rij,rjk,ril,rik,rkl;
       real3 Hop,sigmai,sigmaj,dA,nf,force1,force2,forceg;
   
       compute_mean_curvature_operature(_system.Numvertices,
                                       _system.Numfaces, 
                                       &_system.vertices[0],
                                       &_system.faces[0],
                                       meanH_operator,
                                       meanH);
       for (int face_index = 0; 
           face_index < _system.Numfaces; 
           face_index ++)
       {
           local_index[0]=_system.faces[face_index].v1;
           local_index[1]=_system.faces[face_index].v2;
           local_index[2]=_system.faces[face_index].v3;
   
           local_vertices[0]=_system.vertices[local_index[0]].r;
           local_vertices[1]=_system.vertices[local_index[1]].r;
           local_vertices[2]=_system.vertices[local_index[2]].r;
   
           update_face(local_index,
                       local_vertices, 
                       local_cot, 
                       local_area, 
                       local_sigma, 
                       At,nf);
   
           for (int i = 0; i < 3; ++i)
           {
               //force on i:
               vi = local_index[i];
               ri = _system.vertices[vi].r;
               sigmai = local_sigma[i];
               for (int j = 0; j < 3; ++j)
               { 
                   a = local_area[j];
                   sigmaj = local_sigma[j];
                   Hop = meanH_operator[local_index[j]];
                   H = meanH[local_index[j]];
                   int type = _system.vertices[local_index[j]].type;
                   H_0 = H0[type];
                   k_b = kappaH[type];
                   k_g = kappaG[type];
                   vj = local_index[j];
                   cot_j = local_cot[j];
                   if (i==j)
                   {  
                       vk = local_index[(i+1)%3];
                       vl = local_index[(i+2)%3];
                       rk = _system.vertices[vk].r;
                       rl = _system.vertices[vl].r;
                       vsub(ril, ri, rl);
                       vsub(rik, ri, rk);
                       vsub(rkl, rk, rl);
                       cot_k = local_cot[(i+1)%3];
                       cot_l = local_cot[(i+2)%3];
                       Xvec3(dA,
                             (1. - a/At), sigmai,
                             - 0.125 * (cot_k+cot_l), rik,
                             - 0.125 * (cot_k+cot_l), ril);
                       Xvec1(force1,
                             0.5 * (H - 2.*H_0) * (H + 2.*H_0), dA);
                       Xvec3(force2,
                             - 1.0  * vdot(Hop,sigmai), sigmai,
                             + 0.25 * vdot(rkl,rkl), Hop,
                             - 0.25 * vdot(rkl,Hop), rkl);
                       if (vdot(force2,force2)>0.)
                           aXvec(-(H - 2.*H_0)/(H * At), force2);
                       // forceg.x=forceg.y=forceg.z=0.;
                       Xvec2(forceg,
                             -1.0*cot_k/vdot(rik,rik), rik,
                             -1.0*cot_l/vdot(ril,ril), ril);
                   }
                   else
                   {
                       vj = local_index[j];
                       vk = local_index[3-i-j];
                       rj = _system.vertices[vj].r;
                       rk = _system.vertices[vk].r;
                       vsub(rij, ri, rj);
                       vsub(rik, ri, rk);
                       vsub(rjk, rj, rk);
                       cot_i = local_cot[i];
                       cot_k = local_cot[3-i-j];
                       Xvec3(dA,
                             (0.5 - a/At), sigmai,
                             + 0.125 * (2*cot_k), rik,
                             + 0.125 * (cot_i-cot_k), rjk);
                       Xvec1(force1,
                             0.5 * (H - 2.*H_0) * (H + 2.*H_0), dA);
                       Xvec4(force2,
                             + 1.0 * vdot(Hop, sigmaj), sigmai,
                             - 0.5 * vdot(Hop, rjk), rik,
                             + 0.25 * vdot(Hop, rik), rjk,
                             + 0.25 * vdot(rjk, rik), Hop);
   
                       if (vdot(force2,force2)>0.)
                           aXvec((H - 2.*H_0)/(H * At), force2);
                       // forceg.x=forceg.y=forceg.z=0.;
                       Xvec2(forceg,
                             0.5/At, rjk,
                             cot_j/vdot(rij,rij), rij);
                   }
                   _system.vertices[vi].forceC.x += k_b * (force1.x + force2.x) + k_g * forceg.x;
                   _system.vertices[vi].forceC.y += k_b * (force1.y + force2.y) + k_g * forceg.y;
                   _system.vertices[vi].forceC.z += k_b * (force1.z + force2.z) + k_g * forceg.z;
               }
           }
       }
   }    
   
   
   double compute_vertex_energy_fn(int query_vertex_index,
       const HE_VertexProp *vertices,
       const HE_HalfEdgeProp *halfedges,
       const double *_kappaH,
       const double *_kappaG,
       const double *_H0)
   {
       real3 sigma, nv, nf;
       nv.x = nv.y = nv.z = 0.0;
       sigma.x = sigma.y = sigma.z = 0.0;
       double gaussian_curv = 2.0*defPI;
       double vertex_area = 0.0;
       int v0=query_vertex_index,v1,v2;
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
               nf = pymemb::compute_normal_triangle(vertices[v0].r, vertices[v1].r, vertices[v2].r);
               nv.x += nf.x;
               nv.y += nf.y;
               nv.z += nf.z;
   
               real3 r0 = vertices[v0].r;
               real3 r1 = vertices[v1].r;
               real3 r2 = vertices[v2].r;
               real3 r01, r02, r10, r12, r20, r21;
               vsub(r01, r0, r1);
               vsub(r02, r0, r2);
               vsub(r10, r1, r0);
               vsub(r12, r1, r2);
               vsub(r20, r2, r0);
               vsub(r21, r2, r1);
               double r01_dot_r02 = vdot(r01, r02);
               double r10_dot_r12 = vdot(r10, r12);
               double r20_dot_r21 = vdot(r20, r21);
               real3 r01_cross_r02, r10_cross_r12, r20_cross_r21;
               vcross(r01_cross_r02, r01, r02);
               vcross(r10_cross_r12, r10, r12);
               vcross(r20_cross_r21, r21, r20);
               double r01_cross_r02n = sqrt(vdot(r01_cross_r02, r01_cross_r02));
               double r10_cross_r12n = sqrt(vdot(r10_cross_r12, r10_cross_r12));
               double r20_cross_r21n = sqrt(vdot(r20_cross_r21, r20_cross_r21));
               double cot_alpha = r10_dot_r12 / r10_cross_r12n;
               double cot_beta = r20_dot_r21 / r20_cross_r21n;
               double cot_weight = 0.5 * (cot_alpha + cot_beta);
               sigma.x += 0.5 * cot_alpha * r02.x + 0.5 * cot_beta * r01.x;
               sigma.y += 0.5 * cot_alpha * r02.y + 0.5 * cot_beta * r01.y;
               sigma.z += 0.5 * cot_alpha * r02.z + 0.5 * cot_beta * r01.z;
   
               // vertex_area += 0.125 * vdot(r02, r02) * cot_alpha + 0.125 * vdot(r01, r01) * cot_beta;
               if (r01_dot_r02<0 || r10_dot_r12<0 || r20_dot_r21<0)
               {
                   if (r01_dot_r02<0)
                       vertex_area += 0.5 * r01_cross_r02n;
                   else
                       vertex_area += 0.25 * r01_cross_r02n;
               }
               else
                   vertex_area += 0.125 * vdot(r02, r02) * cot_alpha + 0.125 * vdot(r01, r01) * cot_beta;
   
               gaussian_curv-=acos(r01_dot_r02/sqrt(vdot(r01,r01)*vdot(r02,r02)));
           }
           int he_pair = halfedges[he].pair;
           he = halfedges[he_pair].next;
       } while ((he != first));
   
       int type = vertices[query_vertex_index].type;
       double sign = (vdot(nv, sigma) > 0.) ? 1. : -1.;
       double H = sign * sqrt(vdot(sigma, sigma)) / vertex_area; 
       double delH = H - 2.0 * _H0[type];
       return (0.5 * _kappaH[type] * delH * delH * vertex_area + _kappaG[type]*gaussian_curv);
   }
   
   void ComputeVertexBendingGKEnergy::compute_energy(void)
   {
       for (int vertex_index = 0; vertex_index < _system.Numvertices; vertex_index++)
       {
           double energy=compute_vertex_energy_fn(vertex_index,
                                               &_system.vertices[0],
                                               &_system.halfedges[0],
                                               &kappaH[0],
                                               &kappaG[0],
                                               &H0[0]);
           _system.vertices[vertex_index].energy += energy;
       }
   }
   
   double ComputeVertexBendingGKEnergy::compute_edge_energy(int query_edge_index)
   {
       pymemb::vector<int> v_index_vec(4);
       v_index_vec[0] = _system.edges[query_edge_index].v0;
       v_index_vec[1] = _system.edges[query_edge_index].v1;
       v_index_vec[2] = _system.edges[query_edge_index].v2;
       v_index_vec[3] = _system.edges[query_edge_index].v3;
       double edge_energy = 0.0;
       for (auto v_index : v_index_vec)
       {
           edge_energy+=compute_vertex_energy_fn(v_index,
                                               &_system.vertices[0],
                                               &_system.halfedges[0],
                                               &kappaH[0],
                                               &kappaG[0],
                                               &H0[0]);
       }
   
       return(edge_energy);
   }
   
   //energy of query vertex and its neighbor vertices
   double ComputeVertexBendingGKEnergy::compute_vertex_energy(int query_vertex_index)
   {
       double energy = compute_vertex_energy_fn(query_vertex_index,
                                               &_system.vertices[0],
                                               &_system.halfedges[0],
                                               &kappaH[0],
                                               &kappaG[0],
                                               &H0[0]);
       int he = _system.vertices[query_vertex_index]._hedge;
       int first = he;
       do
       {
           energy += compute_vertex_energy_fn(_system.halfedges[he].vert_to,
                                               &_system.vertices[0],
                                               &_system.halfedges[0],
                                               &kappaH[0],
                                               &kappaG[0],
                                               &H0[0]);
           int he_pair = _system.halfedges[he].pair;
           he = _system.halfedges[he_pair].next;        
       } while ((he != first));
       return energy; 
   }
