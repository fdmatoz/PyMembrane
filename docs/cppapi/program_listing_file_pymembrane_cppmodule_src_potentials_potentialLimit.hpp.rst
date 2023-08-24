
.. _program_listing_file_pymembrane_cppmodule_src_potentials_potentialLimit.hpp:

Program Listing for File potentialLimit.hpp
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_potentials_potentialLimit.hpp>` (``pymembrane/cppmodule/src/potentials/potentialLimit.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef __POTENTIALLIMIT_HPP__
   #define __POTENTIALLIMIT_HPP__
   
   #include <math.h>
   #include "computeforceclass.hpp"
   #include "../system/systemclass.hpp"
   #include "../mesh/computegeometry.hpp"
   
   class ComputeVertexLimitEnergy : public ComputeForceClass
   {
   public:
     ComputeVertexLimitEnergy(SystemClass &system) : ComputeForceClass(system)
     {
       m_name = "Limit"; 
       m_type = "edge";  
       this->set_default_properties();
     }
     ~ComputeVertexLimitEnergy() {}
   
     void set_default_properties(void) override
     {
       pymemb::vector<real> _lmax(NUM_TYPES_ALLOWED, 100.0);
       m_lmax = _lmax;
       pymemb::vector<real> _lmin(NUM_TYPES_ALLOWED, 0.0);
       m_lmin = _lmin;
     }
   
     using ComputeForceClass::set_property;
     void set_property(std::map<std::string, std::map<std::string, std::string>> &region_map) override
     {
       for (const auto &item : region_map)
       {
         if (item.first.compare("lmax") == 0)
         {
           pymemb::vector<real> _lmax = util::from_dict_to_vector_types(pymemb::copy(m_lmax), item.second);
           m_lmax = _lmax;
         }
         else if (item.first.compare("lmin") == 0)
         {
           pymemb::vector<real> _lmin = util::from_dict_to_vector_types(pymemb::copy(m_lmin), item.second);
           m_lmin = _lmin;
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
       value["lmax"] = util::to_string_vec(m_lmax);
       value["lmin"] = util::to_string_vec(m_lmin);
       return value;
     }
   
     void compute_energy(void) override;
     void compute(void) override;
     real compute_edge_energy(int) override;
     real compute_vertex_energy(int) override;
   
   private:
     pymemb::vector<real> m_lmax; 
     pymemb::vector<real> m_lmin; 
   };
   
   #endif
   
