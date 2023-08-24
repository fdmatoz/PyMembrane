
.. _program_listing_file_pymembrane_cppmodule_src_potentials_potentialCauchyGreen.hpp:

Program Listing for File potentialCauchyGreen.hpp
=================================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_potentials_potentialCauchyGreen.hpp>` (``pymembrane/cppmodule/src/potentials/potentialCauchyGreen.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #ifndef __POTENTIALCAUCHYGREEN_HPP__
   #define __POTENTIALCAUCHYGREEN_HPP__
   
   
   #include "computeforceclass.hpp"
   #include "../system/systemclass.hpp"
   #include "../mesh/computegeometry.hpp"
   
   class ComputeVertexCauchyGreenEnergy : public ComputeForceClass
   {
   public:
     ComputeVertexCauchyGreenEnergy(SystemClass &system) : ComputeForceClass(system)
     {
       m_name = "Cauchy-Green"; 
       m_type = "face";        
       this->set_default_properties();
     }
   
     ~ComputeVertexCauchyGreenEnergy() {}
   
     void set_default_properties(void) override
     {
       pymemb::vector<real> _E(NUM_TYPES_ALLOWED, 1.0);
       Y = _E;
       pymemb::vector<real> _nu(NUM_TYPES_ALLOWED, (1.0 / 3.0));
       nu = _nu;
       pymemb::vector<real> _h(NUM_TYPES_ALLOWED, 1.0);
       h = _h;
     }
   
     using ComputeForceClass::set_property;
     void set_property(std::map<std::string, std::map<std::string, std::string>> &region_map) override
     {
       for (const auto &item : region_map)
       {
         if (item.first.compare("E") == 0)
         {
           pymemb::vector<real> _E = util::from_dict_to_vector_types(pymemb::copy(Y), item.second);
           Y = _E;
         }
         else if (item.first.compare("nu") == 0)
         {
           pymemb::vector<real> _nu = util::from_dict_to_vector_types(pymemb::copy(nu), item.second);
           nu = _nu;
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
       value["E"] = util::to_string_vec(Y);
       value["nu"] = util::to_string_vec(nu);
       value["h"] = util::to_string_vec(h);
       return value;
     }
   
     void compute_energy(void) override;
     void compute(void) override;
     real compute_edge_energy(int) override;
     real compute_vertex_energy(int) override;
     void compute_stress(void) override;
     void compute_atomic_stress(void) override;
   
   private:
     pymemb::vector<real> Y;  
     pymemb::vector<real> nu; 
     pymemb::vector<real> h;  
   };
   
   #endif
   
