
.. _program_listing_file_pymembrane_cppmodule_src_integrators_integratorclass.hpp:

Program Listing for File integratorclass.hpp
============================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_integrators_integratorclass.hpp>` (``pymembrane/cppmodule/src/integrators/integratorclass.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   /************************************************************************************
   * MIT License                                                                       *
   *                                                                                   *
   * Copyright (c) 2023 Dr. Daniel Alejandro Matoz Fernandez                           *
   *               fdamatoz@gmail.com                                                  *
   * Permission is hereby granted, free of charge, to any person obtaining a copy      *
   * of this software and associated documentation files (the "Software"), to deal     *
   * in the Software without restriction, including without limitation the rights      *
   * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell         *
   * copies of the Software, and to permit persons to whom the Software is             *
   * furnished to do so, subject to the following conditions:                          *
   *                                                                                   *
   * The above copyright notice and this permission notice shall be included in all    *
   * copies or substantial portions of the Software.                                   *
   *                                                                                   *
   * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR        *
   * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,          *
   * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE       *
   * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER            *
   * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,     *
   * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     *
   * SOFTWARE.                                                                         *
   *************************************************************************************/
   #ifndef __integratorclass__hpp__
   #define __integratorclass__hpp__
   
   #include <memory>
   #include <iostream>
   
   #include "../types/globaltypes.hpp"
   #include "../utils/fromstring.hpp"
   #include "../system/systemclass.hpp"
   #include "../box/pbc.hpp"
   #include <pybind11/pybind11.h>
   namespace py = pybind11;
   
   
   class IntegratorClass
   {
   public:
     IntegratorClass(SystemClass &system) : _system(system)
     {
     }
     virtual ~IntegratorClass()
     {
     }
     virtual void prestep(void) = 0;
     virtual void poststep(void) = 0;
     void set_temperature(const real &_T)
     {
       T = fabs(_T);
       update_temperature_parameters();
     }
     virtual void update_temperature_parameters() {}
   
     real get_temperature(void) const { return T; }
     virtual void set_time_step(const real &_dt)
     {
       dt = fabs(_dt);
       update_time_step_parameters();
     }
     virtual void update_time_step_parameters() {}
     real get_time_step(void) const { return dt; }
     std::string get_name(void) { return name; }
     std::string get_type(void) { return type; }
   
     virtual void set_default_properties(void) = 0;
     virtual void set_property(std::map<std::string, std::map<std::pair<std::string, std::string>, std::string>> &value_map) { this->print_warning_calling("map<string, map<pair<string, string>, string>> "); };
     virtual void set_property(std::map<std::string, std::map<std::string, std::string>> &value_map) { this->print_warning_calling("map<string, map<string, string>>"); };
     virtual void set_property(std::map<std::string, std::string> &value_map) { this->print_warning_calling("std::map<std::string, std::string>"); }
     void print_warning_calling(const std::string &message) { py::print("integrator ", name, " cannot be called with ", message); }
     void print_warning_property_name(const std::string &message) { py::print("parameter ", message, " is not part of ", name, "-", type, " integrator"); }
   
   protected:
     SystemClass &_system; 
     std::string name;     
     std::string type;     
     real T;               
     real dt;              
   };
   
   typedef std::shared_ptr<IntegratorClass> IntegratorClass_ptr;
   
   #endif
   
   
