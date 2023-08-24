
.. _program_listing_file_pymembrane_cppmodule_src_integrators_montercarlo_swap_edge.cpp:

Program Listing for File montercarlo_swap_edge.cpp
==================================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_integrators_montercarlo_swap_edge.cpp>` (``pymembrane/cppmodule/src/integrators/montercarlo_swap_edge.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "montercarlo_swap_edge.hpp"
   
   int MonteCarloIntegratorSwapEdge::integrate(void)
   {
       int accepted_moves = 0;
       int e1, e2;
       int type1;
       double energy_i, energy_f;
       for (int edge_index = 0; edge_index < _system.Numedges; edge_index++)
       {
           e1 = _system.Numedges * m_rng->drnd();
           e2 = _system.Numedges * m_rng->drnd();
           // if edges have same type do nothing
           if ((_system.edges[e1].type != _system.edges[e2].type) && (freezed_edge[_system.edges[e1].type] == false && freezed_edge[_system.edges[e2].type] == false))
           {
               energy_i = this->ComputeEnergyFromEdge(e1);
               energy_i += this->ComputeEnergyFromEdge(e2);
               type1 = _system.edges[e1].type;
               _system.edges[e1].type = _system.edges[e2].type;
               _system.edges[e2].type = type1;
               energy_f = this->ComputeEnergyFromEdge(e1);
               energy_f += this->ComputeEnergyFromEdge(e2);
               double delE = energy_f - energy_i;
               accepted_moves++;
               //stochastic tunnelling
               if (stochastic_tunnelling)
                   delE = exp(-stochastic_gamma * delE) - exp(-stochastic_gamma * (energy_f - get_minimum_energy()));
               if (!(delE < 0.0))
               {
                   if (!(m_rng->drnd() < exp(-delE / get_temperature())))
                   {
                       _system.edges[e2].type = _system.edges[e1].type;
                       _system.edges[e1].type = type1;
                       accepted_moves--;
                   }
               }
           }
       }
       return (accepted_moves);
   }
