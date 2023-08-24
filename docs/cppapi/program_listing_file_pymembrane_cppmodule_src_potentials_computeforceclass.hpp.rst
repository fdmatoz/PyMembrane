
.. _program_listing_file_pymembrane_cppmodule_src_potentials_computeforceclass.hpp:

Program Listing for File computeforceclass.hpp
==============================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_potentials_computeforceclass.hpp>` (``pymembrane/cppmodule/src/potentials/computeforceclass.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef __computeforceclass_hpp__
   #define __computeforceclass_hpp__
   
   #include <memory>
   #include <map>
   #include <iostream>
   
   #include "../utils/fromdicttovec.hpp"
   #include "../utils/fromstring.hpp"
   #include "../types/globaltypes.hpp"
   #include "../system/systemclass.hpp"
   #include <pybind11/pybind11.h>
   constexpr double BIG_VERTEX_ENERGY_LIMIT = 1e15;
   
   class ComputeForceClass
   {
   public:
       ComputeForceClass(SystemClass &system) : _system(system),
                                                NUM_TYPES_ALLOWED(10), 
                                                NUM_TYPES_PAIR(NUM_TYPES_ALLOWED * NUM_TYPES_ALLOWED + 1)
       {
       }
       virtual ~ComputeForceClass() {}
       virtual void compute_energy(void) {}
       virtual void compute(void) {}
       virtual real compute_edge_energy(int) { return 0.0; }
       virtual real compute_face_energy(int) { return 0.0; }
       virtual real compute_vertex_energy(int) { return 0.0; };
   
       virtual void compute_stress(void){};
   
       virtual void compute_atomic_stress(void){};
   
       std::string get_name(void) { return m_name; }
       std::string get_type(void) { return m_type; }
       virtual std::map<std::string, std::string> get_info(void) = 0;
       virtual void set_default_properties(void) = 0;
       virtual void set_property(std::map<std::string, std::map<std::pair<std::string, std::string>, std::string>> &region_map) { this->print_warning_calling("map<string, map<pair<string, string>, string>> "); };
       virtual void set_property(std::map<std::string, std::map<std::string, std::string>> &region_map) { this->print_warning_calling("map<string, map<string, string>>"); };
       virtual void set_property(std::map<std::string, std::string> &region_map) { this->print_warning_calling("std::map<std::string, std::string>"); }
       void print_warning_calling(const std::string &message) { pybind11::print("potential ", m_name, " cannot be called with ", message); }
       void print_warning_property_name(const std::string &message) { pybind11::print("parameter ", message, " is not part of ", m_name, " potential"); }
   
   protected:
       SystemClass &_system; 
       std::string m_name;   
       std::string m_type;   
       int NUM_TYPES_ALLOWED;
       int NUM_TYPES_PAIR;
   };
   
   typedef std::unique_ptr<ComputeForceClass> ComputeForceClass_ptr;
   typedef std::map<std::string, ComputeForceClass_ptr> VertexCompute;
   
   #endif
