
.. _program_listing_file_pymembrane_cppmodule_src_potentials_potentialBendingHelfrich.hpp:

Program Listing for File potentialBendingHelfrich.hpp
=====================================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_potentials_potentialBendingHelfrich.hpp>` (``pymembrane/cppmodule/src/potentials/potentialBendingHelfrich.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef __potentialBendingHelfrich_HPP__
   #define __potentialBendingHelfrich_HPP__
   
   //host
   #include "computeforceclass.hpp"
   
   //
   #include "../system/systemclass.hpp"
   #include "../mesh/computegeometry.hpp"
   class ComputeVertexBendingHelfrichEnergy : public ComputeForceClass
   {
   public:
     ComputeVertexBendingHelfrichEnergy(SystemClass &system) : ComputeForceClass(system)
     {
       m_name = "BendingGK"; 
       m_type = "vertex";    
       this->set_default_properties();
     }
   
     ~ComputeVertexBendingHelfrichEnergy() {}
   
     void set_default_properties(void) override
     {
       pymemb::vector<real> _kappaH(NUM_TYPES_ALLOWED, 1.0);
       m_kappaH = _kappaH;
       pymemb::vector<real> _H0(NUM_TYPES_ALLOWED, 0.0);
       m_H0 = _H0;
       pymemb::vector<real> _kappaG(NUM_TYPES_ALLOWED, 0.0);
       m_kappaG = _kappaG;
     }
   
     using ComputeForceClass::set_property;
     void set_property(std::map<std::string, std::map<std::string, std::string>> &region_map) override
     {
       for (const auto &item : region_map)
       {
         if (item.first.compare("kappaH") == 0)
         {
           pymemb::vector<real> _kappaH= util::from_dict_to_vector_types(pymemb::copy(m_kappaH), item.second);
           m_kappaH = _kappaH;
         }
         else if (item.first.compare("H0") == 0)
         {
           pymemb::vector<real> _H0 = util::from_dict_to_vector_types(pymemb::copy(m_H0), item.second);
           m_H0 = _H0;
         }
         else if (item.first.compare("kappaG") == 0)
         {
           pymemb::vector<real> _kappaG = util::from_dict_to_vector_types(pymemb::copy(m_kappaG), item.second);
           m_kappaG = _kappaG;
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
       value["kappaH"] = util::to_string_vec(m_kappaH);
       value["H0"] = util::to_string_vec(m_H0);
       value["kappaG"] = util::to_string_vec(m_kappaG);
       return value;
     }
   
     void compute_energy(void) override;
     void compute(void) override;
     real compute_edge_energy(int) override;
     real compute_vertex_energy(int) override;
     void compute_stress(void) override;
     void compute_atomic_stress(void) override;
   private:
     pymemb::vector<double> m_kappaH; 
     pymemb::vector<double> m_H0;    
     pymemb::vector<double> m_kappaG;  
   };
   
   #endif
   
