
.. _program_listing_file_pymembrane_cppmodule_src_integrators_integrator_brownian_vertex.cpp:

Program Listing for File integrator_brownian_vertex.cpp
=======================================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_integrators_integrator_brownian_vertex.cpp>` (``pymembrane/cppmodule/src/integrators/integrator_brownian_vertex.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "integrator_brownian_vertex.hpp"
   
   void IntegratorBrownianMeshVertex::poststep(void)
   {
     for (int id = 0; id < _system.Numvertices; id++)
     {
       real3 force_rnd;
       force_rnd.x = force_rnd.y = force_rnd.z = 0.0;
       if (this->get_temperature() > 0.0)
       {
         force_rnd.x = m_B * m_rng->gauss_rng();
         force_rnd.y = m_B * m_rng->gauss_rng();
         force_rnd.z = m_B * m_rng->gauss_rng();
       }
       // Update particle position
       _system.vertices[id].r.x += m_mu * this->get_time_step() * _system.vertices[id].forceC.x + m_sqrt_dt * force_rnd.x;
       _system.vertices[id].r.y += m_mu * this->get_time_step() * _system.vertices[id].forceC.y + m_sqrt_dt * force_rnd.y;
       _system.vertices[id].r.z += m_mu * this->get_time_step() * _system.vertices[id].forceC.z + m_sqrt_dt * force_rnd.z;
   
       _system.vertices[id].v.x = m_sqrt_dt * force_rnd.x;
       _system.vertices[id].v.y = m_sqrt_dt * force_rnd.y;
       _system.vertices[id].v.z = m_sqrt_dt * force_rnd.z;
   
       // particles may have been moved slightly outside the box by the above steps, wrap them back into place
       pymemb::enforce_periodic(_system.vertices[id].r, _system.vertices[id].ip, _system.get_box());
     }
   }
