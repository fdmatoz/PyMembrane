
.. _program_listing_file_pymembrane_cppmodule_src_potentials_potentialHarmonicSloutskin.cpp:

Program Listing for File potentialHarmonicSloutskin.cpp
=======================================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_potentials_potentialHarmonicSloutskin.cpp>` (``pymembrane/cppmodule/src/potentials/potentialHarmonicSloutskin.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "potentialHarmonicSloutskin.hpp"
   
   inline real ComputeEdgeHarmonicSpinEnergy(const real3 &rij,
                                             const real &k,
                                             const real &l0)
   {
       auto dr = sqrt(vdot(rij, rij));
       return (0.5 * k * (dr - l0) * (dr - l0));
   }
   
   inline real ComputeEdgeHarmonicSpinEnergy_ferro(const real &s1,
                                                   const real &s2,
                                                   const real &J)
   {
       return (-J * s1 * s2);
   }
   
   void ComputeVertexHarmonicSpinEnergy_fn(const int Numedges,
                                           HE_EdgeProp *edges,
                                           const HE_VertexProp *__restrict__ vertices,
                                           const real *__restrict__ _k,
                                           const real *__restrict__ _l0,
                                           const real *__restrict__ _J,
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
   
           auto s1 = vertices[v1].spin;
           auto s2 = vertices[v2].spin;
   
           // Harmonic Energy
           double energy = ComputeEdgeHarmonicSpinEnergy(rij, _k[type], _l0[type]);
           // Ferromagnetic Energy
           energy += ComputeEdgeHarmonicSpinEnergy_ferro(s1, s2, _J[type]);
   
           edges[edge_index].energy += energy;
       }
   }
   
   double ComputeVertexHarmonicSpinEnergy::compute_edge_energy(int query_edge_index)
   {
       int type = _system.edges[query_edge_index].type;
       int v1 = _system.edges[query_edge_index].i;
       auto r1 = _system.vertices[v1].r;
       int v2 = _system.edges[query_edge_index].j;
       auto r2 = _system.vertices[v2].r;
       auto rij = pymemb::vector_subtract(r2, r1, _system.get_box());
   
       auto s1 = _system.vertices[v1].spin;
       auto s2 = _system.vertices[v2].spin;
   
       // Harmonic Energy
       double edge_energy = ComputeEdgeHarmonicSpinEnergy(rij, k[type], l0[type]);
   
       // Ferromagnetic Energy
       edge_energy += ComputeEdgeHarmonicSpinEnergy_ferro(s1, s2, J[type]);
       return edge_energy;
   }
   
   double ComputeVertexHarmonicSpinEnergy::compute_vertex_energy(int query_vertex_index)
   {
   
       double harmonic_energy = 0.0;
       double ferro_energy = 0.0;
       int he = _system.vertices[query_vertex_index]._hedge;
       int first = he;
       do
       {
           
           auto edge_index = _system.halfedges[he].edge;
           auto type = _system.edges[edge_index].type;
           auto v1 = _system.edges[edge_index].i;
           auto v2 = _system.edges[edge_index].j;
   
           auto r1 = _system.vertices[v1].r;
           auto r2 = _system.vertices[v2].r;
           auto rij = pymemb::vector_subtract(r2, r1, _system.get_box());
           harmonic_energy += 0.5 * ComputeEdgeHarmonicSpinEnergy(rij, k[type], l0[type]);
   
           auto s1 = _system.vertices[v1].spin;
           auto s2 = _system.vertices[v2].spin;
           ferro_energy += 0.5 * ComputeEdgeHarmonicSpinEnergy_ferro(s1, s2, J[type]);
           
           int he_pair = _system.halfedges[he].pair;
           he = _system.halfedges[he_pair].next;
       } while ((he != first));
   
       if (_system.vertices[query_vertex_index].spin > 0.0)
       {
           auto vertex_type = _system.vertices[query_vertex_index].type;
           harmonic_energy = Ea[vertex_type];
       }
       auto energy = harmonic_energy + ferro_energy;
       return energy;
   }
   
   void ComputeVertexHarmonicSpinEnergy::compute_energy(void)
   {
   
       ComputeVertexHarmonicSpinEnergy_fn(_system.Numedges,
                                          &_system.edges[0],
                                          &_system.vertices[0],
                                          &k[0],
                                          &l0[0],
                                          &J[0],
                                          _system.get_box());
   }
   
   double ComputeVertexHarmonicSpinForce(const real3 rij,
                                         const double k,
                                         const double l0)
   {
       double dr = sqrt(vdot(rij, rij));
       double fval = k * (dr - l0) / dr;
       return fval;
   }
   
   void ComputeVertexHarmonicSpinForce_fn(const int Numedges,
                                          HE_VertexProp *vertices,
                                          const HE_EdgeProp *__restrict__ edges,
                                          const double *__restrict__ _k,
                                          const double *__restrict__ _l0,
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
   
           double fval = ComputeVertexHarmonicSpinForce(rij, _k[type], _l0[type]);
   
           vertices[v1].forceC.x += fval * rij.x;
           vertices[v1].forceC.y += fval * rij.y;
           vertices[v1].forceC.z += fval * rij.z;
   
           vertices[v2].forceC.x += -1.0 * fval * rij.x;
           vertices[v2].forceC.y += -1.0 * fval * rij.y;
           vertices[v2].forceC.z += -1.0 * fval * rij.z;
       }
   }
   
   void ComputeVertexHarmonicSpinEnergy::compute(void)
   {
   
       ComputeVertexHarmonicSpinForce_fn(_system.Numedges,
                                         &_system.vertices[0],
                                         &_system.edges[0],
                                         &k[0],
                                         &l0[0],
                                         _system.get_box());
   }
