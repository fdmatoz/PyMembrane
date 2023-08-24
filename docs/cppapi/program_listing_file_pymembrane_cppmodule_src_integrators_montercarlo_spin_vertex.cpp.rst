
.. _program_listing_file_pymembrane_cppmodule_src_integrators_montercarlo_spin_vertex.cpp:

Program Listing for File montercarlo_spin_vertex.cpp
====================================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_integrators_montercarlo_spin_vertex.cpp>` (``pymembrane/cppmodule/src/integrators/montercarlo_spin_vertex.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "montercarlo_spin_vertex.hpp"
   
   
   int MonteCarloIntegratorSpinVertex::integrate(void)
   {
       int accepted_moves = 0;
       for(int vertex_index = 0; vertex_index<_system.Numvertices; vertex_index++)
       {
           //attempt to move the vertex
           double energy_i = this->ComputeEnergyFromVertex(vertex_index);
           _system.vertices[vertex_index].spin *= -1;
           double energy_f = this->ComputeEnergyFromVertex(vertex_index);
           double delE = energy_f - energy_i;
           accepted_moves++;
           // std::cout << "vertexmove:"<<vertex_index<<std::endl;
           if (!(delE<0.0))
           {
               if(!(m_rng->drnd() < exp(-delE/get_temperature())))
               {
                   _system.vertices[vertex_index].spin *= -1;
                   accepted_moves--;
                   // std::cout<< "regret"<<std::endl;
               }
           }
       }
       return(accepted_moves);
   }
