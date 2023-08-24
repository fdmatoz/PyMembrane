
.. _program_listing_file_pymembrane_cppmodule_src_integrators_montecarlointegrator.hpp:

Program Listing for File montecarlointegrator.hpp
=================================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_integrators_montecarlointegrator.hpp>` (``pymembrane/cppmodule/src/integrators/montecarlointegrator.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef __montecarlointegrator__HPP__
   #define __montecarlointegrator__HPP__
   
   
   #include <memory>
   #include <math.h>
   
   #include "../types/globaltypes.hpp"
   #include "../system/systemclass.hpp"
   #include "../potentials/computeforceclass.hpp"
   #include "../box/pbc.hpp"
   #include <pybind11/pybind11.h>
   namespace py = pybind11;
   
   class MonteCarloIntegrator
   {
   public:
     MonteCarloIntegrator(SystemClass &system, VertexCompute &potentials) : _system(system), _potentials(potentials), integrate_every(1), minimum_energy(0.0) 
     {
       NUM_TYPES_ALLOWED = 10;
     }
     virtual ~MonteCarloIntegrator() {}
     virtual int integrate(void) = 0;
   
     real ComputeEnergyFromEdge(const int &edge_index)
     {
       double edge_energy = 0.0;
       for (const auto& f : _potentials)
         edge_energy += f.second->compute_edge_energy(edge_index);
   
       return edge_energy;
     }
   
     real ComputeEnergyFromVertex(const int &vertex_index)
     {
       double vertex_energy = 0.0;
       for (const auto& f : _potentials)
         vertex_energy += f.second->compute_vertex_energy(vertex_index);
       return vertex_energy;
     }
   
     real ComputeEnergyFromFace(const int &vertex_index)
     {
       double face_energy = 0.0;
       for (const auto& f : _potentials)
         face_energy += f.second->compute_face_energy(vertex_index);
       return face_energy;
     }
   
     void set_temperature(double _T)
     {
       T = fabs(_T);
       update_temperature_parameters();
     }
     virtual void update_temperature_parameters(){};
   
     double get_temperature(void) const { return T; };
     std::string get_name(void) { return name; }
     std::string get_type(void) { return type; }
     int get_integrate_every() const { return integrate_every; }
     void set_integrate_every(int _integrate_every) { integrate_every = _integrate_every; }
   
     double get_minimum_energy() const { return minimum_energy; }
     void set_minimum_energy(double _minimum_energy) { minimum_energy = _minimum_energy; }
     virtual void set_default_properties(void) = 0;
     virtual void set_property(std::map<std::string, std::map<std::pair<std::string, std::string>, std::string>> &value_map) { this->print_warning_calling("map<string, map<pair<string, string>, string>> "); };
     virtual void set_property(std::map<std::string, std::map<std::string, std::string>> &value_map) { this->print_warning_calling("map<string, map<string, string>>"); };
     virtual void set_property(std::map<std::string, std::string> &value_map) { this->print_warning_calling("std::map<std::string, std::string>"); }
     void print_warning_calling(const std::string &message) { py::print("integrator ", name, " cannot be called with ", message); }
     void print_warning_property_name(const std::string &message) { py::print("parameter ", message, " is not part of ", name, "-", type, " integrator"); }
   
   protected:
     SystemClass &_system;       
     VertexCompute &_potentials; 
     std::string name;           
     std::string type;           
     real T;                     
     int integrate_every;
     double minimum_energy;    
     int NUM_TYPES_ALLOWED;
   };
   
   typedef std::shared_ptr<MonteCarloIntegrator> MonteCarloIntegrator_ptr;
   
   #endif
   
   
