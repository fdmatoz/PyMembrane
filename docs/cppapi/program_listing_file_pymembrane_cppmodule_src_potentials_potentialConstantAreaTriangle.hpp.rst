
.. _program_listing_file_pymembrane_cppmodule_src_potentials_potentialConstantAreaTriangle.hpp:

Program Listing for File potentialConstantAreaTriangle.hpp
==========================================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_potentials_potentialConstantAreaTriangle.hpp>` (``pymembrane/cppmodule/src/potentials/potentialConstantAreaTriangle.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef __potentialConstantAreaTriangle_HPP__
   #define __potentialConstantAreaTriangle_HPP__
   
   #include "computeforceclass.hpp"
   #include "../system/systemclass.hpp"
   #include "../mesh/computegeometry.hpp"
   
   class ComputeVertexConstantAreaTriangleEnergy : public ComputeForceClass
   {
   public:
       ComputeVertexConstantAreaTriangleEnergy(SystemClass &system) : ComputeForceClass(system)
       {
           m_name = "Constant Area"; 
           m_type = "face";          
           this->set_default_properties();
       }
   
       ~ComputeVertexConstantAreaTriangleEnergy() {}
   
       void set_default_properties(void) override
       {
           pymemb::vector<real> _sigma(NUM_TYPES_ALLOWED, 0.0);
           sigma = _sigma;
           pymemb::vector<real> _target_area(NUM_TYPES_ALLOWED, 1.0);
           target_area = _target_area;
       }
   
       using ComputeForceClass::set_property;
       void set_property(std::map<std::string, std::map<std::string, std::string>> &region_map) override
       {
           for (const auto &item : region_map)
           {
               if (item.first.compare("sigma") == 0)
               {
                   pymemb::vector<real> _sigma = util::from_dict_to_vector_types(pymemb::copy(sigma), item.second);
                   sigma = _sigma;
               }
               else if (item.first.compare("target_area") == 0)
               {
                   pymemb::vector<real> _target_area = util::from_dict_to_vector_types(pymemb::copy(target_area), item.second);
                   target_area = _target_area;
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
           value["sigma"] = util::to_string_vec(sigma);
           value["target_area"] = util::to_string_vec(target_area);
   
           return value;
       }
   
       void compute_energy(void) override;
       void compute(void) override;
       real compute_edge_energy(int) override;
       real compute_vertex_energy(int) override;
       void compute_stress(void) override;
       void compute_atomic_stress(void) override;
   
   private:
       pymemb::vector<real> sigma;       
       pymemb::vector<real> target_area; 
   };
   
   #endif
