
.. _program_listing_file_pymembrane_cppmodule_src_potentials_potentialIsing.hpp:

Program Listing for File potentialIsing.hpp
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_potentials_potentialIsing.hpp>` (``pymembrane/cppmodule/src/potentials/potentialIsing.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #ifndef __POTENTIALISING_HPP__
   #define __POTENTIALISING_HPP__
   
   #include <math.h>
   //host
   #include "computeforceclass.hpp"
   
   //
   
   #include "../system/systemclass.hpp"
   #include "../mesh/computegeometry.hpp"
   
   class ComputeVertexFerromagneticEnergy : public ComputeForceClass
   {
   public:
     ComputeVertexFerromagneticEnergy(SystemClass &system) : ComputeForceClass(system)
     {
       m_name = "Limit"; 
       m_type = "edge";  
       this->set_default_properties();
     }
     ~ComputeVertexFerromagneticEnergy() {}
   
     void set_default_properties(void) override
     {
       pymemb::vector<real> _J(NUM_TYPES_ALLOWED, 1.0);
       J = _J;
       pymemb::vector<real> _h(NUM_TYPES_ALLOWED, 0.0);
       h = _h;
     }
   
     using ComputeForceClass::set_property;
     void set_property(std::map<std::string, std::map<std::string, std::string>> &region_map) override
     {
       for (const auto &item : region_map)
       {
         if (item.first.compare("J") == 0)
         {
           pymemb::vector<real> _J = util::from_dict_to_vector_types(pymemb::copy(J), item.second);
           J = _J;
         }
         else if (item.first.compare("h") == 0)
         {
           pymemb::vector<real> _h = util::from_dict_to_vector_types(pymemb::copy(h), item.second);
           h = _h;
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
       value["J"] = util::to_string_vec(J);
       value["h"] = util::to_string_vec(h);
       return value;
     }
   
     void compute_energy(void) override;
     void compute(void) override;
     real compute_edge_energy(int) override;
     real compute_vertex_energy(int) override;
   
   private:
     pymemb::vector<real> J; 
     pymemb::vector<real> h; 
   
   };
   
   #endif
   
