
.. _program_listing_file_pymembrane_cppmodule_src_potentials_potentialHarmonicSloutskin.hpp:

Program Listing for File potentialHarmonicSloutskin.hpp
=======================================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_potentials_potentialHarmonicSloutskin.hpp>` (``pymembrane/cppmodule/src/potentials/potentialHarmonicSloutskin.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #ifndef __potentialHarmonicSloutskin_HPP__
   #define __potentialHarmonicSloutskin_HPP__
   
   //host
   #include "computeforceclass.hpp"
   
   //
   
   #include "../system/systemclass.hpp"
   #include "../mesh/computegeometry.hpp"
   
   class ComputeVertexHarmonicSpinEnergy : public ComputeForceClass
   {
   public:
     ComputeVertexHarmonicSpinEnergy(SystemClass &system) : ComputeForceClass(system)
     {
       m_name = "Harmonic"; 
       m_type = "edge";     
       this->set_default_properties();
     }
   
     ~ComputeVertexHarmonicSpinEnergy() {}
   
     void set_default_properties(void) override
     {
       pymemb::vector<real> _k(NUM_TYPES_ALLOWED, 0.0);
       k = _k;
       pymemb::vector<real> _l0(NUM_TYPES_ALLOWED, 1.0);
       l0 = _l0;
       pymemb::vector<real> _Ea(NUM_TYPES_ALLOWED, 1.0);
       Ea = _Ea;
     }
   
     using ComputeForceClass::set_property;
     void set_property(std::map<std::string, std::map<std::string, std::string>> &region_map) override
     {
       for (const auto &item : region_map)
       {
         if (item.first.compare("k") == 0)
         {
           pymemb::vector<real> _k = util::from_dict_to_vector_types(pymemb::copy(k), item.second);
           k = _k;
         }
         else if (item.first.compare("l0") == 0)
         {
           pymemb::vector<real> _l0 = util::from_dict_to_vector_types(pymemb::copy(l0), item.second);
           l0 = _l0;
         }
         else if (item.first.compare("J") == 0)
         {
           pymemb::vector<real> _J = util::from_dict_to_vector_types(pymemb::copy(l0), item.second);
           J = _J;
         }
         else if (item.first.compare("Ea") == 0)
         {
           pymemb::vector<real> _Ea = util::from_dict_to_vector_types(pymemb::copy(l0), item.second);
           Ea = _Ea;
         }
         else
           this->print_warning_property_name(item.first);
       }
     }
     
     std::map<std::string, std::string> get_info(void) override
     {
       std::map<std::string, std::string> value;
       value["name"] = m_name;
       value["type"] = m_type;
       value["k"] = util::to_string_vec(k);
       value["l0"] = util::to_string_vec(l0);
       value["J"] = util::to_string_vec(J);
       value["Ea"] = util::to_string_vec(Ea);
       return value;
     }
   
     void compute_energy(void) override;
     void compute(void) override;
     real compute_edge_energy(int) override;
     real compute_vertex_energy(int) override;
     void compute_stress(void) override {}
     void compute_atomic_stress(void)override {}
   
   private:
     pymemb::vector<real> k;  
     pymemb::vector<real> l0; 
     pymemb::vector<real> J; 
     pymemb::vector<real> Ea; 
   
   };
   
   #endif
   
