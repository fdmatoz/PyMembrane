
.. _program_listing_file_pymembrane_cppmodule_src_potentials_potentialLimit.cpp:

Program Listing for File potentialLimit.cpp
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_potentials_potentialLimit.cpp>` (``pymembrane/cppmodule/src/potentials/potentialLimit.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "potentialLimit.hpp"
   
   real ComputeEdgeLimitEnergy_lambda(const real3 &rij,
                                      const real &lmax,
                                      const real &lmin)
   {
       real dr = sqrt(vdot(rij, rij));
   
       real energy = 0.0;
       if (dr > lmax)
           energy = BIG_VERTEX_ENERGY_LIMIT;
       else if (dr < lmin)
           energy = BIG_VERTEX_ENERGY_LIMIT;
       return energy;
   }
   
   void ComputeVertexLimitEnergy_fn(int Numedges,
                                    HE_EdgeProp *edges,
                                    HE_VertexProp *vertices,
                                    real *_lmax,
                                    real *_lmin,
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
   
           edges[edge_index].energy += ComputeEdgeLimitEnergy_lambda(rij, _lmax[type], _lmin[type]);
       }
   }
   
   void ComputeVertexLimitEnergy::compute_energy(void)
   {
   
       ComputeVertexLimitEnergy_fn(_system.Numedges,
                                   &_system.edges[0],
                                   &_system.vertices[0],
                                   &m_lmax[0],
                                   &m_lmin[0],
                                   _system.get_box());
   }
   
   real ComputeVertexLimitEnergy::compute_edge_energy(int query_edge_index)
   {
       int type = _system.edges[query_edge_index].type;
       int v1 = _system.edges[query_edge_index].i;
       auto r1 = _system.vertices[v1].r;
       int v2 = _system.edges[query_edge_index].j;
       auto r2 = _system.vertices[v2].r;
       auto rij = pymemb::vector_subtract(r2, r1, _system.get_box());
       return ComputeEdgeLimitEnergy_lambda(rij, m_lmax[type], m_lmin[type]);
       ;
   }
   
   real ComputeVertexLimitEnergy::compute_vertex_energy(int query_vertex_index)
   {
   
       real energy = 0.0;
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
           energy += ComputeEdgeLimitEnergy_lambda(rij, m_lmax[type], m_lmin[type]);
           // MOVE TO THE NEXT edge
           int he_pair = _system.halfedges[he].pair;
           he = _system.halfedges[he_pair].next;
       } while ((he != first));
       return energy;
   }
   
   real ComputeVertexLimitForce(const real3 &rij,
                                const real &lmax,
                                const real &lmin)
   {
       real dr = sqrt(vdot(rij, rij));
       real fval = 0.0;
       if (dr > lmax)
           fval = 1.0 / dr;
       else if (dr < lmin)
           fval = -1.0 / dr;
       return fval;
   }
   
   void ComputeVertexLimitForce_fn(int Numedges,
                                   HE_VertexProp *vertices,
                                   HE_EdgeProp *edges,
                                   real *_lmax,
                                   real *_lmin,
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
   
           auto fval = ComputeVertexLimitForce(rij, _lmax[type], _lmin[type]);
           vertices[v1].forceC.x += fval * rij.x;
           vertices[v1].forceC.y += fval * rij.y;
           vertices[v1].forceC.z += fval * rij.z;
           vertices[v2].forceC.x += -1.0 * fval * rij.x;
           vertices[v2].forceC.y += -1.0 * fval * rij.y;
           vertices[v2].forceC.z += -1.0 * fval * rij.z;
       }
   }
   void ComputeVertexLimitEnergy::compute(void)
   {
   
       ComputeVertexLimitForce_fn(_system.Numedges,
                                  &_system.vertices[0],
                                  &_system.edges[0],
                                  &m_lmax[0],
                                  &m_lmin[0],
                                  _system.get_box());
   }
