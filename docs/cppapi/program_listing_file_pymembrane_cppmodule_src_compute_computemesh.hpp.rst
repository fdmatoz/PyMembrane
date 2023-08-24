
.. _program_listing_file_pymembrane_cppmodule_src_compute_computemesh.hpp:

Program Listing for File computemesh.hpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_compute_computemesh.hpp>` (``pymembrane/cppmodule/src/compute/computemesh.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef __computemesh_hpp__
   #define __computemesh_hpp__
   
   #include <memory>
   #include <map>
   
   #include "../types/globaltypes.hpp"
   #include "../types/pymembvector.hpp"
   
   class SystemClass; // forward declaration
   class EvolverClass;
   
   class ComputeMesh
   {
   public:
       ComputeMesh(SystemClass &system) : _system(system) {}
   
       void compute_vertex_normals(bool vertex_normal_angle_weight = false);
   
       void compute_face_normals(void);
   
       pymemb::vector<real> compute_edge_lengths(void);
   
       real gaussiancurvature_vertex(const int &vertex_index);
   
       real meancurvature_vertex(const int &vertex_index);
       pymemb::vector<real> gaussiancurvature(void);
   
       pymemb::vector<real> meancurvature(void);
   
       std::map<std::string, pymemb::vector<real>> compute_mesh_curvature(void);
       pymemb::vector<pymemb::vector<real>> compute_face_metric(void);
   
       real compute_mesh_volume(void);
   
       pymemb::vector<real> compute_face_area(void);
   
       real compute_mesh_area(void);
   
       pymemb::vector<real> compute_vertex_area(void);
   
   
       std::map<std::string, real> compute_mesh_energy(EvolverClass &);
   
       pymemb::vector<real3> compute_vertex_forces(EvolverClass &);
   
   
       realTensor compute_stresses(EvolverClass &, const bool &);
   
       realTensor get_stresses(EvolverClass &, const bool &);
   
       std::vector<real> compute_pressure(EvolverClass &);
   
       realTensor compute_kinetic_energy_tensor(void);
       real compute_kinetic_energy(void);
   
       real compute_temperature(void);
   
       realTensor compute_stresses_virial(EvolverClass &evolver, const bool &computeForAllAtoms);
   
       std::vector<realTensor> compute_stresses_atom(EvolverClass &evolver, const bool &computeForAllAtoms);
   
   private:
       SystemClass &_system;
   };
   
   #endif
