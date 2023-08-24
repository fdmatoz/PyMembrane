
.. _program_listing_file_pymembrane_cppmodule_src_potentials_potentialLineTension.cpp:

Program Listing for File potentialLineTension.cpp
=================================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_potentials_potentialLineTension.cpp>` (``pymembrane/cppmodule/src/potentials/potentialLineTension.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "potentialLineTension.hpp"
   
   void ComputeVertexLineTension::compute_energy(void)
   {
       for(int vertex_index=0;vertex_index<_system.Numvertices;vertex_index++)
       {
           _system.vertices[vertex_index].energy=this->compute_vertex_energy(vertex_index);
       }
   }
   
   real ComputeVertexLineTension::compute_edge_energy(int query_edge_index)
   {
       real edge_energy = this->compute_vertex_energy(_system.edges[query_edge_index].i);
       edge_energy+=this->compute_vertex_energy(_system.edges[query_edge_index].j);
       return edge_energy;
   }
   
   real ComputeVertexLineTension::compute_vertex_energy(int query_vertex_index)
   {
       real energy = 0.0;
       int he = _system.vertices[query_vertex_index]._hedge;
       int first = he;
       int type_0 = _system.vertices[query_vertex_index].type;
       real3 r0 = _system.vertices[query_vertex_index].r;
   
       real scale_gamma, gamma_type;
       do
       {
           int v1 = _system.halfedges[he].vert_to;
           int type_1 = _system.vertices[v1].type;
           if(type_0!=type_1)
           {    
               if(m_flag_avg==true)
                   gamma_type = 0.5*(m_gamma[type_0]+m_gamma[type_1]);
               else 
                   gamma_type = m_gamma[type_0];
               if(m_flag_scale==true)
               {
                   auto r1 = _system.vertices[query_vertex_index].r;
                   auto rij = pymemb::vector_subtract(r1, r0, _system.get_box());
                   scale_gamma = sqrt(vdot(rij, rij));
               }
               else
                   scale_gamma = 1.0;
               energy+= scale_gamma*gamma_type;
           }
           // MOVE TO THE NEXT edge
           int he_pair = _system.halfedges[he].pair;
           he = _system.halfedges[he_pair].next;
       }while((he!=first));
       return energy;
   }
