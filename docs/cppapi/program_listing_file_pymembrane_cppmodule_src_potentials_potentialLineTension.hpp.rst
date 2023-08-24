
.. _program_listing_file_pymembrane_cppmodule_src_potentials_potentialLineTension.hpp:

Program Listing for File potentialLineTension.hpp
=================================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_potentials_potentialLineTension.hpp>` (``pymembrane/cppmodule/src/potentials/potentialLineTension.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #ifndef __POTENTIALLINETENSION_HPP__
   #define __POTENTIALLINETENSION_HPP__
   
   //host
   #include "computeforceclass.hpp"
   
   //
   #include "../system/systemclass.hpp"
   #include "../mesh/computegeometry.hpp"
   
   class ComputeVertexLineTension : public ComputeForceClass
   {
   public:
     ComputeVertexLineTension(SystemClass &system) : ComputeForceClass(system)
     {
       m_name = "Line Tension"; 
       m_type = "vertex";       
       this->set_default_properties();
     }
   
     ~ComputeVertexLineTension() {}
   
     void set_default_properties(void) override
     {
       std::vector<double> _gamma(NUM_TYPES_ALLOWED, 0.0);
       m_gamma = _gamma;
       m_flag_avg = false;
       m_flag_scale = false;
     }
   
     using ComputeForceClass::set_property;
     void set_property(std::map<std::string, std::map<std::string, std::string>> &region_map) override
     {
       for (const auto &item : region_map)
       {
         if (item.first.compare("gamma") == 0)
         {
           pymemb::vector<real> _gamma = util::from_dict_to_vector_types(pymemb::copy(m_gamma), item.second);
           m_gamma = _gamma;
         }
         else
           this->print_warning_property_name(item.first);
       }
     }
   
     void set_property(std::map<std::string, std::string> &region_map) override
     {
       for (const auto &item : region_map)
       {
         if (item.first.compare("avg") == 0)
           m_flag_avg = util::from_string_bool(item.second);
         else if (item.first.compare("scale") == 0)
           m_flag_scale = util::from_string_bool(item.second);
         else
           this->print_warning_property_name(m_name);
       }
     }
   
     std::map<std::string, std::string> get_info(void) override
     {
       std::map<std::string, std::string> value;
       value["name"] = m_name;
       value["type"] = m_type;
       value["gamma"] = util::to_string_vec(m_gamma);
       value["avg"] = util::to_string(m_flag_avg);
       value["scale"] = util::to_string(m_flag_scale);
       return value;
     }
   
     void compute_energy(void) override;
     real compute_edge_energy(int) override;
     real compute_vertex_energy(int) override;
   
   private:
     pymemb::vector<real> m_gamma; 
     bool m_flag_avg;            
     bool m_flag_scale;
   };
   
   #endif
   
