
.. _program_listing_file_pymembrane_cppmodule_src_integrators_montercarlo_vertex.cpp:

Program Listing for File montercarlo_vertex.cpp
===============================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_integrators_montercarlo_vertex.cpp>` (``pymembrane/cppmodule/src/integrators/montercarlo_vertex.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "montercarlo_vertex.hpp"
   
   
   int MonteCarloIntegratorVertex::integrate(void)
   {
       int accepted_moves = 0;
       for(int vertex_index = 0; vertex_index<_system.Numvertices; vertex_index++)
       {
           //attempt to move the vertex
           real dx, dy, dz;
           auto r_old = _system.vertices[vertex_index].r;
           if (m_spherical_move)
           {
               auto len_r_old = sqrt(r_old.x * r_old.x + r_old.y * r_old.y + r_old.z * r_old.z);
               auto sigma = sqrt(m_dx * m_dx + m_dy * m_dy + m_dz * m_dz);
               dx = m_rng->gauss_rng(sigma);
               dy = m_rng->gauss_rng(sigma);
               dz = m_rng->gauss_rng(sigma);
               auto x_new = r_old.x + dx;
               auto y_new = r_old.y + dy;
               auto z_new = r_old.z + dz;
               auto len_r_new = sqrt(x_new * x_new + y_new * y_new + z_new * z_new);
               auto scale = len_r_old / len_r_new;
               dx = scale * x_new - r_old.x;
               dy = scale * y_new - r_old.y;
               dz = scale * z_new - r_old.z;
           }
           else
           {
               dx = m_dx * (m_rng->drnd() - 0.5);
               dy = m_dy * (m_rng->drnd() - 0.5);
               dz = m_dz * (m_rng->drnd() - 0.5);
           }
           //Compute the initial energy
           auto energy_i = this->ComputeEnergyFromVertex(vertex_index);
           //Move the vertex
           _system.vertices[vertex_index].r.x+= dx;
           _system.vertices[vertex_index].r.y+= dy;
           _system.vertices[vertex_index].r.z+= dz;
           //Enforce periodic boundary conditions
           //particles may have been moved slightly outside the box by the above steps, wrap them back into place
           pymemb::enforce_periodic(_system.vertices[vertex_index].r, _system.vertices[vertex_index].ip, _system.get_box());
           //Compute the final energy
           auto energy_f = this->ComputeEnergyFromVertex(vertex_index);
           //Compute the energy difference
           auto delE = energy_f - energy_i;
           //Accept or reject the move
           accepted_moves++;
           // std::cout << "vertexmove:"<<vertex_index<<std::endl;
           if (!(delE<0.0))
           {
               //reject the move
               if(!(m_rng->drnd() < exp(-delE/get_temperature())))
               {
                   _system.vertices[vertex_index].r = r_old;
                   accepted_moves--;
                   // std::cout<< "regret"<<std::endl;
               }
           }
       }
       return(accepted_moves);
   }
