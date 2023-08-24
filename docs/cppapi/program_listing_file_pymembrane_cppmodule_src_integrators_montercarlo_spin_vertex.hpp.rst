
.. _program_listing_file_pymembrane_cppmodule_src_integrators_montercarlo_spin_vertex.hpp:

Program Listing for File montercarlo_spin_vertex.hpp
====================================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_integrators_montercarlo_spin_vertex.hpp>` (``pymembrane/cppmodule/src/integrators/montercarlo_spin_vertex.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #ifndef __montercarlo_spin_vertex_HPP__
   #define __montercarlo_spin_vertex_HPP__
   
   
   #include <iostream>
   #include "montecarlointegrator.hpp"
   #include "../rng/rng.hpp"
   #include "../types/globaltypes.hpp"
   #include "../utils/fromstring.hpp"
   #include "../system/systemclass.hpp"
   class MonteCarloIntegratorSpinVertex : public MonteCarloIntegrator
   {
   public:
     MonteCarloIntegratorSpinVertex(SystemClass &system, VertexCompute &potentials) : MonteCarloIntegrator(system, potentials)
     {
       type = "monte carlo";
       name = "vertex move";
       this->set_default_properties();
     }
     ~MonteCarloIntegratorSpinVertex() {}
   
     void set_default_properties(void) override
     { 
       pymemb::vector<bool> _freezed_vertex(NUM_TYPES_ALLOWED, false);
       freezed_vertex = _freezed_vertex;
       this->set_temperature(0.0);
       m_seed = 123456; 
       m_rng = std::make_unique<RNG>(m_seed);
     }
   
     using MonteCarloIntegrator::set_property;
     void set_property(std::map<std::string, std::string> &value_map) override
     {
       for (const auto &item : value_map)
       {
         if (item.first.compare("T") == 0)
         {
           this->set_temperature(util::from_string_double(item.second));
           this->update_temperature_parameters();
         }
         else if (item.first.compare("seed") == 0)
         {
           m_seed = uint(util::from_string_int(item.second));
           m_rng = std::make_unique<RNG>(m_seed);
         }
         else
           this->print_warning_property_name(name);
       }
     }
   
     void set_property(std::map<std::string, std::map<std::string, std::string>> &value_map) override
     {
       for (const auto &item : value_map)
       {
         if (item.first.compare("freeze") == 0)
         {
           pymemb::vector<bool> _freezed_vertex = util::from_dict_to_vector_types(pymemb::copy(freezed_vertex), item.second);
           freezed_vertex = _freezed_vertex;
         }
         else
           this->print_warning_property_name(item.first);
       }
     }
     int integrate(void) override;
   
   private:
     unsigned int m_seed; 
     RNG_ptr m_rng;       
     pymemb::vector<bool> freezed_vertex;
   };
   
   typedef std::shared_ptr<MonteCarloIntegratorSpinVertex> MonteCarloIntegratorSpinVertex_ptr;
   
   #endif
   
   
