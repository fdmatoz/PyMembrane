
.. _program_listing_file_pymembrane_cppmodule_src_minimizer_fire.cpp:

Program Listing for File fire.cpp
=================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_minimizer_fire.cpp>` (``pymembrane/cppmodule/src/minimizer/fire.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   /*
   * @Author: siyu
   * @Date:   2020-07-26 11:06:58
   * @Last Modified by:   siyu
   * @Last Modified time: 2020-08-11 22:55:44
   */
   #include "fire.hpp"
   
   void MinimizerMeshFIRE::reset(void)
   {
     m_converged = false;
     m_n_since_negative = m_nmin + 1;
     m_n_since_start = 0;
     m_alpha = m_alpha_start;
     m_energy_total = 0.0;
     m_old_energy = 0.0;
     m_dE = 0.0;
     m_fnorm = 0.0;
     m_dt = 0.005;
     this->update_time_step_parameters();
     for (int id = 0; id < _system.Numvertices; id++)
     {
       _system.vertices[id].v.x = 0.0;
       _system.vertices[id].v.y = 0.0;
       _system.vertices[id].v.z = 0.0;
     }
   }
   
   void MinimizerMeshFIRE::poststep1(void)
   {
     // NVT, velocity verlet
     for (int id = 0; id < _system.Numvertices; id++)
     {
       Xvec3(_system.vertices[id].r,
             1., _system.vertices[id].r,
             m_dt, _system.vertices[id].v,
             0.5 * m_dt * m_dt, _system.vertices[id].forceC);
       Xvec2(_system.vertices[id].v,
             1., _system.vertices[id].v,
             0.5 * m_dt, _system.vertices[id].forceC);
     }
   }
   void MinimizerMeshFIRE::poststep2(void)
   {
     for (int id = 0; id < _system.Numvertices; id++)
     {
       Xvec2(_system.vertices[id].v,
             1., _system.vertices[id].v,
             0.5 * m_dt, _system.vertices[id].forceC);
     }
   }
   void MinimizerMeshFIRE::poststep3(void)
   {
     //FIRE
     double P = 0.;
     double vnorm = 0.;
     double fnorm = 0.;
     double tnorm = 0.;
     double energy = 0.;
   
     for (int id = 0; id < _system.Numvertices; id++)
     {
       P += vdot(_system.vertices[id].forceC, _system.vertices[id].v);
       fnorm += vdot(_system.vertices[id].forceC, _system.vertices[id].forceC);
       vnorm += vdot(_system.vertices[id].v, _system.vertices[id].v);
       energy += _system.vertices[id].energy;
     }
   
     for (int id = 0; id < _system.Numedges; id++)
       energy += _system.edges[id].energy;
     for (int id = 0; id < _system.Numfaces; id++)
       energy += _system.faces[id].energy;
   
     fnorm = sqrt(fnorm);
     vnorm = sqrt(vnorm);
     m_fnorm = fnorm;
     m_energy_total = energy;
     m_dE = m_energy_total - m_old_energy;
     // energy = energy/(_system.Numvertices);
   
     // if ((fnorm/sqrt(3.)/(_system.Numvertices) < m_ftol && fabs(energy-m_old_energy) < m_etol) && m_n_since_start >= m_run_minsteps)
     if (fnorm < m_ftol && fabs(energy - m_old_energy) < m_etol && m_n_since_start >= m_run_minsteps)
     {
       pybind11::print(m_name, " converged, ftol:" , m_ftol , " m_etol:" , m_etol);
       m_converged = true;
       return;
     }
   
     // double factor_t;
     // if (fabs(fnorm) > EPSILON)
     //     factor_t = m_alpha*vnorm/fnorm;
     // else
     //     factor_t = 1.0;
   
     for (int id = 0; id < _system.Numvertices; id++)
     {
       Xvec2(_system.vertices[id].v,
             1.0 - m_alpha, _system.vertices[id].v,
             m_alpha * vnorm / fnorm, _system.vertices[id].forceC);
     }
   
     if (P > 0.0)
     {
       m_n_since_negative++;
       if (m_n_since_negative > m_nmin)
       {
         m_dt = std::min(m_dt, m_dT_max);
         m_alpha *= m_falpha;
       }
     }
     else if (P <= 0.0)
     {
       m_dt = m_dt * m_fdec;
       m_alpha = m_alpha_start;
       m_n_since_negative = 0;
   
       //pybind11::print( this->name, " zero velocities");
       //pybind11::print("P:" , P , " fnorm:" , fnorm , " vnorm:" , vnorm , " delta energy:" , fabs(energy - m_old_energy) , " energy/N:" , energy / (_system.Numvertices) , "alpha:" , m_alpha);
   
       for (int id = 0; id < _system.Numvertices; id++)
       {
         _system.vertices[id].v.x = 0.0;
         _system.vertices[id].v.y = 0.0;
         _system.vertices[id].v.z = 0.0;
       }
     }
   
     m_n_since_start++;
     m_old_energy = energy;
   }
   
   void MinimizerMeshFIRE::minimize(void)
   {
     for (auto step = 0; step < m_max_iter; step++)
     {
       _evolver.reset_mesh_forces();
       _evolver.compute_mesh_forces();
       poststep1();
       _system.enforce_periodic_boundary_conditions();
   
       _evolver.reset_mesh_forces();
       _evolver.compute_mesh_forces();
       poststep2();
   
       //enforce the constraints before calculate the energy
       _evolver.enforce_mesh_constraints();
   
       _evolver.reset_mesh_energy();
       _evolver.compute_mesh_energy();
       poststep3();
   
       if (m_converged == true)
         break;
     }
     //pybind11::print(" Minimizer ", this->get_info());
   }
