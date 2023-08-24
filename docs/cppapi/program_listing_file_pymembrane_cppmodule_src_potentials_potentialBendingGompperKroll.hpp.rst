
.. _program_listing_file_pymembrane_cppmodule_src_potentials_potentialBendingGompperKroll.hpp:

Program Listing for File potentialBendingGompperKroll.hpp
=========================================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_potentials_potentialBendingGompperKroll.hpp>` (``pymembrane/cppmodule/src/potentials/potentialBendingGompperKroll.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef __POTENTIALBENDINGGOMPPERKROLL_HPP__
   #define __POTENTIALBENDINGGOMPPERKROLL_HPP__
   
   //host
   #include "computeforceclass.hpp"
   
   //
   #include "../system/systemclass.hpp"
   #include "../mesh/computegeometry.hpp"
   class ComputeVertexBendingGKEnergy : public ComputeForceClass
   {
   public:
     ComputeVertexBendingGKEnergy(SystemClass &system) : ComputeForceClass(system)
     {
       m_name = "BendingGK"; 
       m_type = "vertex";    
       this->set_default_properties();
     }
   
     ~ComputeVertexBendingGKEnergy() {}
   
     void set_default_properties(void) override
     {
       pymemb::vector<real> _kappaH(NUM_TYPES_ALLOWED, 1.0);
       kappaH = _kappaH;
       pymemb::vector<real> _H0(NUM_TYPES_ALLOWED, 0.0);
       H0 = _H0;
       pymemb::vector<real> _kappaG(NUM_TYPES_ALLOWED, 0.0);
       kappaG = _kappaG;
     }
   
     using ComputeForceClass::set_property;
     void set_property(std::map<std::string, std::map<std::string, std::string>> &region_map) override
     {
       for (const auto &item : region_map)
       {
         if (item.first.compare("kappaH") == 0)
         {
           pymemb::vector<real> _kappaH= util::from_dict_to_vector_types(pymemb::copy(kappaH), item.second);
           kappaH = _kappaH;
         }
         else if (item.first.compare("H0") == 0)
         {
           pymemb::vector<real> _H0 = util::from_dict_to_vector_types(pymemb::copy(H0), item.second);
           H0 = _H0;
         }
         else if (item.first.compare("kappaG") == 0)
         {
           pymemb::vector<real> _kappaG = util::from_dict_to_vector_types(pymemb::copy(kappaG), item.second);
           kappaG = _kappaG;
         }
         else
           this->print_warning_property_name(item.first);
       }
     }
   
     std::map<std::string, std::string> get_info(void)
     {
       std::map<std::string, std::string> value;
       value["name"] = m_name;
       value["type"] = m_type;
       value["kappaH"] = util::to_string_vec(kappaH);
       value["H0"] = util::to_string_vec(H0);
       value["kappaG"] = util::to_string_vec(kappaG);
       return value;
     }
   
     void compute_energy(void);
     void compute(void);
     double compute_edge_energy(int);
     double compute_vertex_energy(int);
   
   private:
     pymemb::vector<double> kappaH; 
     pymemb::vector<double> H0;    
     pymemb::vector<double> kappaG;  
   };
   
   #endif
   
